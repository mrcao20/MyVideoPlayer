#pragma once

#include <QTabWidget>
#include <qurl.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class WebView;

struct TabWidgetData;

class TabWidget : public QTabWidget
{
	Q_OBJECT

public:
	explicit TabWidget(QWidget *parent = 0);
	~TabWidget();

	void setUrl(const QUrl &url);
	void clear();

signals:
	void playVideo(const QUrl &url);

public slots:
	WebView *createTab();
	WebView *createBackgroundTab();
	void closeTab(int index);

private:
	WebView *currentWebView() const;
	WebView *webView(int index) const;
	void setupView(WebView *webView);

private:
	QScopedPointer<TabWidgetData> d;
};
