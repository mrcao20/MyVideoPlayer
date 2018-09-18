#pragma once
#pragma execution_character_set("utf-8")
#include "VideoApi.h"
#include <qobject.h>
#include <QString>

class VideoApiFactory{

public:
	VideoApiFactory();

	VideoApi *getVideoApi(const QString &videoSrc, QObject *parent = 0);
};

