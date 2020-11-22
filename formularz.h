#ifndef FORMULARZ_H
#define FORMULARZ_H

#include <QDialog>
#include <QList>
#include <spotkanie.h>
#include <QDate>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QtXml/QtXml>
#include <QtSql>
#include <QMessageBox>
#include "kontakt.h"

namespace Ui {
class formularz;
}

class formularz : public QDialog
{
    Q_OBJECT

public:
    explicit formularz(int _typ, int _id, QList<spotkanie> _spotkania, QString _data, QSqlDatabase sql, QWidget *parent = nullptr);
    ~formularz();

private slots:
    void on_bb_ok_anuluj_accepted();

private:
    Ui::formularz *ui;

    int typ;
    int id;
    QList<spotkanie> spotkania;
    QList<Osoba> osoby;
    QDate data;
    QDomDocument _xml;
    QSqlDatabase _sql;
    QMessageBox _msg;

    bool isOpen;

    void getFormularzDane();
    void setDane();
    int getLastID();

    QString _ID_kontaktu;
};

#endif // FORMULARZ_H
