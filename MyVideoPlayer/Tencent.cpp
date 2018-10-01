#include "Tencent.h"
#include <qregularexpression.h>
#include <QTime>
#include <qjsonvalue.h>
#include <qfile.h>
#include <QtScript/QtScript>
#include <qdebug.h>
#include "NetworkTools.h"

// _t后面跟到现在为止的毫秒
#define CHECK_LOGIN "https://video.qq.com/fcgi-bin/check_login?callback=_jsonp01536894357247&_t=%1"
#define GET_VIDEO "https://vd.l.qq.com/proxyhttp"

struct TencentData{
	NetworkTools m_networkTools;
	QStringList m_videoIds;
	QStringList m_videoNames;
	QString m_refererSuffix;
};

Tencent::Tencent(QObject *parent)
	: QObject(parent)
	, d(new TencentData)
{

}

void Tencent::initApi(const QUrl &url) {
	getVideoInfo(url);
	getRefererSuffix(url);
}

QStringList Tencent::getVideoInfoList() {
	return d->m_videoIds;
}

QStringList Tencent::getVideoNames() {
	return d->m_videoNames;
}

QString Tencent::getVideo(const QString &id) {
	QString videoLink;
	QString vusession = getVusession(id);
	QMap<QByteArray, QByteArray> header;
	header["Host"] = "vd.l.qq.com";
	QString r = d->m_refererSuffix.arg(id);
	QString h = r.left(r.lastIndexOf(QRegularExpression(R"(\?)")));
	header["Referer"] = r.toLatin1();
	header["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36";

	QDateTime dt = QDateTime::currentDateTime();
	QString encryptVer = QString("7.%1").arg(dt.date().dayOfWeek());
	QByteArray pd;
	pd.append(getParam(h, dt.toSecsSinceEpoch(), vusession, vusession, id, encryptVer, getCKey(id)).toLatin1());
	QTextStream cout(stdout);
	QByteArray b = d->m_networkTools.getNetworkData(GET_VIDEO, pd, header);
	QByteArray c = d->m_networkTools.getJsonValue(b, "vinfo").toString().toLatin1();
	videoLink = d->m_networkTools.getJsonValue(c, "vl.vi.ul.ui.url", 4).toString();
	return videoLink;
}

void Tencent::getVideoInfo(const QUrl &url) {
	QMap<QByteArray, QByteArray> header;
	header["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36";
	QString data = d->m_networkTools.getNetworkData(url.toString(), header);
	QRegularExpression re(R"(<div class=".*? _wind="columnname=选集".*?>([\s\S]*?)</div>)");
	QRegularExpressionMatch match = re.match(data);
	QStringList a = match.capturedTexts();	// 得到匹配结果
	if (a.size() < 2) {
		return;
	}
	QString b = a.at(1);	// 得到视频集数的html文档
	getVideoIds(b);
	getVideoNames(b);
}

void Tencent::getVideoIds(const QString &data) {
	QRegularExpression re(R"(id="(.+?)\")");
	auto itr = re.globalMatch(data);
	while (itr.hasNext()) {
		auto match = itr.next();
		auto a = match.capturedTexts();		// 得到匹配结果
		if (a.size() < 2) {
			continue;
		}
		auto b = a.at(1);	// 得到视频id
		d->m_videoIds.append(b);
	}
}

void Tencent::getVideoNames(const QString &data) {
	QRegularExpression re(R"(<a href=".*?">\s*(.*?)\s*</a>)");
	auto itr = re.globalMatch(data);
	while (itr.hasNext()) {
		auto match = itr.next();
		auto a = match.capturedTexts();		// 得到匹配结果
		if (a.size() < 2) {
			continue;
		}
		auto b = a.at(1);	// 得到视频id
		d->m_videoNames.append(b);
	}
}

void Tencent::getRefererSuffix(const QUrl &url) {
	QString a = url.toString();
	int i = a.lastIndexOf(QRegularExpression(R"(/)"));
	QString b = a.left(i);
	d->m_refererSuffix = b + ".html?vid=%1";
}

QString Tencent::getVusession(const QString &id) {
	QString vusession;
	QMap<QByteArray, QByteArray> header;
	header["Host"] = "video.qq.com";
	header["Referer"] = d->m_refererSuffix.arg(id).toLatin1();
	header["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36";
	header["Cookie"] = "appid=wx5ed58254bc0d6b7f; openid=ox8XOvhWqZMNfLmw5oGejBHWyAEQ; vuserid=893145644; refresh_token=13_-2ZXT_8lfmo14VLTkbLwyFstPN47U6F0eb"
		"hZi-m9Ny2oe6VXmGJr-P-2u-Tgq2qG5yQVQUYLuiS3QQmt_Ec4FQ; main_login=wx; _video_qq_next_refresh_time=6600; next_refresh_time=6600; _video_qq_access_toke"
		"n=13_-2ZXT_8lfmo14VLTkbLwyHNerIIewHZc564sSVF_h30nahDtNkaLijt-62K2kUBn1e7xDoGN0kwcI5E-w0u68A; access_token=13_-2ZXT_8lfmo14VLTkbLwyHNerIIewHZc564sSVF"
		"_h30nahDtNkaLijt-62K2kUBn1e7xDoGN0kwcI5E-w0u68A; monitor_count=1; pgv_info=ssid=s3227628691; login_time_last=2018-9-14 11:5:45; _video_qq_vusession="
		"ad7b7930466a680f504addbbd298; vusession=ad7b7930466a680f504addbbd298";
	QString a = d->m_networkTools.getNetworkData(QString(CHECK_LOGIN).arg(QDateTime::currentMSecsSinceEpoch()), header);
	a.remove(QRegularExpression(R"(_jsonp01536894357247\()"));
	a = a.left(a.size() - 1);
	vusession = d->m_networkTools.getJsonValue(a.toLatin1(), "vusession").toString();
	return vusession;
}

QString Tencent::getCKey(const QString &id) {
	QString cKey = "";
	QFile scriptFile(":/MyVideoPlayer/Encrypt.js");
	if (!scriptFile.open(QIODevice::ReadOnly))
	{
		cKey.clear();
		qWarning() << "encodePass.js open failed";
		return cKey;
	}
	QTextStream out(&scriptFile);
	QString contents = out.readAll();
	scriptFile.close();
	
	
	QDateTime dt = QDateTime::currentDateTime();
	long tm = dt.toSecsSinceEpoch();
	QScriptValueList args;      //调用js方法时传入的参数
	args << QScriptValue("10201") << QScriptValue(id) << QScriptValue("v1010") << QScriptValue(1) << QScriptValue(tm) << QScriptValue(QString("7.%1").arg(dt.date().dayOfWeek()));
	QScriptEngine engine;
	QScriptValue js = engine.evaluate(contents);        //个人理解：加载js文本到操作引擎
	QScriptValue func;
	func = engine.globalObject().property("Encrypt");   //调用js方法
	cKey = func.call(QScriptValue(), args).toString();
	return cKey;
}

QString Tencent::getParam(const QString &ehost, qint64 tm, const QString &access_token, const QString &vusession, 
	const QString &id, const QString &encryptVer, const QString &cKey) {

	QString param;
	QString a = R"({"buid":"onlyvinfo","vinfoparam":"charge=1&defaultfmt=auto&otype=ojson&guid=e47e82b5db21a590b047248ff2a628b8&flowid=522ab07f4f5eba900d0f2fe93e653452_10201&platform=10201&sdtfrom=v1010&defnpayver=1&appVer=3.5.55&host=v.qq.com&ehost=)";
	QString b = QUrl::toPercentEncoding(ehost);
	QString c = R"(&refer=v.qq.com&sphttps=1&tm=)";
	QString d = QString::number(tm);
	QString e = R"(&spwm=4&logintoken=%7B%22openid%22%3A%22ox8XOvhWqZMNfLmw5oGejBHWyAEQ%22%2C%22appid%22%3A%22wx5ed58254bc0d6b7f%22%2C%22access_token%22%3A%22)";
	QString f = access_token;
	QString g = R"(%22%2C%22vuserid%22%3A%22893145644%22%2C%22vusession%22%3A%22)";
	QString h = vusession;
	QString i = R"(%22%2C%22main_login%22%3A%22wx%22%7D&unid=6f9b101a4ec811e89d19a0429186d00a&vid=)";
	QString j = id;
	QString k = R"(&defn=fhd&fhdswitch=0&show1080p=1&isHLS=1&dtype=3&sphls=2&spgzip=1&dlver=2&defsrc=2&encryptVer=)";
	QString l = encryptVer;
	QString m = R"(&cKey=)";
	QString n = cKey;
	QString o = R"(&fp2p=1"})";
	param = a + b + c + d + e + f + g + h + i + j + k + l + m + n + o;
	return param;
}

Tencent::~Tencent(){
}
