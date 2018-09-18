#include "MyVideoPlayer.h"
#include "PlayerWidget.h"
#include <qevent.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include "BrowserWindow.h"
#include "VideoApiFactory.h"
#include "VideoApi.h"

struct VideoPlayerDataPrivate{
	PlayerInterface *m_playerWidget;
	QScopedPointer<BrowserWindow> m_browserWindow;
	VideoApi *m_videoApi;
	QString m_videoSrc;
};

MyVideoPlayer::MyVideoPlayer(QWidget *parent)
	: Widget(parent)
	, d(new VideoPlayerDataPrivate){

	d->m_playerWidget = new PlayerWidget(this);

	init();

	resize(1200, 700);
	move(10, 10);
	show();
}

void MyVideoPlayer::init() {
	QObject *playerWidget = dynamic_cast<QObject *>(d->m_playerWidget);
	connect(playerWidget, SIGNAL(search(QString)), this, SLOT(search(QString)));
	connect(playerWidget, SIGNAL(getVideoLink(QString)), this, SLOT(getVideoLink(QString)));
}

void MyVideoPlayer::playFile(QString &fileName) {
	d->m_playerWidget->play(fileName);
}

void MyVideoPlayer::search(const QString &name) {
	d->m_playerWidget->stop();
	showMinimized();
	QUrl url;
	if (name == "ÌÚÑ¶ÊÓÆµ") {
		url = "https://v.qq.com/";
	}
	else {
		return;
	}
	d->m_videoSrc = name;
	if (!d->m_browserWindow) {
		d->m_browserWindow.reset(new BrowserWindow());
		connect(d->m_browserWindow.data(), &BrowserWindow::quit, [this]() {
			d->m_browserWindow.reset();
		});
		connect(d->m_browserWindow.data(), &BrowserWindow::playVideo, this, &MyVideoPlayer::playNetworkVideo);
	}
	d->m_browserWindow->setUrl(url);
	d->m_browserWindow->showMaximized();
}

void MyVideoPlayer::playNetworkVideo(const QUrl &url) {
	d->m_browserWindow->clear();
	d->m_browserWindow->showMinimized();
	VideoApiFactory factory;
	d->m_videoApi = factory.getVideoApi(d->m_videoSrc, this);
	d->m_videoApi->initApi(url);
	d->m_playerWidget->setPlayList(d->m_videoApi->getVideoInfoList());
	showNormal();
}

void MyVideoPlayer::getVideoLink(const QString &id) {
	QString link = d->m_videoApi->getVideo(id);
	playFile(link);
}

void MyVideoPlayer::closeEvent(QCloseEvent *event) {
	d->m_browserWindow.reset();
	delete d->m_playerWidget;
}

MyVideoPlayer::~MyVideoPlayer() {
}
