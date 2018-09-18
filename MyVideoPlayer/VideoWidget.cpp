#include "VideoWidget.h"
#include <qevent.h>
#include <qdebug.h>

VideoWidget::VideoWidget(QWidget *parent)
	: QVideoWidget(parent) {

	setFocus();
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
}

void VideoWidget::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Escape && isFullScreen()) {
		setFullScreen(false);
		event->accept();
	}
	else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		emit statusChanged();
		event->accept();
	}
	else if (event->key() == Qt::Key_Left) {
		emit positionChanged(false);
		event->accept();
	}
	else if (event->key() == Qt::Key_Right) {
		emit positionChanged(true);
		event->accept();
	}
	else {
		QVideoWidget::keyPressEvent(event);
	}
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		setFullScreen(!isFullScreen());
		event->accept();
	}
	else {
		QVideoWidget::mouseDoubleClickEvent(event);
	}
}
