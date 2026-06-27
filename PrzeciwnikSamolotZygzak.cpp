#include "PrzeciwnikSamolotZygzak.h"
#include <cstdlib>
#include <cmath>

PrzeciwnikSamolotZygzak::PrzeciwnikSamolotZygzak(QObject* parent) : Przeciwnik(parent)
{
    m_typ = 3;
    // Ustawiamy wartości początkowe specyficzne dla tego typu
    m_rozmiar = 140;
    xPredkosc = 8;
    m_y = 50 + (rand() % 700);
}

void PrzeciwnikSamolotZygzak::updatePrzeciwnik()
{


    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }
    ustawX(m_x - xPredkosc);

    // 2. RUCH ZYGZAKIEM:
    // m_x / 50.0 określa jak "gęsty" jest zygzak
    // * 5.0 określa jak "wysoko" skacze góra-dół
    double przesuniecieY = sin(m_x / 50.0) * 5.0;
    ustawY(m_y + przesuniecieY);
}