#pragma once

#include <qstring.h>

class PlayerInterface{

public:
	virtual ~PlayerInterface() = default;

	virtual void play(const QString &path) = 0;
	virtual void stop() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void seekForward() = 0;
	virtual void seekBackward() = 0;
	virtual void setPlayList(const QStringList &videoNames, QStringList &videoInfo) = 0;

};