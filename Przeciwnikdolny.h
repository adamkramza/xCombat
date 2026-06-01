#ifndef PRZECIWNIKDOLNY_H
#define PRZECIWNIKDOLNY_H
#include "Przeciwnik.h"
using namespace std;


class PrzeciwnikDolny : public Przeciwnik
{
    Q_OBJECT

public:
    explicit PrzeciwnikDolny(QObject* parent = nullptr);

public slots:
    void updatePrzeciwnik();

private:
    bool zatrzymany = false;
    int licznikStop = 0;

};

#endif