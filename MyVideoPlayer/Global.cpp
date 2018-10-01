#include "Global.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>

Output::Output() : QObject(qApp) {
}

Output * Output::m_instance = 0;

Output * Output::instance()
{
	static QMutex mutex;
	if (!m_instance) {
		QMutexLocker locker(&mutex);
		if (!m_instance)
			m_instance = new Output;
	}

	return m_instance;
}