#include "PlayerWidget.h"
#include <ui_PlayerWidget.h>
#include <qurl.h>
#include <qmediaplayer.h>
#include <qgridlayout.h>
#include <QTimer>
#include <qlistwidget.h>
#include "VideoWidget.h"
#include "ToolFrame.h"
#include "ScreenSaverCtrl.h"

struct PlayerWidgetData : public Ui::PlayerWidget {
	QMediaPlayer m_mediaPlayer;
	QVideoWidget *m_videoWidget;
	QFrame *m_toolFrame;
	QListWidget *m_listWidget;
	bool m_isPause;
	QStringList m_videoInfos;
	ScreenSaverCtrl m_screenSaverStrl;
};

PlayerWidget::PlayerWidget(QWidget *parent)
	: QObject(parent)
	, d(new PlayerWidgetData)
{
	d->setupUi(parent);

	d->m_toolFrame = new ToolFrame(parent);
	d->m_videoWidget = new VideoWidget(parent);
	d->m_listWidget = new QListWidget(parent);

	d->m_toolFrame->setFixedHeight(60);
	d->m_listWidget->setFixedWidth(150);

	QGridLayout *layout = new QGridLayout(parent);
	layout->addWidget(d->m_videoWidget, 0, 0);
	layout->addWidget(d->m_toolFrame, 1, 0);
	layout->addWidget(d->m_listWidget, 0, 1, 2, 1);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	parent->setLayout(layout);

	init();
}

void PlayerWidget::init() {
	QTimer *screenSaverTimer = new QTimer(this);
	qint64 time = d->m_screenSaverStrl.GetScreenSaverTimeout() * 1000 - 2000;
	connect(screenSaverTimer, &QTimer::timeout, [this, screenSaverTimer]() {
		d->m_screenSaverStrl.SetScreenSaverActive(false);
		d->m_screenSaverStrl.SetScreenSaverActive(true);
		qint64 time = d->m_screenSaverStrl.GetScreenSaverTimeout() * 1000 - 2000;
		screenSaverTimer->start(time);
	});
	screenSaverTimer->start(time);

	d->m_mediaPlayer.setVideoOutput(d->m_videoWidget);

	connect(d->m_videoWidget, SIGNAL(statusChanged()), SLOT(statusChanged()));
	connect(d->m_videoWidget, SIGNAL(positionChanged(bool)), SLOT(positionChanged(bool)));
	connect(&d->m_mediaPlayer, SIGNAL(durationChanged(qint64)), d->m_toolFrame, SLOT(positionUpdateDuration(qint64)));
	connect(&d->m_mediaPlayer, SIGNAL(positionChanged(qint64)), d->m_toolFrame, SLOT(positionUpdatePosition(qint64)));
	connect(d->m_toolFrame, SIGNAL(positionValueChanged(int)), this, SLOT(updatePosition(int)));
	connect(d->m_toolFrame, SIGNAL(volumeValueChanged(int)), this, SLOT(updateVolume(int)));
	connect(d->m_toolFrame, SIGNAL(search(QString)), this, SIGNAL(search(QString)));
	connect(d->m_listWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem *item) {
		int index = d->m_listWidget->row(item);
		emit getVideoLink(d->m_videoInfos.at(index));
	});
}

void PlayerWidget::play(QString &path) {
	QUrl url;
	if (path.startsWith("http")) {
		url = QUrl(path);
	}
	else {
		url = QUrl::fromLocalFile(path);
	}
	d->m_mediaPlayer.setMedia(url);
	d->m_mediaPlayer.play();
}

void PlayerWidget::stop() {
	pause();
}

void PlayerWidget::pause() {
	d->m_mediaPlayer.pause();
}

void PlayerWidget::resume() {
	d->m_mediaPlayer.play();
}

void PlayerWidget::seekForward() {
	qobject_cast<ToolFrame *>(d->m_toolFrame)->positionSeekForward();
}

void PlayerWidget::seekBackward() {
	qobject_cast<ToolFrame *>(d->m_toolFrame)->positionSeekBackward();
}

void PlayerWidget::setPlayList(QStringList &videoInfo) {
	d->m_videoInfos.swap(videoInfo);
	for (int i = 0; i < d->m_videoInfos.size(); i++) {
		d->m_listWidget->addItem(QString::number(i));
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

void PlayerWidget::updatePosition(int value) {
	if (qAbs(d->m_mediaPlayer.position() - value) > (5 * 100)) {
		d->m_mediaPlayer.pause();
		d->m_mediaPlayer.setPosition(value);
		d->m_mediaPlayer.play();
	}
}

void PlayerWidget::updateVolume(int value) {
	d->m_mediaPlayer.setVolume(value);
}

PlayerWidget::~PlayerWidget(){
}
