#pragma once
#pragma execution_character_set("utf-8")

#include <QtCore/QObject>

#define output (*(Output::instance()))
#define OUTPUT_ENUM(Class,Enum) \
inline Output &operator<<(Output &o, Class::Enum value) \
{ \
    int index = Class::staticMetaObject.indexOfEnumerator(#Enum); \
	o << QString("%1::%2").arg(#Class, Class::staticMetaObject.enumerator(index).valueToKey(value));	\
    return o; \
}

class Output : public QObject {
	Q_OBJECT

public:
	static Output *instance();
	inline Output &operator<< (const QString& msg) { emit message(msg); return *this; }
	inline Output &operator<< (const char * msg) { emit message(QString::fromLocal8Bit(msg)); return *this; }
	inline Output &operator<< (const QChar * msg) { emit message(QString(msg)); return *this; }
	inline Output &operator<< (const QByteArray &msg) { emit message(msg); return *this; }
	inline Output &operator<< (const int &msg) { emit message(QString::number(msg)); return *this; }
	inline Output &operator<< (const qint64 &msg) { emit message(QString::number(msg)); return *this; }

signals:
	void message(const QString &msg);

private:
	Output();
	static Output * m_instance;
};