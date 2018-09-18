#include "WebView.h"
#include <qmenu.h>
#include <QAction>
#include <qevent.h>
#include "BrowserWindow.h"
#include "TabWidget.h"

WebView::WebView(QWidget *parent)
	: QWebEngineView(parent)
{

}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type){
	BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(window());
	if (!mainWindow)
		return nullptr;

	switch (type) {
	case QWebEnginePage::WebBrowserTab: {
		return mainWindow->tabWidget()->createTab();
	}
	case QWebEnginePage::WebBrowserBackgroundTab: {
		return mainWindow->tabWidget()->createBackgroundTab();
	}
	case QWebEnginePage::WebBrowserWindow: {
		break;
	}
	case QWebEnginePage::WebDialog: {
		break;
	}
	}
	return nullptr;
}

void WebView::contextMenuEvent(QContextMenuEvent *event){
	QMenu *menu = page()->createStandardContextMenu();
	const QList<QAction*> actions = menu->actions();
	auto it = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::OpenLinkInThisWindow));
	if (it != actions.cend()) {
		(*it)->setText(tr("Open Link in This Tab"));
		QAction *before1(it == actions.cend() ? nullptr : *it);
		menu->insertAction(before1, page()->action(QWebEnginePage::OpenLinkInNewTab));
		++it;
		QAction *before2(it == actions.cend() ? nullptr : *it);
		menu->insertAction(before2, page()->action(QWebEnginePage::OpenLinkInNewWindow));
	}
	QAction *playAction = menu->addAction("play this video");
	connect(playAction, &QAction::triggered, [this]() {
		emit playVideo(page()->url());
	});
	menu->popup(event->globalPos());
}

WebView::~WebView(){
}
