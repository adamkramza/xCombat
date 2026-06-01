#include "Przeciwnikduzy.h"
using namespace std;

PrzeciwnikDuzy::PrzeciwnikDuzy(QObject* parent)
    : Przeciwnik(parent)
{
    m_typ = 2;
    m_rozmiar=140;
    m_x = 2000;
    m_y = 50 + (rand() % 600); // Większy margines, bo jest wysoki
    xPredkosc = 2; // Wolny, ale groźny....... ;)


    // Standardowo rozłączamy i łączymy timer pod naszą klasę
    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);
    connect(&eTime, &QTimer::timeout, this, &PrzeciwnikDuzy::updatePrzeciwnik);

    eTime.start(16);
}

void PrzeciwnikDuzy::updatePrzeciwnik()
{
    // Prosty ruch w lewo, ale powolny
    ustawX(m_x - xPredkosc);


    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go!
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }

}