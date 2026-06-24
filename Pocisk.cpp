#include "Pocisk.h"
using namespace std;

Pocisk::Pocisk(QObject* parent):  m_x(200), m_y(1080/2-50), xPredkosc(10), yPredkosc(0)
{

    connect(&bTime, &QTimer::timeout, this, &Pocisk::updatePocisk);
    bTime.start(16);

}

double Pocisk::x()
{
    return m_x;
}

double Pocisk::y()
{
    return m_y;
}

void Pocisk::ustawX(double wartosc)
{
    if(m_x != wartosc)
    {
        m_x = wartosc;
        emit xZmiana();
    }
}

void Pocisk::ustawY(double wartosc)
{
    if(m_y != wartosc)
    {
        m_y = wartosc;
        emit yZmiana();
    }


}

void Pocisk::ustawPredkosc(double p)
{
    xPredkosc = p;
}



void Pocisk::updatePocisk()
{
    ustawX(m_x + xPredkosc); //tu możemy zmienić kierunek lotu pocisku - nasze po x w prawo


}