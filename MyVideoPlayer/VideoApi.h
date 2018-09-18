#pragma once

#include <qurl.h>

class VideoApi{

public:
	virtual ~VideoApi() = default;

	virtual void initApi(const QUrl &url) = 0;
	virtual QStringList getVideoInfoList() = 0;
	virtual QString getVideo(const QString &id) = 0;

};