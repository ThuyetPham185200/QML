#include <QGuiApplication>
#include <QQmlApplicationEngine>
//#include "LoadFile.h"
#include <LoadFile.h>
#include "UDPConnector.h"
#include "receiver.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    // embedded class into qml
    qmlRegisterType<LoadFile>("thuyet.loadfile.com", 1, 0,"LoadFile");
    qmlRegisterType<UDPConnector>("thuyet.connector.com", 1, 0,"UDPConnector");
    qmlRegisterType<Receiver>("thuyet.receiver.com", 1, 0,"Receiver");


    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
