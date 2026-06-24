#include "Przeciwnikdolny.h"
#include <QDebug>
using namespace std;

//To jest przeciwnik UFO - na 4 levelu
PrzeciwnikDolny::PrzeciwnikDolny(QObject* parent) : Przeciwnik(parent)
{

    m_typ = 1;
    m_rozmiar=140;
    m_x = 1940;
    m_y = 100 + (rand() % 500);
    xPredkosc = 4;
    // rozłączamy i łączymy timer pod naszą klasę
    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);
    connect(&eTime, &QTimer::timeout, this, &PrzeciwnikDolny::updatePrzeciwnik);

    eTime.start(16);


}

void PrzeciwnikDolny::updatePrzeciwnik()
{
    //Podstawowy ruch w lewo
    ustawX(m_x - xPredkosc);

    // ruch zygzakiem
    // m_x / 40.0 określa jak gęsty"jest zygzak
    // * 10.0 określa jak wysoko skacze góra-dół
    double przesuniecieY = sin(m_x / 40.0) * 10.0;
    double bazoweY = m_y;
    ustawY(bazoweY + przesuniecieY);




    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }



}