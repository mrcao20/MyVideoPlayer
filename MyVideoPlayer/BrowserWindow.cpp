#include "BrowserWindow.h"
#include <qlayout.h>
#include <qevent.h>
#include <QApplication>
#include <QDesktopWidget>
#include <qlineedit.h>
#include <qaction.h>
#include "TabWidget.h"

struct BrowserWindowsData{
	TabWidget *m_tabWidget;
	QLineEdit *m_urlLineEdit;
};

BrowserWindow::BrowserWindow(QWidget *parent)
	: QWidget(parent)
	, d(new BrowserWindowsData)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setFocusPolicy(Qt::ClickFocus);

	d->m_tabWidget = new TabWidget(this);
	d->m_tabWidget->setTabsClosable(true);
	d->m_urlLineEdit = new QLineEdit(this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->addWidget(d->m_urlLineEdit);
	layout->addWidget(d->m_tabWidget);
	setLayout(layout);
	
	init();
}

QSize BrowserWindow::sizeHint() const{
	QRect desktopRect = QApplication::desktop()->screenGeometry();
	QSize size = desktopRect.size() * qreal(0.9);
	return size;
}

void BrowserWindow::init() {
	connect(d->m_tabWidget, SIGNAL(playVideo(QUrl)), this, SIGNAL(playVideo(QUrl)));
	connect(d->m_tabWidget, &TabWidget::urlChanged, [this](const QUrl &url) {
		d->m_urlLineEdit->setText(url.toDisplayString());
	});
	connect(d->m_urlLineEdit, &QLineEdit::returnPressed, [this]() {
		d->m_tabWidget->setUrl(QUrl::fromUserInput(d->m_urlLineEdit->text()));
	});
	QAction *focusUrlLineEditAction = new QAction(this);
	addAction(focusUrlLineEditAction);
	focusUrlLineEditAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	connect(focusUrlLineEditAction, &QAction::triggered, this, [this]() {
		d->m_urlLineEdit->setFocus(Qt::ShortcutFocusReason);
	});
}

void BrowserWindow::setUrl(const QUrl &url) {
	d->m_tabWidget->createTab();
	d->m_tabWidget->setUrl(url);
}

void BrowserWindow::clear() {
	d->m_tabWidget->clear();
}

TabWidget *BrowserWindow::tabWidget() const {
	return d->m_tabWidget;
}

void BrowserWindow::closeEvent(QCloseEvent *event) {
	event->accept();
	emit quit();
}

BrowserWindow::~BrowserWindow(){
}
