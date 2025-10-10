#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <Qt>

auto main(int argc, char** argv) -> int
{
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app, [] { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
	engine.loadFromModule("app.pong", "Main");
	return QGuiApplication::exec();
}