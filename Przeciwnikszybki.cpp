#include "Przeciwnikszybki.h"
#include <cmath>
using namespace std;

// Inicjalizacja listy parametrów
PrzeciwnikSzybki::PrzeciwnikSzybki(QObject* parent)
    : Przeciwnik(parent)
{
    m_typ = 1;
    m_rozmiar=80;
    // Ustawiamy wartości początkowe specyficzne dla tego typu
    m_x = 2000;
    m_y = 100 + (rand() % 700);
    xPredkosc = 6; // Wyraźnie szybciej


    // WAŻNE: Rozłączamy timer od funkcji rodzica (updatePrzeciwnik z Przeciwnik.cpp)
    // Dzięki temu będzie się wykonywać tylko nowa funkcja z zygzakiem
    disconnect(&eTime, &QTimer::timeout, nullptr, nullptr);

    // Łączymy timer ponownie do naszej nowej funkcji
    connect(&eTime, &QTimer::timeout, this, &PrzeciwnikSzybki::updatePrzeciwnik);

    // Startujemy timer (częstotliwość 60 FPS)
    eTime.start(16);
}

void PrzeciwnikSzybki::updatePrzeciwnik()
{
    // 1. Podstawowy ruch w lewo
    ustawX(m_x - xPredkosc);

    // 2. RUCH ZYGZAKIEM:
    // m_x / 50.0 określa jak "gęsty" jest zygzak
    // * 8.0 określa jak "wysoko" skacze góra-dół
    double przesuniecieY = sin(m_x / 50.0) * 8.0;
    double bazoweY = 300;
    ustawY(bazoweY + przesuniecieY);

    // Jeśli ten przeciwnik akurat dostał i wybucha, zatrzymujemy go
    if (m_typ == 99)
    {
        eTime.stop();
        return;
    }


}