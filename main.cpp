#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "subdirectorymodel.h"
#include "stringfilterproxymodel.h"
#include <QApplication>
#include <QDialog>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    qmlRegisterType<SubDirectoryModel>("SubDirectoryModel", 1, 0, "SubDirectoryModel");
    qmlRegisterType<StringFilterProxyModel>("SortFilterProxyModel", 1, 0, "StringFilterProxyModel");
    qmlRegisterUncreatableType<SubDirectory>("SubDirectory", 1, 0, "SubDirectory",
        QStringLiteral("SubDirectory should not be created in QML"));

    QQmlApplicationEngine engine;
    SubDirectory dir(argv[0]);
    engine.rootContext()->setContextProperty(QStringLiteral("SubDirectory"), &dir);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
