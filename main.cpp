#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Sterowanie.h>
#include <QObject>
#include <cstdlib>
#include <ctime>
#include <HistoriaWynikow.h>

using namespace std;


//MAIN POZOSTAŁ PRAKTYCZNIE BEZ ZMIAN  - KONTROLA
int main(int argc, char *argv[])
{
    srand(time(nullptr));
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Sterowanie kontrola;
    HistoriaWynikow historia;
    engine.rootContext()->setContextProperty("kontrola", &kontrola);
    engine.rootContext()->setContextProperty("historia", &historia);
    const QUrl url(QStringLiteral("qrc:/xCombat/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    srand(time(nullptr));

    return QCoreApplication::exec();
}
