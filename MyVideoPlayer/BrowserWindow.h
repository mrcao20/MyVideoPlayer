#pragma once

#include <QWidget>
#include <qurl.h>
#include <qscopedpointer.h>

class TabWidget;

struct BrowserWindowsData;

class BrowserWindow : public QWidget{
	Q_OBJECT

public:
	explicit BrowserWindow(QWidget *parent = 0);
	~BrowserWindow();

	QSize sizeHint() const override;
	void setUrl(const QUrl &url);
	void clear();
	TabWidget *tabWidget() const;

protected:
	void closeEvent(QCloseEvent *event) override;

signals:
	void quit();
	void playVideo(const QUrl &url);

private:
	void init();

private:
	QScopedPointer<BrowserWindowsData> d;
};
