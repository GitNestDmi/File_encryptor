#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "filemode.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setOrganizationName("somename");   //нужно для FileDialog
    app.setOrganizationDomain("somename");

    IFileMode* fMode = new FileMode();
    qmlRegisterType<NameFM>("NameFM", 1, 0, "NameFM");

    //qmlRegisterUncreatableMetaObject(NameFM::staticMetaObject,
     //           "my.namespace", 1, 0, "MyNamespace","Error");

    auto root_context = engine.rootContext();
    root_context->setContextProperty("fMode", fMode);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },Qt::QueuedConnection);

    engine.load(url);

    int r = app.exec();
    qDebug() << "app.exec() = " << r;
    return r;
}
