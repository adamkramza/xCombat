#ifndef PRZECIWNIKSZYBKI_H
#define PRZECIWNIKSZYBKI_H
#include "Przeciwnik.h"

class PrzeciwnikSzybki : public Przeciwnik
{
    Q_OBJECT

public:

 explicit PrzeciwnikSzybki(QObject* parent = nullptr);


public slots:
    void updatePrzeciwnik();
};

#endif