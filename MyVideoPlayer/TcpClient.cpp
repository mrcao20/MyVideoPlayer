#include "TcpClient.h"
#include <qtcpsocket.h>
#include <qmessagebox.h>
#include "Global.h"

struct TcpClientData {
	QTcpSocket *m_tcpClient;
	QString m_ip;
	quint16 m_port;
};

TcpClient::TcpClient(QObject *parent)
	: QObject(parent)
	, d(new TcpClientData)
{

	d->m_ip = "127.0.0.1";
	d->m_port = 8000;

	connect(Output::instance(), &Output::message, this, &TcpClient::writeData);
}

bool TcpClient::init() {
	//初始化TCP客户端
	d->m_tcpClient = new QTcpSocket(this);   //实例化tcpClient
	d->m_tcpClient->abort();                 //取消原有连接
	d->m_tcpClient->connectToHost(d->m_ip, d->m_port);
	if (!d->m_tcpClient->waitForConnected(1000)) {  // 连接成功则进入if{}
		deleteTcpSocket();
		return false;
	}
	writeData("MyVideoPlayer Connected\n");
	connect(d->m_tcpClient, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(d->m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), \
		this, SLOT(readError(QAbstractSocket::SocketError)));
	return true;
}

void TcpClient::readData(){
	QByteArray buffer = d->m_tcpClient->readAll();
	
}

void TcpClient::readError(QAbstractSocket::SocketError){
	d->m_tcpClient->disconnectFromHost();
	//QMessageBox::warning(NULL, "warning", tr("failed to connect server because %1").arg(d->m_tcpClient->errorString()));
	deleteTcpSocket();
}

void TcpClient::writeData(const QString &msg) {
	if (!d->m_tcpClient) {
		if (!init())
			return;
	}
	d->m_tcpClient->write(msg.toUtf8().toBase64()); //qt5除去了.toAscii()
	d->m_tcpClient->flush();
}

void TcpClient::deleteTcpSocket() {
	d->m_tcpClient->disconnect();
	d->m_tcpClient->deleteLater();
	d->m_tcpClient = NULL;
}

TcpClient::~TcpClient(){
	if (d->m_tcpClient) {
		writeData("MyVideoPlayer Disconnected");
		d->m_tcpClient->disconnectFromHost();
	}
}
