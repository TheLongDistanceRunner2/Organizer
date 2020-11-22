#ifndef KONTAKT_DODAJ_H
#define KONTAKT_DODAJ_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class kontakt_dodaj;
}

class kontakt_dodaj : public QDialog
{
    Q_OBJECT

public:
    explicit kontakt_dodaj(int typ, QSqlDatabase sql, QWidget *parent = nullptr);
    // konstruktor do uruchomienia podglądu:
    kontakt_dodaj(QString imie, QString nazwisko, QString firma, QString email, int tel, QSqlDatabase sql, QWidget *parent = nullptr);
    ~kontakt_dodaj();

    void set_imie(QString imie);
    void set_nazwisko(QString nazwisko);
    void set_email(QString email);
    void set_numer_telefonu(QString numer_telefonu);
    void set_firma(QString firma);

    void set_line_edits_edytuj_button();

    QString get_imie();
    QString get_nazwisko();
    QString get_email();
    QString get_numer_telefonu();
    QString get_firma();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::kontakt_dodaj *ui;

    QSqlDatabase _sql;
    int typ_metody;

    QString imie;
    QString nazwisko;
    QString email;
    QString numer_telefonu;
    QString firma;
};

#endif // KONTAKT_DODAJ_H
