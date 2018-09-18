#pragma once
#pragma execution_character_set("UTF-8")

#include "PlayerInterface.h"
#include <QObject>
#include <qscopedpointer.h>

struct PlayerWidgetData;

class PlayerWidget : public QObject, public PlayerInterface {
	Q_OBJECT

public:
	explicit PlayerWidget(QWidget *parent = 0);
	~PlayerWidget();

	void play(QString &path) override;
	void stop() override;
	void pause() override;
	void resume() override;
	void seekForward() override;
	void seekBackward() override;
	void setPlayList(QStringList &videoInfo) override;

signals:
	void search(const QString &name);
	void getVideoLink(const QString &id);

private slots:
	void statusChanged();
	void positionChanged(bool isForward);
	void updatePosition(int value);
	void updateVolume(int value);

private:
	void init();

private:
	QScopedPointer<PlayerWidgetData> d;
};
