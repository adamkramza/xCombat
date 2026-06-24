#include "Sterowanie.h"
#include "Przeciwnik.h"
#include "Przeciwnikszybki.h"
#include "Przeciwnikduzy.h"
#include "Przeciwnikdolny.h"
#include <ctime>

using namespace std;

//Cale to można w sumie rozbić na mniejsze klasy np. sterowanie, kolizje, spawn przeciwników

Sterowanie::Sterowanie(QObject* parent)
    : m_x(200), m_y(490), xPredkosc(10), minX(100), maxX(1800), minY(50), maxY(850)
{

    connect(&time, &QTimer::timeout, this, &Sterowanie::aktualizujStan);
    connect(&sPrzeciwnik, &QTimer::timeout, this, &Sterowanie::stworzPrzeciwnika);
    time.start(16);
    // sPrzeciwnik.start(1000 + rand() % 2000);
    // sPrzeciwnik.start(1000);
    sPrzeciwnik.start(1200 + rand() % 1000);
    m_gameStarted = false;
    m_pauza = true;

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
    if(m_pauza) return;
    ustawX(m_x - xPredkosc < minX ? minX : m_x - xPredkosc);
}

void Sterowanie::wPrawo()
{
    if(m_pauza) return;
    ustawX(m_x + xPredkosc > maxX ? maxX : m_x + xPredkosc);
}

void Sterowanie::wGore()
{
    if(m_pauza) return;
    ustawY(m_y - xPredkosc < minY ? minY : m_y - xPredkosc);
}

void Sterowanie::wDol()
{
    if(m_pauza) return;
    ustawY(m_y + xPredkosc > maxY ? maxY : m_y + xPredkosc);
}

void Sterowanie::strzalRakieta()
{
    if(m_pauza || m_gameOver)
        return;
    Pocisk* newPocisk = new Pocisk(this);
    newPocisk->ustawX(m_x + 50);
    newPocisk->ustawY(m_y + 20);
    newPocisk->ustawPredkosc(xPredkosc + 15);
    pociskList.append(newPocisk);
    emit bulletChanged();
}



void Sterowanie::stworzPrzeciwnika()
{
    if (m_pauza || m_gameOver || !m_gameStarted) return;
    Przeciwnik* nP = nullptr;

    // Przypisałem stworzu, domyślnie 1, jeśli m_level jest nieustawione
    int aktualnyLevel = (m_level > 0) ? m_level : 1;


    //Poziom 1:
    if (aktualnyLevel == 1)
    {
        // predkosc spawny przeciwnika
        nP = new Przeciwnik(this);
        czasSpawnu = 1000 + rand() % 1500;
    }


    //Poziom 2:
    else if (aktualnyLevel == 2)
    {

        if (rand() % 2 == 0)
        {
            nP = new Przeciwnik(this);
        } else {

            nP = new PrzeciwnikDuzy(this);
        }

        czasSpawnu = 800 + rand() % 1000;
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
        czasSpawnu = 600 + rand() % 800;
    }


    // Poziom 4
    else if(aktualnyLevel==4)
    {

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

         czasSpawnu = 200 + rand() % 500;
    }


    if (nP == nullptr)
    {
        // Restartujemy timer, żeby gra nie utknęła
        return;
    }

    // właściwości obiektu
    nP->ustawPoziom(aktualnyLevel);
    nP->ustawX(2000);
    nP->ustawY(100 + rand() % 600);

    //Dodanie do listy i powiadomienie QML
    przeciwnikList.append(nP);



    sPrzeciwnik.start(czasSpawnu);
    emit przeciwnikChanged();

}

void Sterowanie::aktualizujStan()
{
    if (m_pauza) return;

    //Usuwanie pocisków lecących poza ekran
    for (int i = pociskList.size() - 1; i >= 0; i--) {
        Pocisk* p = pociskList[i];
        if (p->x() > 2000 || p->x() < -100 || p->y() > 1100)
        {
            pociskList.removeAt(i);
            p->deleteLater();
            emit bulletChanged();
        }
    }

    // Obsługa przeciwników (ucieczka poza ekran LUB odliczanie wybuchu)
    for (int i = przeciwnikList.size() - 1; i >= 0; i--)
    {
        Przeciwnik* pr = przeciwnikList[i];

        // Jeśli uciekł żywy za lewą krawędź - usuwamy go
        if (pr->x() < -pr->rozmiar() && pr->pobierzTyp() != 99)
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
    //POCISKI I PRZECIWNICY
    for (int i = pociskList.size() - 1; i >= 0; i--)
    {
        Pocisk* pocisk = pociskList[i];

        for (int j = przeciwnikList.size() - 1; j >= 0; j--)
        {
            Przeciwnik* pr = przeciwnikList[j];
            if(pr->x() < 0 || pr->x() > 1920)
            {
                continue;
            }

            if (pr->pobierzTyp() == 99) continue;

            double pociskSzerokosc =  20.0;
            double pociskWysokosc = 10.0;
            double wrogRozmiar = pr->rozmiar();

            if (pocisk->x() < pr->x() + wrogRozmiar &&
                pocisk->x() + pociskSzerokosc > pr->x() &&
                pocisk->y() < pr->y() + wrogRozmiar &&
                pocisk->y() + pociskWysokosc > pr->y())
            {
                int nagroda = 25;
                if (pr->pobierzTyp() == 1) nagroda = 100;
                if (pr->pobierzTyp() == 2) nagroda = 10;
                 if (pr->pobierzTyp() == 3) nagroda = 50;
                ustawPunkty(m_punkty + nagroda);

                usunPocisk(pocisk);
                pr->ustawWybuch();
                pr->licznikWybuchu = 1;
                pocisk = nullptr;
                break;
            }
        }
    }

    // GRACZ I PRZECIWNICY
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

            if (m_zycie <= 0)
            {
                time.stop();
                sPrzeciwnik.stop();
                m_gameOver = true;
                emit gameOverZmiana();
                return;
            }
        }
    }

//tu dodałem zmianę levelu w zależności od zdobytych punktów

  int nowyPoziom = 1;

    if(m_punkty >= 1000)
    {
        nowyPoziom = 4;
    }
    else if(m_punkty >= 500)
    {
        nowyPoziom = 3;
    }
    else if(m_punkty >= 100)
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

void Sterowanie::startGame()
{
    m_gameStarted = true;
    m_gameOver = false;
    m_pauza = false;
    time.start(16);
    sPrzeciwnik.start(1200 + rand() % 1000);
}

//tu dodałem funkcję zatrzymującą grę, zatrzymuje przciwników po ESC
void Sterowanie::ustawPauze(bool p)
{
    m_pauza = p;

    if (m_pauza)
    {
        time.stop();
        sPrzeciwnik.stop();
    }
    else
    {
        time.start(16);
        sPrzeciwnik.start(1200 + rand() % 1000);
    }

    for (int i = 0; i < przeciwnikList.size(); i++)
    {
        przeciwnikList[i]->ustawPauze(p);
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

    qDeleteAll(przeciwnikList);
    przeciwnikList.clear();

    m_punkty = 0;
    m_x = 200;
    m_y = 490;
    m_level = 1;
    m_zycie = 100;

    emit zmianaPunkty();
    emit xZmiana();
    emit yZmiana();
    emit levelZmiana();
    emit zycieZmiana();
    m_gameOver = false;
    m_pauza = true;
    m_gameStarted = false;
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