#include "Slider.h"
#include <qevent.h>

struct SliderData {
	bool m_isPress;
};

Slider::Slider(QWidget *parent)
	: QSlider(parent) 
	, d(new SliderData)
{
}

int Slider::getCursorCurrentPos() {
	QPoint mousePoint = parentWidget()->mapFromGlobal(QCursor::pos());
	int position = (mousePoint.x() - x()) / (width() * 1.0) * maximum();
	return position;
}

void Slider::mouseMoveEvent(QMouseEvent *event) {
	if (d->m_isPress) {
		int pos = getCursorCurrentPos();
		setValue(pos);
		return;
	}
	QSlider::mouseMoveEvent(event);
}

void Slider::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		int pos = getCursorCurrentPos();
		setValue(pos);
		d->m_isPress = true;
		return;
	}
	QSlider::mousePressEvent(event);
}

void Slider::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		d->m_isPress = false;
		return;
	}
	QSlider::mouseReleaseEvent(event);
}
