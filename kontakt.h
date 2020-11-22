#ifndef KONTAKT_H
#define KONTAKT_H

#include <QDialog>
#include <QSqlDatabase>
#include <qmessagebox.h>
#include "osoba.h"

namespace Ui {
class Kontakt;
}

class Kontakt : public QDialog
{
    Q_OBJECT

public:
    explicit Kontakt(QSqlDatabase sql, QDialog *parent = nullptr);
    ~Kontakt();

    QList<Osoba> osoby;

    void set_imie(QString imie);
    void set_nazwisko(QString nazwisko);
    void set_email(QString email);
    void set_numer_telefonu(QString numer_telefonu);
    void set_firma(QString firma);

    QString get_imie();
    QString get_nazwisko();
    QString get_firma();
    QString get_email();
    QString get_numer_telefonu();

    // pobieramy dane z bazy do wyśwetlenia na liście:
    void getData();
    // ustawiamy zawartość listy:
    void set_list_widget();
    void set_list_widget_po_imieniu();
    void set_list_widget_po_nazwisku();
    void set_list_widget_po_firmie();

private slots:
    void on_pushButtonDodaj_clicked();

    void on_pushButtoPodglad_clicked();

    void on_pushButtonEdytuj_clicked();

    void on_pushButtonSkasuj_clicked();

    void on_pushButton_sort_Imie_clicked();

    void on_pushButton_sort_Nazwisko_clicked();

    void on_pushButton_sort_firma_clicked();

private:
    Ui::Kontakt *ui;

    QMessageBox _msg;
    QSqlDatabase _sql;
    bool isOpen;



    QString imie;
    QString nazwisko;
    QString firma;
    QString email;
    QString numer_telefonu;

};

#endif // KONTAKT_H
