#ifndef PRZECIWNIKSAMOLOTZYGZAK_H
#define PRZECIWNIKSAMOLOTZYGZAK_H
#include "Przeciwnik.h"

class PrzeciwnikSamolotZygzak : public Przeciwnik
{
    Q_OBJECT
public:
    explicit PrzeciwnikSamolotZygzak(QObject* parent = nullptr);
public slots:
    void updatePrzeciwnik() override;
};
#endif