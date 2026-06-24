#include "Przeciwnikduzy.h"
using namespace std;

//To jest przeciwnik zwykły samolot 1 - na levelu 2
PrzeciwnikDuzy::PrzeciwnikDuzy(QObject* parent)
    : Przeciwnik(parent)
{
    m_typ = 0;
    m_rozmiar=120;
    m_x = 1960;
    m_y = 50 + (rand() % 600); // Większy margines, bo jest wysoki
    xPredkosc = 10; // Wolny, ale groźny....... ;)

    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);
    connect(&eTime, &QTimer::timeout, this, &PrzeciwnikDuzy::updatePrzeciwnik);

    eTime.start(16);
}

void PrzeciwnikDuzy::updatePrzeciwnik()
{

    ustawX(m_x - xPredkosc);


    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }

}