#pragma once

#include <qslider.h>
#include <qscopedpointer.h>

struct SliderData;

class Slider : public QSlider{
	Q_OBJECT

public:
	explicit Slider(QWidget *parent = 0);
	virtual ~Slider() = default;

protected:
	void mouseMoveEvent(QMouseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	int getCursorCurrentPos();

private:
	QScopedPointer<SliderData> d;
};
