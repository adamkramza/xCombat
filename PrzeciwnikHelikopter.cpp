#include "PrzeciwnikHelikopter.h"
#include <cstdlib>

PrzeciwnikHelikopter::PrzeciwnikHelikopter(QObject* parent) : Przeciwnik(parent)
{
    m_typ = 2;
    m_rozmiar = 120;
    xPredkosc = 5;
    m_y = 100 + (rand() % 700);
}

void PrzeciwnikHelikopter::updatePrzeciwnik()
{
    if (m_typ == 99) { eTime.stop(); return; }
    ustawX(m_x - xPredkosc);
}