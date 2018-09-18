#include "NetworkTools.h"
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qjsonvalue.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qeventloop.h>
#include <qregularexpression.h>

struct NetworkToolsData {
	QNetworkAccessManager m_networkAccessManager;
};

NetworkTools::NetworkTools(QObject *parent)
	: QObject(parent)
	, d(new NetworkToolsData)
{

}

/*
根据指定的nodeName从QByteArray中找出第一个满足条件的QJsonValue
*/
QJsonValue NetworkTools::getJsonValue(const QByteArray &json, const QString &nodeName) {
	return getJsonValue(json, nodeName, 1);
}

QJsonValue NetworkTools::getJsonValue(const QByteArray &json, const QString &nodeName, int subscript) {
	QJsonValue nodeValue;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(json);
	if (jsonDocument.isEmpty()) {
		printf("audio link error, json value not found\n");
		fflush(stdout);
		return nodeValue;
	}
	QJsonObject root = jsonDocument.object();
	nodeValue = getJsonValue(root, nodeName, subscript);
	return nodeValue;
}

QJsonValue NetworkTools::getJsonValue(const QJsonObject &root, const QString &nodeName) {
	return getJsonValue(root, nodeName, 1);
}

/*
根据nodeName从QJsonObject中找出第一个满足条件的QJsonValue
*/
QJsonValue NetworkTools::getJsonValue(const QJsonObject &root, const QString &nodeName, int subscript) {
	QJsonValue nodeValue;
	QStringList nodeList = nodeName.split(QRegularExpression("\\."), QString::SkipEmptyParts);
	QJsonObject jsonObject = root;
	getValue(jsonObject, nodeValue, nodeList, subscript);
	return nodeValue;
}

/*
	每次进入该函数，拷贝一份nodeList，每次循环提出第一个node查找，如果不存在则立即返回
	如果存在则根据该node对应对象继续查找，如果查找到最后一个节点，根据需要查找的下标，
	返回同名的最后一个节点的相应下标位置的对象,如果改下标位置对象不存在，则依次返回前一个
*/
void NetworkTools::getValue(QJsonObject &root, QJsonValue &nodeValue, QStringList &nodeList, int subscript) {
	QStringList list = nodeList;
	int curSubscript = 1;
	for (int i = 0; i < nodeList.size(); i++) {
		nodeValue = root.value(list.takeFirst());   // 如果不存在该node，则nodeValue的type为Undefined
		if (nodeValue.type() != QJsonValue::Object && nodeValue.type() != QJsonValue::Array)
			break;
		if (i == nodeList.size() - 1)
			break;
		if (nodeValue.type() == QJsonValue::Object) {
			root = nodeValue.toObject();
			continue;
		}
		QJsonArray array = nodeValue.toArray();
		for (auto itr = array.begin(); itr != array.end(); itr++) {
			root = (*itr).toObject();
			getValue(root, nodeValue, list, subscript);
			if (!nodeValue.isUndefined() && !nodeValue.isNull()) {
				if (curSubscript == subscript || list.size() > 1) {
					return;
				}
				else {
					curSubscript++;
					continue;
				}
			}
		}
	}
}

QByteArray NetworkTools::getNetworkData(const QString &url, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkRequest request;
	request.setUrl(url);
	for (auto itr = headers.cbegin(); itr != headers.cend(); itr++) {
		request.setRawHeader(itr.key(), itr.value());
	}
	QNetworkReply *reply = d->m_networkAccessManager.get(request);
	QByteArray data = getNetworkData(reply);
	return data;
}

QByteArray NetworkTools::getNetworkData(const QString &url, const QByteArray &postData, const QMap<QByteArray, QByteArray> &headers) {
	QNetworkRequest request;
	request.setUrl(url);
	for (auto itr = headers.cbegin(); itr != headers.cend(); itr++) {
		request.setRawHeader(itr.key(), itr.value());
	}
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());
	QNetworkReply *reply = d->m_networkAccessManager.post(request, postData);
	QByteArray data = getNetworkData(reply);
	return data;
}

QByteArray NetworkTools::getNetworkData(QNetworkReply *reply) {
	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();
	QByteArray data = reply->readAll();
	reply->close();
	delete reply;
	return data;
}

NetworkTools::~NetworkTools(){
}
