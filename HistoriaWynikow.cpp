#include "HistoriaWynikow.h"
#include <algorithm>
using namespace std;

HistoriaWynikow::HistoriaWynikow(QObject *parent) : QObject(parent) {}

void HistoriaWynikow::zapiszWynik(QString imie, int punkty)
{
    QSettings settings("xCombat");
    settings.beginGroup("Wyniki");
    settings.setValue(imie, punkty);
    settings.endGroup();
}




//FUNKCJA POBIERAJĄCA ORAZ SORTUJĄCA WYNIKI W TABLELI
QStringList HistoriaWynikow::pobierzWyniki()
{
    QSettings settings("xCombat");
    settings.beginGroup("Wyniki");

    const QStringList klucze = settings.childKeys(); // Pobieramy wszystkie zapisane imiona graczy

    // Tworzymy prostą strukturę lokalną, aby powiązać imię z punktami
    struct WpisWyniku {
        QString imie;
        int punkty;
    };

    QList<WpisWyniku> listaTymczasowa;

    // Zczytujemy wszystkie dane z QSettings do naszej listy strukturalnej
    for (const QString &klucz : klucze) {
        int pkt = settings.value(klucz).toInt();
        WpisWyniku wpis;
        wpis.imie = klucz;
        wpis.punkty = pkt;
        listaTymczasowa.append(wpis);
    }
    settings.endGroup();

    //Sortujemy listę tymczasową od największego do najmniejszego wyniku
    sort(listaTymczasowa.begin(), listaTymczasowa.end(), [](const WpisWyniku& a, const WpisWyniku& b) {
        return a.punkty > b.punkty;
    });

    // Przepisujemy posortowane dane do QStringList dla QML
    QStringList listaDoQML;
    for (const WpisWyniku &wpis : listaTymczasowa) {
        listaDoQML.append(wpis.imie + " - " + QString::number(wpis.punkty) + " pkt");
    }

    return listaDoQML;
}
