#ifndef SPOTKANIE_H
#define SPOTKANIE_H

#include <QDate>
#include <QTime>
#include <QString>
#include "osoba.h"


class spotkanie
{

public:
    spotkanie();

    int id;
    QDate data;
    QTime godzina;
    QString temat;
    QString opis;
    Osoba osoba;
    int id_osoby;
};

#endif // SPOTKANIE_H
