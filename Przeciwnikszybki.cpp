#include "Przeciwnikszybki.h"
#include <cmath>
using namespace std;

// Inicjalizacja listy parametrów - dodałem przeciwnika na level 3 - samolot 2
PrzeciwnikSzybki::PrzeciwnikSzybki(QObject* parent)
    : Przeciwnik(parent)
{
    m_typ = 3;
    m_rozmiar=140;
    // Ustawiamy wartości początkowe specyficzne dla tego typu
    m_x = 1960;
    m_y = 50 + (rand() % 700);
    xPredkosc = 8;


    // Rozłączamy timer od funkcji rodzica (updatePrzeciwnik z Przeciwnik.cpp)
    // Dzięki temu będzie się wykonywać tylko nowa funkcja z zygzakiem
    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);

    // Łączymy timer ponownie do naszej nowej funkcji
    connect(&eTime, &QTimer::timeout, this, &PrzeciwnikSzybki::updatePrzeciwnik);

    // Startujemy timer (częstotliwość 60 FPS)
    eTime.start(16);
}




void PrzeciwnikSzybki::updatePrzeciwnik()
{

    // Podstawowy ruch w lewo
    ustawX(m_x - xPredkosc);

    // 2. RUCH ZYGZAKIEM:
    // m_x / 50.0 określa jak "gęsty" jest zygzak
    // * 8.0 określa jak "wysoko" skacze góra-dół
    double przesuniecieY = sin(m_x / 50.0) * 5.0;
    double bazoweY = m_y;
    ustawY(bazoweY + przesuniecieY);

    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }


}