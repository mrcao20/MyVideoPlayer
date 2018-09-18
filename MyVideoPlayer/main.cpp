#include "MyVideoPlayer.h"
#include <QtWidgets/QApplication>
#include <QWebEngineSettings>

QString commandLineFilePathArgument() {
	const QStringList args = QCoreApplication::arguments();
	for (const QString &arg : args.mid(1)) {
		if (!arg.startsWith(QLatin1Char('-')))
			return arg;
	}
	return "";
}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication a(argc, argv);

	QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
	QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

	MyVideoPlayer w;
	QString filePath = commandLineFilePathArgument();
	if (!filePath.isEmpty())
		w.playFile(filePath);
	w.show();
	return a.exec();
}
