#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Sterowanie.h>
#include <QObject>
#include <ctime>

using namespace std;


//MAIN POZOSTAŁ PRAKTYCZNIE BEZ ZMIAN  - KONTROLA
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Sterowanie kontrola;
    engine.rootContext()->setContextProperty("kontrola", &kontrola);

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
