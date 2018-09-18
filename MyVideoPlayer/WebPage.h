#pragma once

#include <QWebEnginePage>

class WebPage : public QWebEnginePage{
	Q_OBJECT

public:
	explicit WebPage(QWidget *parent = 0);
	~WebPage();
};
