#ifndef HISTORIAWYNIKOW_H
#define HISTORIAWYNIKOW_H

#include <QObject>
#include <QStringList>
#include <QSettings>

class HistoriaWynikow : public QObject
{
    Q_OBJECT
public:
    explicit HistoriaWynikow(QObject *parent = nullptr);

    // Zapisuje imię i wynik
    Q_INVOKABLE void zapiszWynik(QString imie, int punkty);

    // Zwraca listę gotowych tekstów "Imie - Punkty" dla interfejsu QML
    Q_INVOKABLE QStringList pobierzWyniki();
};

#endif