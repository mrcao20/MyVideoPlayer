#pragma once
#pragma execution_character_set("UTF-8")

#include "PlayerInterface.h"
#include <QObject>
#include <QUrl>
#include <qscopedpointer.h>

struct VideoTime {
	qint64 m_startTime;
	qint64 m_endTime;
	VideoTime(){}
	VideoTime(qint64 startTime, qint64 endTime)
		: m_startTime(startTime)
		, m_endTime(endTime) {}
};

struct PlayerWidgetData;

class PlayerWidget : public QObject, public PlayerInterface {
	Q_OBJECT

public:
	explicit PlayerWidget(QWidget *parent = 0);
	~PlayerWidget();

	void play(const QString &path) override;
	void stop() override;
	void pause() override;
	void resume() override;
	void seekForward() override;
	void seekBackward() override;
	void setPlayList(const QStringList &videoNames, QStringList &videoInfo) override;
	void quit();

signals:
	void search(const QString &name);
	void getVideoLink(const QString &id);
	void getVideoThreadExit();
	void newTsDownloaded(int i);

private slots:
	void statusChanged();
	void positionChanged(bool isForward);
	void updatePosition(const int &value);
	void updateVolume(const int &value);
	void positionUpdateDuration(const qint64 &duration);
	void positionUpdatePosition(const qint64 &position);

private:
	void init(QWidget *w);
	void play();
	void loadStyleSheet(QWidget *widget, QString fileName);
	void playNormal(const QUrl &url);
	void playCached(const QUrl &url);
	void getVideo(const QString &currVideoPath, const QStringList &tsLinks);
	bool updateCurrTime(const qint64 &position);
	void getCurrTsInfo(const qint64 &position, VideoTime &videoTime, int &index);
	void waitDownloaded(int index, const qint64 &position);
	QString posToStrTime(const qint64 &pos);

private:
	QScopedPointer<PlayerWidgetData> d;
};
