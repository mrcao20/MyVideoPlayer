#include "TabWidget.h"
#include <qurl.h>
#include <qtabbar.h>
#include "WebView.h"
#include "WebPage.h"

struct TabWidgetData {
	QUrl m_mainUrl;
};

TabWidget::TabWidget(QWidget *parent)
	: QTabWidget(parent)
	, d(new TabWidgetData)
{
	connect(tabBar(), &QTabBar::tabCloseRequested, this, &TabWidget::closeTab);
	connect(this, &QTabWidget::currentChanged, this, &TabWidget::handleCurrentChanged);
}

void TabWidget::setUrl(const QUrl &url) {
	d->m_mainUrl = url;
	if (WebView *webView = currentWebView()) {
		webView->setUrl(url);
		webView->setFocus();
	}
}

void TabWidget::clear() {
	while (count() > 0) {
		if (WebView *view = webView(0)) {
			removeTab(0);
			view->deleteLater();
		}
	}
}

void TabWidget::setupView(WebView *webView){
	QWebEnginePage *webPage = webView->page();
	connect(webView, &QWebEngineView::urlChanged, [this, webView](const QUrl &url) {
		int index = indexOf(webView);
		if (index != -1)
			tabBar()->setTabData(index, url);
		if (currentIndex() == index)
			emit urlChanged(url);
	});
	connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
		int index = indexOf(webView);
		if (index != -1) {
			setTabText(index, title);
			setTabToolTip(index, title);
		}
	});
	connect(webPage, &QWebEnginePage::windowCloseRequested, [this, webView]() {
		int index = indexOf(webView);
		if (index >= 0)
			closeTab(index);
	});
	connect(webView, SIGNAL(playVideo(QUrl)), this, SIGNAL(playVideo(QUrl)));
}

WebView *TabWidget::currentWebView() const{
	return webView(currentIndex());
}

WebView *TabWidget::webView(int index) const{
	return qobject_cast<WebView*>(widget(index));
}

WebView *TabWidget::createTab(){
	WebView *webView = createBackgroundTab();
	setCurrentWidget(webView);
	return webView;
}

WebView *TabWidget::createBackgroundTab(){
	WebView *webView = new WebView(this);
	WebPage *webPage = new WebPage(webView);
	webView->setPage(webPage);
	setupView(webView);
	int index = insertTab(currentIndex() + 1, webView, tr("(Untitled)"));

	// Workaround for QTBUG-61770
	webView->resize(currentWidget()->size());
	webView->show();
	return webView;
}

void TabWidget::closeTab(int index){
	if (WebView *view = webView(index)) {
		bool hasFocus = view->hasFocus();
		removeTab(index);
		if (hasFocus && count() > 0)
			currentWebView()->setFocus();
		if (count() == 0) {
			WebView *webView = createTab();
			webView->setUrl(d->m_mainUrl);
		}
		view->deleteLater();
	}
}

void TabWidget::handleCurrentChanged(int index){
	if (index != -1) {
		WebView *view = webView(index);
		if (!view->url().isEmpty())
			view->setFocus();
		emit urlChanged(view->url());
	}
	else {
		emit urlChanged(QUrl());
	}
}

TabWidget::~TabWidget(){
}
