#pragma once

#include <QObject>
#include <qmap.h>
#include <qscopedpointer.h>

QT_BEGIN_NAMESPACE
class QNetworkReply;
QT_END_NAMESPACE

struct NetworkToolsData;

class NetworkTools : public QObject{
	Q_OBJECT

public:
	explicit NetworkTools(QObject *parent = 0);
	~NetworkTools();

	QByteArray getNetworkData(const QString &, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QByteArray getNetworkData(const QString &, const QByteArray &postData, const QMap<QByteArray, QByteArray> & = QMap<QByteArray, QByteArray>());
	QJsonValue getJsonValue(const QByteArray &json, const QString &nodeName);
	QJsonValue getJsonValue(const QByteArray &json, const QString &nodeName, int subscript);
	QJsonValue getJsonValue(const QJsonObject &root, const QString &nodeName);
	QJsonValue getJsonValue(const QJsonObject &root, const QString &nodeName, int subscript);

private:
	//根据所指定的nodelist返回第一个满足条件的nodeValue
	void getValue(QJsonObject &root, QJsonValue &nodeValue, QStringList &nodeList, int subscript);
	QByteArray getNetworkData(QNetworkReply *reply);

private:
	QScopedPointer<NetworkToolsData> d;

};
