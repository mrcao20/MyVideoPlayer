#pragma once
#pragma execution_character_set("utf-8")
#include <QObject>
#include <qscopedpointer.h>
#include <qabstractsocket.h>

struct TcpClientData;

class TcpClient : public QObject{
	Q_OBJECT

public:
	explicit TcpClient(QObject *parent = 0);
	~TcpClient();

	bool init();

private slots:
	//¿Í»§¶Ë²Ûº¯Êý
	void readData();
	void readError(QAbstractSocket::SocketError);
	void writeData(const QString &msg);

private:
	void deleteTcpSocket();

private:
	QScopedPointer<TcpClientData> d;
};
