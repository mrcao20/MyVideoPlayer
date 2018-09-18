#pragma once

#include <qvideowidget.h>

class VideoWidget : public QVideoWidget{
	Q_OBJECT

public:
	explicit VideoWidget(QWidget *parent = 0);
	virtual ~VideoWidget() = default;

signals:
	void statusChanged();
	void positionChanged(bool isForward);

protected:
	void keyPressEvent(QKeyEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

};

