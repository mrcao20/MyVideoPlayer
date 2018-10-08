#pragma once
#pragma execution_character_set("utf-8")
#include <qobject.h>
#include "VideoApi.h"
#include <qscopedpointer.h>

struct TencentData;

class Tencent : public QObject, public VideoApi{
	Q_OBJECT

public:
	explicit Tencent(QObject *parent = 0);
	~Tencent() override;

	void initApi(const QUrl &url) override;
	QStringList getVideoInfoList() override;
	QStringList getVideoNames() override;
	QString getVideo(const QString &id) override;

private:
	void getVideoInfo(const QUrl &url);
	void getVideoIds(const QString &data);
	void getVideoNames(const QString &data);
	void getRefererSuffix(const QUrl &url);
	QString getVusession(const QString &id);
	QString getCKey(const QString &id);
	QString getVideoLink(const QByteArray postData, const QMap<QByteArray, QByteArray> header);
	QString getParam(const QString &ehost, qint64 tm, const QString &access_token, const QString &vusession, 
		const QString &id, const QString &encryptVer, const QString &cKey, const QString &defn);

private:
	QScopedPointer<TencentData> d;
};

