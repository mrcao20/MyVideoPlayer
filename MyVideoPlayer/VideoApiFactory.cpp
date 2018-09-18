#include "VideoApiFactory.h"
#include "Tencent.h"

VideoApiFactory::VideoApiFactory(){
}

VideoApi *VideoApiFactory::getVideoApi(const QString &videoSrc, QObject *parent) {
	VideoApi *videoApi = NULL;
	if (videoSrc == "��Ѷ��Ƶ") {
		Tencent *tencent = new Tencent(parent);

		videoApi = tencent;
	}
	return videoApi;
}
