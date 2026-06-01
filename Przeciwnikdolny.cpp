#include "Przeciwnikdolny.h"
#include <QDebug>
using namespace std;

PrzeciwnikDolny::PrzeciwnikDolny(QObject* parent) : Przeciwnik(parent)
{

    m_typ = 3; // typ obiektu - dolny
    m_rozmiar=120; // romiar obiektu - dolny
    zatrzymany=false;
    licznikStop=0;
    m_x = 2000; //pozycja obiektu OX

    // DÓŁ EKRANU
    m_y = 200+rand()%700; //pozycja obiektu OY

    //Prędkość poruszania przeciwnika
    xPredkosc = 4;

    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);

    connect(&eTime,
            &QTimer::timeout,
            this,
            &PrzeciwnikDolny::updatePrzeciwnik);

    eTime.start(16);
}

void PrzeciwnikDolny::updatePrzeciwnik()
{
    // JEŚLI STOI
    if(zatrzymany)
    {
        licznikStop++;

        // 120 klatek to ok. 2 sekundy
        if(licznikStop > 120)
        {
            zatrzymany = false;
            licznikStop = 0;
        }
    }
    else
    {
        // 2. RUCH ZYGZAKIEM:
        // m_x / 50.0 określa jak "gęsty" jest zygzak
        // * 8.0 określa jak "wysoko" skacze góra-dół
        double przesuniecieY = sin(m_x / 50.0) * 150.0;
        double bazoweY = 300;
        ustawY(bazoweY + przesuniecieY);


    }

    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }



}