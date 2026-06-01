#ifndef PRZECIWNIK_H
#define PRZECIWNIK_H
#include <QObject>
#include <QTimer>
using namespace std;

class Przeciwnik : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double x READ x WRITE ustawX NOTIFY xZmiana)
    Q_PROPERTY(double y READ y WRITE ustawY NOTIFY yZmiana)
    Q_PROPERTY(int typ READ pobierzTyp NOTIFY typZmiana)
    Q_PROPERTY(int rozmiar READ rozmiar CONSTANT)


protected:
    double m_x;
    double m_y;
    double yPredkosc;
    double xPredkosc;
    QTimer eTime;

    int m_typ;          // 0 - zwykły, 1 - szybki, 2 - duży, 3 - dolny, 99 - WYBUCH
    int m_rozmiar;      // tutaj trzymamy rozmiar przeciwnika (W pikselach, np. 60, 120, 140)

public:
    int licznikWybuchu; // Licznik do odliczania czasu trwania wybuchu
    Przeciwnik(QObject* parent = nullptr);
    virtual ~Przeciwnik() {};
    double x();
    double y();
    int rozmiar(); // Zwykły getter, bez "virtual"
    int pobierzTyp();
    void ustawPoziom(int level);
    void ustawX(double wartosc);
    void ustawY(double wartosc);
    void ustawWybuch();

public slots:
    void updatePrzeciwnik();

signals:
    void xZmiana();
    void yZmiana();
    void typZmiana();
};

#endif