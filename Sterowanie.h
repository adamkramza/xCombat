#ifndef STEROWANIE_H
#define STEROWANIE_H
#include <QTimer>
#include <Pocisk.h>
#include <Przeciwnik.h>
#include <QObject>
#include <QDebug>
#include <qqmllist.h>
#include <QQmlListProperty>
using namespace std;

//Cale sterowanie można w sumie rozbić na mniejsze klasy, zamiast dużego kombajnu np. sterowanie, kolizje, spawn przeciwników - mechanika


class Sterowanie:public QObject
{
    Q_OBJECT
    Q_PROPERTY(double x READ x WRITE ustawX NOTIFY xZmiana)
    Q_PROPERTY(double y READ y WRITE ustawY NOTIFY yZmiana)
    Q_PROPERTY(QQmlListProperty<Pocisk> pociski READ pobierzPociski NOTIFY bulletChanged)
    Q_PROPERTY(QQmlListProperty<Przeciwnik> przeciwnicy READ pobierzPrzeciwnicy NOTIFY przeciwnikChanged)
    Q_PROPERTY(double punkty READ punkty WRITE ustawPunkty NOTIFY zmianaPunkty)
    Q_PROPERTY(int zycie READ zycie WRITE ustawZycie NOTIFY zycieZmiana)
    Q_PROPERTY(int level READ level NOTIFY levelZmiana)
    Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverZmiana)


 private:
     double m_x; //aktualna pozycja gracza na współrzędnej x
     double m_y; //aktualna pozycja gracza na współrzędnej y
     double xPredkosc; //prędkość poruszania - przeskok pixeli
     double minX;//zmienne ograniczające zakres poruszania się gracza min/max
     double maxX;
     double minY;
     double maxY;
     double m_punkty=0; //ilość punktów
     int m_zycie = 100; //ilość pancerza/życia
     int m_level = 1; //level - pozim gry
     bool m_gameOver=false;
     bool m_pauza = false;
     bool m_gameStarted = false;
     QTimer time;
     QTimer sPrzeciwnik;
     QList<Pocisk*> pociskList;
     QList<Przeciwnik*> przeciwnikList;
    int czasSpawnu = 1500;

public:
    Sterowanie(QObject* parent=nullptr);
    double x();
    double y();
    int zycie();
    int level();
    double punkty();

    void ustawX(double wartosc);
    void ustawY(double wartosc);
    void ustawPunkty(double wartosc);
    void ustawZycie(int wartosc);
    bool gameOver() const;

    //funkcje poruszanie się gracza,
    Q_INVOKABLE void wLewo();
    Q_INVOKABLE void wPrawo();
    Q_INVOKABLE void wGore();
    Q_INVOKABLE void wDol();


    //funkcje broni - strzelania
    Q_INVOKABLE void strzalRakieta();

    //funkcje przeciwnnikow
    Q_INVOKABLE void stworzPrzeciwnika();
    Q_INVOKABLE void ustawPauze(bool p);

    //funkcje punktacja
    Q_INVOKABLE QString pokazPunkty();
    Q_INVOKABLE void resetujGre();

    QQmlListProperty<Pocisk> pobierzPociski();
    QQmlListProperty<Przeciwnik> pobierzPrzeciwnicy();



public slots:
    void aktualizujStan();
    void usunPocisk(Pocisk* pocisk);
    void usunPrzeciwnika(Przeciwnik* przeciwnik);
    void sprawdzKolizje();
    void startGame();

signals:
    void xZmiana();
    void yZmiana();
    void bulletChanged();
    void przeciwnikChanged();
    void zmianaPunkty();
    void zycieZmiana();
    void levelZmiana();
    void gameOverZmiana();

};

#endif
