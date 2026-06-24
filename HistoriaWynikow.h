#ifndef HISTORIAWYNIKOW_H
#define HISTORIAWYNIKOW_H

#include <QObject>
#include <QStringList>

class HistoriaWynikow : public QObject
{
    Q_OBJECT

public:
    explicit HistoriaWynikow(QObject *parent = nullptr);

    Q_INVOKABLE void zapiszWynik(QString imie, int punkty);
    Q_INVOKABLE QStringList pobierzWyniki();
};

#endif