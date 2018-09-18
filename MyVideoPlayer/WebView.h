#pragma once

#include <QWebEngineView>
#include <QUrl>

class WebView : public QWebEngineView{
	Q_OBJECT

public:
	explicit WebView(QWidget *parent = 0);
	~WebView();

signals:
	void playVideo(const QUrl &url);

protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
	QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
};
