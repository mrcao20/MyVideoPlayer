#include "Widget.h"
#include <qfile.h>

Widget::Widget(QWidget *parent)
	:QWidget(parent) {

}

void Widget::loadStyleSheet(QWidget *widget, QString fileName) {
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly))
		return;
	QByteArray byteArray = file.readAll();
	widget->setStyleSheet(byteArray);
}