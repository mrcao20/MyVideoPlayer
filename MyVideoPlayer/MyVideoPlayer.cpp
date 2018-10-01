#include "MyVideoPlayer.h"
#include "PlayerWidget.h"
#include <qevent.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qdebug.h>
#include "BrowserWindow.h"
#include "VideoApiFactory.h"
#include "VideoApi.h"
#include "TcpClient.h"

struct VideoPlayerDataPrivate{
	QScopedPointer<BrowserWindow> m_browserWindow;
	PlayerInterface *m_playerWidget;
	VideoApi *m_videoApi;
	QString m_videoSrc;
	TcpClient *m_tcpClient;
};

MyVideoPlayer::MyVideoPlayer(QWidget *parent)
	: Widget(parent)
	, d(new VideoPlayerDataPrivate){

	d->m_playerWidget = new PlayerWidget(this);
	d->m_tcpClient = new TcpClient(this);
	d->m_tcpClient->init();

	//playFile(R"(E:\迅雷下载\video\触不可及_bd.mp4)");

	init();

	resize(1200, 700);
	move(10, 10);
	show();
}

void MyVideoPlayer::init() {
	PlayerWidget *playerWidget = dynamic_cast<PlayerWidget *>(d->m_playerWidget);
	if (playerWidget) {
		connect(playerWidget, SIGNAL(search(QString)), this, SLOT(search(QString)));
		connect(playerWidget, &PlayerWidget::getVideoLink, [this](const QString &id) {
			QString link = d->m_videoApi->getVideo(id);
			playFile(link);
		});
	}
}

void MyVideoPlayer::playFile(const QString &fileName) {
	d->m_playerWidget->play(fileName);
}

void MyVideoPlayer::search(const QString &name) {
	d->m_playerWidget->stop();
	showMinimized();
	QUrl url;
	if (name == "腾讯视频") {
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
	d->m_browserWindow->setFocus();
}

void MyVideoPlayer::playNetworkVideo(const QUrl &url) {
	d->m_browserWindow->clear();
	d->m_browserWindow->showMinimized();
	VideoApiFactory factory;
	d->m_videoApi = factory.getVideoApi(d->m_videoSrc, this);
	d->m_videoApi->initApi(url);
	d->m_playerWidget->setPlayList(d->m_videoApi->getVideoNames(), d->m_videoApi->getVideoInfoList());
	showNormal();
}

void MyVideoPlayer::closeEvent(QCloseEvent *event) {
	event->accept();
	hide();
	if (d->m_browserWindow) {
		d->m_browserWindow->clear();
	}
	PlayerWidget *playerWidget = dynamic_cast<PlayerWidget *>(d->m_playerWidget);
	if (playerWidget) {
		playerWidget->quit();
	}
	QApplication::exit(0);
}

MyVideoPlayer::~MyVideoPlayer() {
}
