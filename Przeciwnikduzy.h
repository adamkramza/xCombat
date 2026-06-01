#ifndef PRZECIWNIKDUZY_H
#define PRZECIWNIKDUZY_H
#include "Przeciwnik.h"
using namespace std;

class PrzeciwnikDuzy : public Przeciwnik
{
    Q_OBJECT

public:
explicit PrzeciwnikDuzy(QObject* parent = nullptr);

public slots:
    void updatePrzeciwnik();
};

#endif