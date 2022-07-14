#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "NetworkRequest.h"
#include "Networking.h"
#include "EnumInfo.h"
#include "ReadyState.h"
#include "Settings.h"
#include "SettingsFormat.h"

template <typename T>
QObject* singletonProvider(QQmlEngine*, QJSEngine*)
{
    return new T();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<EnumInfo>("ArcGIS.AppFramework.Testing", 1, 0, "EnumInfo", singletonProvider<EnumInfo>);
    qmlRegisterSingletonType<ReadyStateEnum>("ArcGIS.AppFramework.Testing", 1, 0, "ReadyState", singletonProvider<ReadyStateEnum>);
    qmlRegisterSingletonType<NetworkErrorEnum>("ArcGIS.AppFramework.Testing", 1, 0, "NetworkError", singletonProvider<NetworkErrorEnum>);
    qmlRegisterSingletonType<SettingsFormatEnum>("ArcGIS.AppFramework.Testing", 1, 0, "SettingsFormat", singletonProvider<SettingsFormatEnum>);

    qmlRegisterType<NetworkRequest>("ArcGIS.AppFramework.Testing", 1, 0, "NetworkRequest");
    qmlRegisterType<Networking>("ArcGIS.AppFramework.Testing", 1, 0, "Networking");
    qmlRegisterType<Settings>("ArcGIS.AppFramework.Testing", 1, 0, "Settings");

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
