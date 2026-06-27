#include "Przeciwnik.h"
using namespace std;

// W liście inicjalizacyjnej dodałem domyślne wartości dla zwykłego przeciwnika - helikoptera na 1 levelu
Przeciwnik::Przeciwnik(QObject* parent): QObject(parent),
    m_x(1960), // wspólny punkt startu dla wszystkich
    yPredkosc(0),
    licznikWybuchu(0)
{
    // Timer podpinamy tylko raz tutaj. Dzieci już nie muszą tego robić
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