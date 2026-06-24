#include "HistoriaWynikow.h"
#include <QSettings>
#include <algorithm>

using namespace std;

HistoriaWynikow::HistoriaWynikow(QObject *parent)
    : QObject(parent){}


void HistoriaWynikow::zapiszWynik(QString imie, int punkty)
{
    struct Wynik {
        QString imie;
        int punkty;
    };

    QList<Wynik> lista;

    QSettings settings("xCombat");

    //odczyt
    settings.beginGroup("Wyniki");

    int ile = settings.value("ile", 0).toInt();

    for (int i = 0; i < ile; i++)
    {
        settings.beginGroup(QString::number(i));

        Wynik w;
        w.imie = settings.value("imie").toString();
        w.punkty = settings.value("punkty").toInt();

        lista.append(w);

        settings.endGroup();
    }

    settings.endGroup();

    //dodaj nowy wynik - imie i pinkty
    lista.append({imie, punkty});

    // funkcja sortująca listę
    sort(lista.begin(), lista.end(),
         [](const Wynik &a, const Wynik &b)
         {
             return a.punkty > b.punkty;
         });


    if (lista.size() > 10)
        lista = lista.mid(0, 10);


    settings.beginGroup("Wyniki");
    settings.remove(""); // czyści stare dane
    settings.setValue("ile", lista.size());

    for (int i = 0; i < lista.size(); i++)
    {
        settings.beginGroup(QString::number(i));
        settings.setValue("imie", lista[i].imie);
        settings.setValue("punkty", lista[i].punkty);
        settings.endGroup();
    }
    settings.endGroup();
}


QStringList HistoriaWynikow::pobierzWyniki()
{
    QStringList wynik;
    QSettings settings("xCombat");

    settings.beginGroup("Wyniki");

    int ile = settings.value("ile", 0).toInt();
    for (int i = 0; i < ile; i++)
    {
        settings.beginGroup(QString::number(i));

        QString imie = settings.value("imie").toString();
        int punkty = settings.value("punkty").toInt();

        wynik.append(imie + " - " + QString::number(punkty) + " pkt");

        settings.endGroup();
    }
    settings.endGroup();
    return wynik;
}