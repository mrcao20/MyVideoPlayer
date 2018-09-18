#pragma once

#include "Widget.h"
#include <qscopedpointer.h>

struct VideoPlayerDataPrivate;

class MyVideoPlayer : public Widget{
	Q_OBJECT

public:
	explicit MyVideoPlayer(QWidget *parent = Q_NULLPTR);
	~MyVideoPlayer();

	void playFile(QString &fileName);

protected:
	void closeEvent(QCloseEvent *event) override;

private slots:
	void search(const QString &name);
	void playNetworkVideo(const QUrl &url);
	void getVideoLink(const QString &id);

private:
	void init();

private:
	QScopedPointer<VideoPlayerDataPrivate> d;
};
