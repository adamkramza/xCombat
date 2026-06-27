#ifndef PRZECIWNIKUFO_H
#define PRZECIWNIKUFO_H
#include "Przeciwnik.h"

class PrzeciwnikUFO : public Przeciwnik
{
    Q_OBJECT

public:
    explicit PrzeciwnikUFO(QObject* parent = nullptr);

public slots:
    void updatePrzeciwnik() override;
};

#endif
