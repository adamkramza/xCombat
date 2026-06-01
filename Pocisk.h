#ifndef POCISK_H
#define POCISK_H
#include <QObject>
#include <QTimer>
using namespace std;

class Pocisk:public QObject
{
    Q_OBJECT
    Q_PROPERTY(double x READ x WRITE ustawX NOTIFY xZmiana)
    Q_PROPERTY(double y READ y WRITE ustawY NOTIFY yZmiana)

protected:
    double m_x;
    double m_y;
    double yPredkosc;
    double xPredkosc;
    QTimer bTime;

public:

    Pocisk (QObject* parent=nullptr);    //konstruktor z listą inicjalizacyjną
    double x();
    double y();
    void ustawX(double wartosc);
    void ustawY(double wartosc);
    virtual ~Pocisk() {}
    void ustawPredkosc(double p);


public slots:
    virtual void updatePocisk();


signals:
    void xZmiana();
    void yZmiana();
    void pociskZniszczony();




};

#endif