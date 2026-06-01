#include "Sterowanie.h"
#include "Przeciwnik.h"
#include "Przeciwnikszybki.h"
#include "Przeciwnikduzy.h"
#include "Przeciwnikdolny.h"
#include <ctime>
using namespace std;



Sterowanie::Sterowanie(QObject* parent)
    : m_x(200), m_y(490), xPredkosc(10), minX(100), maxX(1800), minY(50), maxY(850)
{
    connect(&time, &QTimer::timeout, this, &Sterowanie::aktualizujStan);
    connect(&sPrzeciwnik, &QTimer::timeout, this, &Sterowanie::stworzPrzeciwnika);
    time.start(16);
    sPrzeciwnik.start(1000 + rand() % 2000);

}

double Sterowanie::x()
{
    return m_x;
}

double Sterowanie::y()
{
    return m_y;
}



void Sterowanie::ustawX(double wartosc)
{
    if (m_x != wartosc)
    {
        m_x = wartosc; emit xZmiana();
    }
}

void Sterowanie::ustawY(double wartosc)
{
    if (m_y != wartosc)
    {
        m_y = wartosc; emit yZmiana();
    }
}

void Sterowanie::wLewo()
{
    ustawX(m_x - xPredkosc < minX ? minX : m_x - xPredkosc);
}

void Sterowanie::wPrawo()
{
    ustawX(m_x + xPredkosc > maxX ? maxX : m_x + xPredkosc);
}

void Sterowanie::wGore()
{
    ustawY(m_y - xPredkosc < minY ? minY : m_y - xPredkosc);
}

void Sterowanie::wDol()
{
    ustawY(m_y + xPredkosc > maxY ? maxY : m_y + xPredkosc);
}

void Sterowanie::strzalRakieta()
{

    Pocisk* newPocisk = new Pocisk(this);
    newPocisk->ustawX(m_x + 50);
    newPocisk->ustawY(m_y + 20);
    newPocisk->ustawPredkosc(xPredkosc + 15);
    pociskList.append(newPocisk);
    emit bulletChanged();
}



void Sterowanie::stworzPrzeciwnika()
{
    Przeciwnik* nP = nullptr;

    // Przypisanie stworzu, domyślnie 1, jeśli m_level jest nieustawione
    int aktualnyLevel = (m_level > 0) ? m_level : 1;

    //Poziom 1:
    if (aktualnyLevel == 1)
    {
        sPrzeciwnik.start(2000);//predkosc spawny przeciwnika
        nP = new Przeciwnik(this);
    }

    //POziom 2:
    else if (aktualnyLevel == 2)
    {
        sPrzeciwnik.start(1000);//predkosc spawny przeciwnika
        if (rand() % 2 == 0)
        {
            nP = new Przeciwnik(this);
        } else {

            nP = new PrzeciwnikDuzy(this);
        }
    }
    // Poziom 3
    else if(aktualnyLevel == 3)
    {
        int los = rand() % 3;

        switch(los)
        {
        case 0:
            nP = new Przeciwnik(this);
            break;

        case 1:
            nP = new PrzeciwnikDuzy(this);
            break;

        case 2:
            nP = new PrzeciwnikSzybki(this);
            break;
        }
    }


    // Poziom 4
    else if(aktualnyLevel==4)
    {
        sPrzeciwnik.start(500);
        int los = rand() % 4;
        switch (los)
        {
        case 0:
            nP = new Przeciwnik(this);
            break;
        case 1:
            nP = new PrzeciwnikSzybki(this);
            break;
        case 2:
            nP = new PrzeciwnikDuzy(this);
            break;
        case 3:
            nP = new PrzeciwnikDolny(this);
            break;
        default:
            nP = new Przeciwnik(this);
            break;
        }
    }

    // Krytyczny bezpiecznik: Jeśli wskaźnik z jakiegoś powodu jest pusty,
    // natychmiast przerywamy funkcję, żeby nie dotykać pamięci nullptr!
    if (nP == nullptr)
    {
        // sPrzeciwnik.start(1000 + rand() % 2000); // Restartujemy timer, żeby gra nie utknęła
        return;
    }

    // Ustawianie właściwości obiektu
    nP->ustawPoziom(aktualnyLevel);
    nP->ustawX(2000);
    nP->ustawY(100 + rand() % 700);

    //Dodanie do listy i powiadomienie QML
    przeciwnikList.append(nP);

    //Losowanie czasu do następnego spawnu (od 1.2s do 3.2s)
    // sPrzeciwnik.start(1200 + rand() % 2000);

    emit przeciwnikChanged();
}

void Sterowanie::aktualizujStan()
{


    // 1. Usuwanie pocisków lecących poza ekran
    for (int i = pociskList.size() - 1; i >= 0; i--) {
        Pocisk* p = pociskList[i];
        if (p->x() > 2000 || p->x() < -100 || p->y() > 1100)
        {
            pociskList.removeAt(i);
            p->deleteLater();
            emit bulletChanged();
        }
    }

    // 2. Obsługa przeciwników (ucieczka poza ekran LUB odliczanie wybuchu)
    for (int i = przeciwnikList.size() - 1; i >= 0; i--)
    {
        Przeciwnik* pr = przeciwnikList[i];

        // Jeśli uciekł żywy za lewą krawędź - usuwanie
        if (pr->x() < -100 && pr->pobierzTyp() != 99)
        {
            przeciwnikList.removeAt(i);
            pr->deleteLater();
            emit przeciwnikChanged();
            continue;
        }

        // Jeśli przeciwnik jest w stanie wybuchu
        if (pr->licznikWybuchu > 0)
        {
            pr->licznikWybuchu++;

            if (pr->licznikWybuchu > 15)
            {
                przeciwnikList.removeAt(i);
                pr->deleteLater();
                emit przeciwnikChanged();
                continue; // przechodzi od razu do następnego obrotu pętli
            }
        }
    }

    sprawdzKolizje();
}

void Sterowanie::usunPocisk(Pocisk* pocisk)
{
    if (pociskList.removeOne(pocisk)) {
        pocisk->deleteLater();
        emit bulletChanged();
    }
}

void Sterowanie::usunPrzeciwnika(Przeciwnik* przeciwnik)
{
    if (przeciwnikList.removeOne(przeciwnik)) {
        przeciwnik->deleteLater();
        emit przeciwnikChanged();
    }
}

void Sterowanie::sprawdzKolizje()
{
    // PĘTLA 1: POCISKI I PRZECIWNICY
    for (int i = pociskList.size() - 1; i >= 0; i--)
    {
        Pocisk* pocisk = pociskList[i];

        for (int j = przeciwnikList.size() - 1; j >= 0; j--)
        {
            Przeciwnik* pr = przeciwnikList[j];

            if (pr->pobierzTyp() == 99) continue;

            double pociskSzerokosc =  20.0;
            double pociskWysokosc = 10.0;
            double wrogRozmiar = pr->rozmiar();

            if (pocisk->x() < pr->x() + wrogRozmiar &&
                pocisk->x() + pociskSzerokosc > pr->x() &&
                pocisk->y() < pr->y() + wrogRozmiar &&
                pocisk->y() + pociskWysokosc > pr->y())
            {
                int nagroda = 10;
                if (pr->pobierzTyp() == 1) nagroda = 50;
                if (pr->pobierzTyp() == 2) nagroda = 100;
                 if (pr->pobierzTyp() == 3) nagroda = 200;
                ustawPunkty(m_punkty + nagroda);

                usunPocisk(pocisk);
                pr->ustawWybuch();
                pr->licznikWybuchu = 1;
                pocisk = nullptr;
                break;
            }
        }
    }

    // PĘTLA 2: GRACZ I PRZECIWNICY
    for (int i = przeciwnikList.size() - 1; i >= 0; i--)
    {
        Przeciwnik* pr = przeciwnikList[i];
        if (pr->pobierzTyp() == 99) continue;

        double wrogRozmiar = pr->rozmiar();

        if (m_x < pr->x() + wrogRozmiar &&
            m_x + 80 > pr->x() &&
            m_y < pr->y() + wrogRozmiar &&
            m_y + 80 > pr->y())
        {
            ustawZycie(m_zycie - 25);

            pr->ustawWybuch();
            pr->licznikWybuchu = 1;

            if (m_zycie <= 0) {
                time.stop();
                sPrzeciwnik.stop();
                m_gameOver = true;
                emit gameOverZmiana();
                return;
            }
        }
    }

    int nowyPoziom = 1;

    if(m_punkty >= 1000)
    {
        nowyPoziom = 4;
    }
    else if(m_punkty >= 500)
    {
        nowyPoziom = 3;
    }
    else if(m_punkty >= 200)
    {
        nowyPoziom = 2;
    }
    else
    {
        nowyPoziom = 1;
    }

    if(nowyPoziom > m_level)
    {
        m_level = nowyPoziom;
        emit levelZmiana();
    }
}

QString Sterowanie::pokazPunkty()
{
    return QString::number(m_punkty);
}


void Sterowanie::resetujGre()
{
    qDeleteAll(pociskList);
    pociskList.clear();
    emit bulletChanged();

    qDeleteAll(przeciwnikList);
    przeciwnikList.clear();
    emit przeciwnikChanged();

    m_punkty = 0;
    emit zmianaPunkty();
    m_x = 200;
    emit xZmiana();
    m_y = 490;
    emit yZmiana();
    m_level = 1;
    emit levelZmiana();
    m_zycie = 100;
    emit zycieZmiana();
    m_gameOver = false;
    emit gameOverZmiana();
    time.start(16);
    sPrzeciwnik.start(1000 + rand()%2000);

}


int Sterowanie::level()
{
    return m_level;
}


double Sterowanie::punkty()
{
    return m_punkty;
}


void Sterowanie::ustawPunkty(double wartosc)
{
    if(m_punkty != wartosc)
    {
        m_punkty = wartosc;
        emit zmianaPunkty();

    }
}


int Sterowanie::zycie()
{
    return m_zycie;
}

void Sterowanie::ustawZycie(int wartosc)
{
    if(m_zycie != wartosc)
    {
        m_zycie = wartosc;
        emit zycieZmiana();
    }
}

bool Sterowanie::gameOver() const
{
    return m_gameOver;
}


QQmlListProperty<Pocisk> Sterowanie::pobierzPociski()
{

    return QQmlListProperty<Pocisk>(this, &pociskList);

}


QQmlListProperty<Przeciwnik> Sterowanie::pobierzPrzeciwnicy()
{

    return QQmlListProperty<Przeciwnik>(this, &przeciwnikList);

}