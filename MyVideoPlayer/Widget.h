#pragma once

#include <qwidget.h>

class Widget : public QWidget{

public:
	Widget(QWidget *parent = 0);
	virtual ~Widget() = default;

protected:
	void loadStyleSheet(QWidget *widget, QString fileName);

private:

};