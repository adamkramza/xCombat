#include "Przeciwnik.h"
using namespace std;

// W liście inicjalizacyjnej dodałem domyślne wartości dla zwykłego przeciwnika - helikoptera na 1 levelu
Przeciwnik::Przeciwnik(QObject* parent): QObject(parent),
    m_x(1960),
    m_y(100 + rand() % 700),
    xPredkosc(5),
    yPredkosc(0),
    m_typ(2),
    m_rozmiar(120),
    licznikWybuchu(0)
{
    connect(&eTime, &QTimer::timeout, this, &Przeciwnik::updatePrzeciwnik);
    eTime.start(16);
}

double Przeciwnik::x()
{
    return m_x;
}
double Przeciwnik::y()
{
    return m_y;
}
int Przeciwnik::rozmiar()
{
    return m_rozmiar;
}

int Przeciwnik::pobierzTyp()
{
    return m_typ;
}

void Przeciwnik::ustawPoziom(int level)
{
    xPredkosc += level;
}

void Przeciwnik::ustawX(double wartosc)
{
    if(m_x != wartosc)
    {
        m_x = wartosc; emit xZmiana();
    }
}

void Przeciwnik::ustawY(double wartosc)
{
    if(m_y != wartosc)
    {
        m_y = wartosc; emit yZmiana();
    }
}

void Przeciwnik::ustawWybuch()
{
    m_typ = 99; emit typZmiana();
}

void Przeciwnik::ustawPauze(bool p)
{
    if(p)
    {
        eTime.stop();
    }
    else
    {
        eTime.start(16);
    }
}

void Przeciwnik::updatePrzeciwnik()
{

    // Jeśli przeciwnik wybucha (typ 99), zatrzymujemy jego ruch
    if (m_typ == 99)
    {
        eTime.stop(); // Wyłączamy timer tego konkretnego wroga, żeby nie generował błędów
        return;
    }
    // Normalny ruch w lewą stronę ekranu (OX)
    ustawX(m_x - xPredkosc);

}