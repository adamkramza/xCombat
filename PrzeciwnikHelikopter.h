#ifndef PRZECIWNIKHELIKOPTER_H
#define PRZECIWNIKHELIKOPTER_H
#include "Przeciwnik.h"

class PrzeciwnikHelikopter : public Przeciwnik
{
    Q_OBJECT
public:
    explicit PrzeciwnikHelikopter(QObject* parent = nullptr);
public slots:
    void updatePrzeciwnik() override;
};
#endif