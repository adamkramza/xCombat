#include "PrzeciwnikUFO.h"
#include <cstdlib>
#include <cmath>

//To jest przeciwnik UFO - na 4 levelu
PrzeciwnikUFO::PrzeciwnikUFO(QObject* parent) : Przeciwnik(parent)
{
    m_typ = 1;
    m_rozmiar = 140;
    xPredkosc = 4;
    m_y = 100 + (rand() % 500);
}

void PrzeciwnikUFO::updatePrzeciwnik()
{
    //Podstawowy ruch w lewo
    ustawX(m_x - xPredkosc);

    // ruch zygzakiem
    // m_x / 40.0 określa jak gęsty"jest zygzak
    // * 10.0 określa jak wysoko skacze góra-dół
    double przesuniecieY = sin(m_x / 40.0) * 10.0; //

    ustawY(m_y + przesuniecieY);

    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }

}