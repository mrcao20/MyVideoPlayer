#include "PlayerWidget.h"
#include <ui_PlayerWidget.h>
#include <qurl.h>
#include <qmediaplayer.h>
#include <QTimer>
#include <QFile>
#include <QTime>
#include <qdir.h>
#include <QtConcurrent>
#include "VideoWidget.h"
#include "ScreenSaverCtrl.h"
#include "Global.h"
#include "NetworkTools.h"

OUTPUT_ENUM(QMediaPlayer, Error);

struct PlayerWidgetData : public Ui::PlayerWidget {
	bool m_isGetVideoThreadQuit;
	QString m_appPath;
	QString m_currVideoName;
	QMediaPlayer m_mediaPlayer;
	bool m_isPause;
	bool m_isCachedPlay;
	bool m_isGetVideoThreadRunning;
	QStringList m_videoInfos;
	ScreenSaverCtrl m_screenSaverStrl;
	QMap<qint64, VideoTime> m_tsTimeCollection;
	qint64 m_currTsTime;
	int m_currTsIndex;
	QLabel *m_currPosLabel;
	QUrl m_currUrl;
	qint64 m_mediaCurrPos;
};

PlayerWidget::PlayerWidget(QWidget *parent)
	: QObject(parent)
	, d(new PlayerWidgetData)
{
	d->setupUi(parent);

	d->m_isGetVideoThreadQuit = false;
	d->m_isGetVideoThreadRunning = false;
	d->m_isCachedPlay = false;
	d->m_mediaCurrPos = 0;

	init(parent);

	d->m_appPath = qApp->applicationDirPath();
#ifdef _DEBUG
	d->m_appPath += "/../Release";
#endif // _DEBUG
	QString qssPath = d->m_appPath + "/data/qss/";
	QString toolFrameQSS = qssPath + "ToolFrame.qss";
	QString searchButonQSS = qssPath + "SearchButton.qss";
	QString positionSliderQSS = qssPath + "SliderPosition.qss";
	QString volumeSliderQSS = qssPath + "SliderVolume.qss";
	loadStyleSheet(d->toolFrame, toolFrameQSS);
	loadStyleSheet(d->searchButton, searchButonQSS);
	loadStyleSheet(d->positionSlider, positionSliderQSS);
	loadStyleSheet(d->volumeSlider, volumeSliderQSS);
}

void PlayerWidget::init(QWidget *w) {
	d->m_currPosLabel = new QLabel(w);
	d->m_currPosLabel->setFixedSize(51, 16);
	d->m_currPosLabel->setStyleSheet("color:rgb(204, 204, 204);");
	d->m_currPosLabel->hide();

	QTimer *screenSaverTimer = new QTimer(this);
	qint64 time = d->m_screenSaverStrl.GetScreenSaverTimeout() * 1000 - 2000;
	connect(screenSaverTimer, &QTimer::timeout, [this, screenSaverTimer]() {
		d->m_screenSaverStrl.SetScreenSaverActive(false);
		d->m_screenSaverStrl.SetScreenSaverActive(true);
		qint64 time = d->m_screenSaverStrl.GetScreenSaverTimeout() * 1000 - 2000;
		screenSaverTimer->start(time);
	});
	screenSaverTimer->start(time);

	d->m_mediaPlayer.setVideoOutput(d->videoWidget);

	connect(d->videoWidget, SIGNAL(statusChanged()), SLOT(statusChanged()));
	connect(d->videoWidget, SIGNAL(positionChanged(bool)), SLOT(positionChanged(bool)));
	connect(&d->m_mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(positionUpdateDuration(qint64)));
	connect(&d->m_mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionUpdatePosition(qint64)));
	connect(&d->m_mediaPlayer, &QMediaPlayer::videoAvailableChanged, [this](bool a) {
		if (a) {
			play();
			d->m_mediaPlayer.setPosition(d->m_mediaCurrPos);
		}
	});
	connect(&d->m_mediaPlayer, qOverload<QMediaPlayer::Error>(&QMediaPlayer::error), [this](QMediaPlayer::Error error) {
		d->m_mediaPlayer.stop();
		output << d->m_mediaPlayer.error();
	});
	connect(d->listWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item) {
		int index = d->listWidget->row(item);
		d->m_currVideoName = d->m_videoInfos.at(index);
		emit getVideoLink(d->m_currVideoName);
	});
	connect(d->positionSlider, &QSlider::valueChanged, this, &PlayerWidget::updatePosition);
	connect(d->positionSlider, &Slider::showCurrPos, [this, w](int pos) {
		d->m_currPosLabel->setText(posToStrTime(pos));
		QPoint p = w->mapFromGlobal(QCursor::pos());
		d->m_currPosLabel->move(p - QPoint(0, d->m_currPosLabel->height()));
		d->m_currPosLabel->show();
	});
	connect(d->positionSlider, &Slider::cursorLeave, [this]() {
		d->m_currPosLabel->hide();
	});
	connect(d->volumeSlider, &QSlider::valueChanged, this, &PlayerWidget::updateVolume);
	connect(d->searchButton, &QPushButton::clicked, [this]() {
		QString name = d->searchComboBox->currentText();
		emit search(name);
	});
	connect(d->cb_isCachedPlay, &QCheckBox::clicked, [this](bool checked) {
		if (d->m_currUrl.isLocalFile()) {
			d->cb_isCachedPlay->setChecked(false);
			return;
		}
		d->m_isCachedPlay = checked;
		d->m_isGetVideoThreadQuit = !checked;
		qint64 pos = d->m_mediaPlayer.position();
		play(d->m_currUrl.toString());
		d->m_mediaCurrPos = pos;
	});
}

void PlayerWidget::play() {
	d->m_mediaPlayer.play();
	d->m_isPause = false;
}

void PlayerWidget::loadStyleSheet(QWidget *widget, QString fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray byteArray = file.readAll();
	widget->setStyleSheet(byteArray);
}

void PlayerWidget::playNormal(const QUrl &url) {
	if (d->cb_isCachedPlay->isChecked())
		d->cb_isCachedPlay->setChecked(false);
	d->m_mediaPlayer.setMedia(url);
}

void PlayerWidget::playCached(const QUrl &url) {
	d->m_currTsTime = 0;
	NetworkTools networkTools;
	QByteArray data = networkTools.getNetworkData(url);
	if (data.isEmpty()) {
		output << "获取视频数据失败";
		return;
	}
	// 在video文件夹下以视频名新建一个文件夹用于存放m3u8以及ts文件
	QString videoPath = d->m_appPath + "/video/" + d->m_currVideoName;
	QDir dir;
	dir.mkpath(videoPath);
	// m3u8文件命名为视频名+.ts.m3u8
	QString video = QString("%1/%2.ts.m3u8").arg(videoPath, d->m_currVideoName);
	QFile file(video);
	if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
		return;
	QTextStream out(&file);
	QTextStream in(data);
	QString urlStr = url.toString();
	// 取出最后一个/的位置，/之后为不同的视频分段地址
	int index = urlStr.lastIndexOf(QRegularExpression(R"(/)"));
	// index表示最后一个/的位置，由于位置下标从0开始，故index为/之前的字符个数，index+1才表示包含/在内的字符个数
	QString urlPrefix = urlStr.left(index + 1);
	QStringList tsLinks;
	int i = 0;
	QString str;
	d->m_tsTimeCollection.clear();
	while (in.readLineInto(&str)) {
		if (str.startsWith("#")) {
			out << str << endl;
			QRegularExpression re(R"(#EXTINF:(.*?),)");
			QRegularExpressionMatch match = re.match(str);
			QStringList list = match.capturedTexts();
			if (list.size() != 2)
				continue;
			qint64 endTime = list.at(1).toDouble() * 1000;	// 将时间转换为毫秒
			qint64 startTime = 0;
			if (!d->m_tsTimeCollection.isEmpty()) {
				startTime = d->m_tsTimeCollection.last().m_endTime;
			}
			endTime += startTime;
			d->m_tsTimeCollection.insert(startTime, VideoTime(startTime, endTime));
		}
		else {
			tsLinks.append(urlPrefix + str);
			out << QString("%1/%2.ts").arg(videoPath, QString::number(i++)) << endl;
		}
	}
	out.flush();
	file.close();
	QtConcurrent::run(this, &PlayerWidget::getVideo, videoPath, tsLinks);
}

void PlayerWidget::getVideo(const QString &currVideoPath, const QStringList &tsLinks) {
	d->m_isGetVideoThreadRunning = true;
	NetworkTools networkTools;
	bool isPlay = false;		// 是否需要播放视频
	bool isPlaying = false;		// 是否正在播放视频
	for (d->m_currTsIndex = 0; d->m_currTsIndex < tsLinks.size(); d->m_currTsIndex++) {
		if (d->m_isGetVideoThreadQuit) {
			emit getVideoThreadExit();
			break;
		}
		QFile file(QString("%1/%2.ts").arg(currVideoPath, QString::number(d->m_currTsIndex)));
		if (file.exists())
			continue;
		QString link = tsLinks.at(d->m_currTsIndex);
		QByteArray data = networkTools.getNetworkData(link);
		if (!file.open(QIODevice::WriteOnly)) {
			output << "无法创建ts分段视频文件";
			break;
		}
		file.write(data.data(), data.size());
		file.close();
		emit newTsDownloaded(d->m_currTsIndex);
		if (d->m_currTsIndex >= 2 && !isPlaying)
			isPlay = true;
		if (isPlay) {
			isPlay = false;
			isPlaying = true;
			d->m_mediaPlayer.setMedia(QUrl::fromLocalFile(QString("%1/%2.ts.m3u8").arg(currVideoPath, d->m_currVideoName)));
		}
	}
	d->m_isGetVideoThreadRunning = false;
}

bool PlayerWidget::updateCurrTime(const qint64 &position) {
	static int index = 0;		// 当前正在播放的片段
	static bool isCurrExists = false;
	static bool isNextExists = false;
	VideoTime vt = d->m_tsTimeCollection[d->m_currTsTime];
	// 如果播放到最后一个片段且没有手动往前跳动时，表示正常播放，直接返回true
	if (index >= d->m_tsTimeCollection.size() && position >= vt.m_startTime)
		return true;
	// 如果播放的不是当前片段，需要判断新片段是否存在
	if (position >= vt.m_endTime || position < vt.m_startTime) {
		getCurrTsInfo(position, vt, index);
		d->m_currTsTime = vt.m_startTime;
		isCurrExists = false;
		isNextExists = false;
	}
	QString videoPath = d->m_appPath + "/video/" + d->m_currVideoName;
	// 判断需要播放的片段是否存在
	if (!isCurrExists) {
		int currIndex = index;
		if (QFile::exists(QString("%1/%2.ts").arg(videoPath, QString::number(currIndex)))) {
			isCurrExists = true;
		}
		else {	// 不存在，则等待其存在
			waitDownloaded(currIndex, position);
			return false;
		}
	}
	// 判断下一个片段是否存在
	if (position >= vt.m_endTime - 2000 && !isNextExists) {
		int nextIndex = index + 1;
		if (QFile::exists(QString("%1/%2.ts").arg(videoPath, QString::number(nextIndex))) || nextIndex >= d->m_tsTimeCollection.size()) {
			isNextExists = true;
			return true;
		}
		// 不存在，则等待其存在
		waitDownloaded(nextIndex, position);
		return false;
	}
	// 所有条件都不满足，表示正在正常播放，直接判断true
	return true;
}

void PlayerWidget::getCurrTsInfo(const qint64 &position, VideoTime &videoTime, int &index) {
	/*	由于QMap的key和find都需要遍历整个容器，故提前判断不仅没有提升效率，反而大幅度降低效率
		所以此处不在提前做一次判断，直接遍历容器查找
	*/
	/*auto itr = ++d->m_tsTimeCollection.find(d->m_tsTimeCollection.key(videoTime));
	if (itr != d->m_tsTimeCollection.end()) {
		if (position >= itr.value().m_startTime && position <= itr.value().m_endTime) {
			videoTime = itr.value();
			index++;
			return;
		}
	}*/
	int i = 0;
	for (auto itr = d->m_tsTimeCollection.begin(); itr != d->m_tsTimeCollection.end(); itr++, i++) {
		VideoTime vt = itr.value();
		if (position >= vt.m_startTime && position <= vt.m_endTime) {
			videoTime = vt;
			index = i;
			return;
		}
	}
}

void PlayerWidget::waitDownloaded(int index, const qint64 &position) {
	d->m_currTsIndex = index;
	pause();
	connect(this, &PlayerWidget::newTsDownloaded, [this, index, position](int i) {
		if (i >= index) {
			resume();
			d->m_mediaPlayer.setPosition(position);
			disconnect(this, &PlayerWidget::newTsDownloaded, 0, 0);
		}
	});
}

QString PlayerWidget::posToStrTime(const qint64 &pos) {
	QTime duration(pos / 3600000, (pos / 60000) % 60, (pos / 1000) % 60);
	return duration.toString("hh:mm:ss");
}

void PlayerWidget::play(const QString &path) {
	d->m_mediaCurrPos = 0;
	bool isNet = false;
	QUrl url;
	if (path.startsWith("http")) {
		url = QUrl(path);
		isNet = true;
	}
	else {
		url = QUrl::fromLocalFile(path);
		isNet = false;
	}
	d->m_currUrl = url;
	if (d->m_isCachedPlay && isNet)
		playCached(url);
	else 
		playNormal(url);
}

void PlayerWidget::stop() {
	d->m_mediaPlayer.stop();
	d->m_isPause = true;
}

void PlayerWidget::pause() {
	d->m_mediaPlayer.pause();
	d->m_isPause = true;
}

void PlayerWidget::resume() {
	play();
}

void PlayerWidget::seekForward() {
	pause();
	d->positionSlider->triggerAction(QSlider::SliderPageStepAdd);
	play();
}

void PlayerWidget::seekBackward() {
	pause();
	d->positionSlider->triggerAction(QSlider::SliderSingleStepSub);
	play();
}

void PlayerWidget::setPlayList(const QStringList &videoNames, QStringList &videoInfo) {
	d->m_videoInfos.swap(videoInfo);
	d->listWidget->clear();
	QStringList list;
	list = videoNames.isEmpty() ? d->m_videoInfos : videoNames;
	for (const QString &name : list) {
		d->listWidget->addItem(name);
	}
}

void PlayerWidget::statusChanged() {
	d->m_isPause = !d->m_isPause;
	if (d->m_isPause) {
		pause();
	}
	else {
		resume();
	}
}

void PlayerWidget::positionChanged(bool isForward) {
	if (isForward)
		seekForward();
	else
		seekBackward();
}

// 判断是否手动移动进度条，更新视频进度
void PlayerWidget::updatePosition(const int &value) {
	// 视频自动更新也会移动进度条，此时不在重复更新视频进度
	if (qAbs(d->m_mediaPlayer.position() - value) > (5 * 100)) {
		if (d->m_isCachedPlay) {
			if (!updateCurrTime(value))
				return;
		}
		pause();
		d->m_mediaPlayer.setPosition(value);
		resume();
	}
}

void PlayerWidget::updateVolume(const int &value) {
	d->m_mediaPlayer.setVolume(value);
}

void PlayerWidget::positionUpdateDuration(const qint64 &duration) {
	d->durationLabel->setText("/" + posToStrTime(duration));
	d->positionSlider->setRange(0, duration);
	d->positionSlider->setEnabled(duration > 0);
	d->positionSlider->setPageStep(6000);
	d->positionSlider->setSingleStep(3000);
}

// 视频自动更新进度，移动进度条
void PlayerWidget::positionUpdatePosition(const qint64 &position) {
	static qint64 pos = 0;
	if (pos == position)
		return;
	pos = position;
	if (d->m_isCachedPlay) {
		updateCurrTime(position);
	}
	d->positionSlider->setValue(position);
	d->positionLabel->setText(posToStrTime(position));
}

void PlayerWidget::quit() {
	d->m_isGetVideoThreadQuit = true;
	stop();
	if (d->m_isGetVideoThreadRunning) {
		QEventLoop loop;
		connect(this, &PlayerWidget::getVideoThreadExit, &loop, &QEventLoop::quit);
		loop.exec();
	}
}

PlayerWidget::~PlayerWidget(){
}
