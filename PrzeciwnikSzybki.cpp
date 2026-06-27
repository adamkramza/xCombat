#include "PrzeciwnikSzybki.h"
#include <cstdlib>

PrzeciwnikSzybki::PrzeciwnikSzybki(QObject* parent) : Przeciwnik(parent)
{
    m_typ = 0;
    m_rozmiar = 120;
    xPredkosc = 10; // Ten najszybszy lecący prosto
    m_y = 50 + (rand() % 600);
}

void PrzeciwnikSzybki::updatePrzeciwnik()
{
    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99) { eTime.stop(); return; }
    ustawX(m_x - xPredkosc);
}