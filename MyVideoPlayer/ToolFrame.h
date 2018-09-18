#pragma once
#pragma execution_character_set("UTF-8")
#include <QFrame>
#include <qscopedpointer.h>

struct ToolFrameData;

class ToolFrame : public QFrame{
	Q_OBJECT

public:
	explicit ToolFrame(QWidget *parent = 0);
	~ToolFrame();

	void positionSeekForward();
	void positionSeekBackward();

signals:
	void positionValueChanged(int value);
	void volumeValueChanged(int value);
	void search(const QString &name);

public slots:
	void positionUpdateDuration(qint64 duration);
	void positionUpdatePosition(qint64 position);

private:
	void init();
	void loadStyleSheet(QWidget *widget, QString fileName);

private:
	QScopedPointer<ToolFrameData> d;
};
