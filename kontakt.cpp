#include "kontakt.h"
#include "ui_kontakt.h"
#include "kontakt_dodaj.h"
#include "osoba.h"
#include <QSqlQuery>
#include <QtDebug>
#include <qalgorithms.h>
#include <algorithm>

Kontakt::Kontakt(QSqlDatabase sql, QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Kontakt)
{
    ui->setupUi(this);
    _sql = sql;
    isOpen = _sql.open();

    // pobieramy dane z bazy i uzupełniamy nimi listę osób:
    getData();
    // i wyświetlamy początkowy stan listy:
    if(osoby.count() > 0)
    {

        for(int i=0; i<osoby.count(); i++)
        {
            ui->lista_osob->addItem(osoby[i].imie + " " + osoby[i].nazwisko + " - " + osoby[i].firma);
        }

        if(osoby.count() > 0)
        {
            ui->lista_osob->setCurrentRow(0);
        }
    }
    else
    {
        // wygaszamy odpowiednie przyciski:
        ui->pushButtonEdytuj->setEnabled(false);
        ui->pushButtoPodglad->setEnabled(false);
        ui->pushButtonSkasuj->setEnabled(false);
    }

    // czyscimy liste osob:
    //osoby.clear();
}

Kontakt::~Kontakt()
{
    delete ui;
}

void Kontakt::set_imie(QString imie)
{
    this->imie = imie;
}

void Kontakt::set_nazwisko(QString nazwisko)
{
    this->nazwisko = nazwisko;
}

void Kontakt::set_email(QString email)
{
    this->email = email;
}

void Kontakt::set_numer_telefonu(QString numer_telefonu)
{
    this->numer_telefonu = numer_telefonu;
}

void Kontakt::set_firma(QString firma)
{
    this->firma = firma;
}

QString Kontakt::get_imie()
{
    return imie;
}

QString Kontakt::get_nazwisko()
{
    return nazwisko;
}

QString Kontakt::get_firma()
{
    return firma;
}

QString Kontakt::get_email()
{
    return email;
}

QString Kontakt::get_numer_telefonu()
{
    return numer_telefonu;
}

void Kontakt::on_pushButtonDodaj_clicked()
{
    kontakt_dodaj okno(1, _sql, this);
    okno.exec();

    // łapiemy te wartosci wpisane w oknie i teraz trzeba dodać do listy:
    imie = okno.get_imie();
    nazwisko = okno.get_nazwisko();
    firma = okno.get_firma();
    email = okno.get_email();
    numer_telefonu = okno.get_numer_telefonu();

    getData();
    set_list_widget();

    // odgaszamy odpowiednie przyciski:
    ui->pushButtonEdytuj->setEnabled(true);
    ui->pushButtoPodglad->setEnabled(true);
    ui->pushButtonSkasuj->setEnabled(true);
}

// odczyt danych z bazy i wrzucenie ich do listy:
void Kontakt::getData()
{
    if(isOpen)
    {
        //ui->lista_osob->clear();
        osoby.clear();

        QSqlQuery query(_sql);
        query.exec("SELECT imie, nazwisko, firma, email, numer_telefonu FROM kontakty");

        Osoba osoba;

        while(query.next())
        {
            osoba.imie = query.value(0).toString();
            osoba.nazwisko = query.value(1).toString();
            osoba.firma = query.value(2).toString();
            osoba.email = query.value(3).toString();
            osoba.numer_telefonu = query.value(4).toString();

            osoby.append(osoba);
        }

        //qDebug() << "item: " << osoby[0].imie;
        query.clear();
    }
    else
    {
        _msg.setText("Błąd odczytu bazy danych: ");
        _msg.exec();
    }
}

//=====================================================================================
// wyśwetlanie listy:
void Kontakt::set_list_widget()
{
    // czyścimy widget z lista osob:
    ui->lista_osob->clear();

    if(osoby.count() > 0)
    {
        for(int i=0; i<osoby.count(); i++)
        {
            ui->lista_osob->addItem(osoby[i].imie + " " + osoby[i].nazwisko + " - " + osoby[i].firma);
        }

        if(osoby.count() > 0)
        {
            ui->lista_osob->setCurrentRow(0);
        }
    }

}

void Kontakt::set_list_widget_po_imieniu()
{
    // czyścimy widget z lista osob:
    ui->lista_osob->clear();

    if(osoby.count() > 0)
    {
        for(int i=0; i<osoby.count(); i++)
        {
            ui->lista_osob->addItem(osoby[i].imie + " " + osoby[i].nazwisko + " - " + osoby[i].firma);
        }

        if(osoby.count() > 0)
        {
            ui->lista_osob->setCurrentRow(0);
        }
    }

}

void Kontakt::set_list_widget_po_nazwisku()
{
    // czyścimy widget z lista osob:
    ui->lista_osob->clear();

    if(osoby.count() > 0)
    {
        for(int i=0; i<osoby.count(); i++)
        {
            ui->lista_osob->addItem(osoby[i].nazwisko + " " + osoby[i].imie + " - " + osoby[i].firma);
        }

        if(osoby.count() > 0)
        {
            ui->lista_osob->setCurrentRow(0);
        }
    }

}

void Kontakt::set_list_widget_po_firmie()
{
    // czyścimy widget z lista osob:
    ui->lista_osob->clear();

    if(osoby.count() > 0)
    {
        for(int i=0; i<osoby.count(); i++)
        {
            ui->lista_osob->addItem(osoby[i].firma + " - " + osoby[i].imie + " " + osoby[i].nazwisko);
        }

        if(osoby.count() > 0)
        {
            ui->lista_osob->setCurrentRow(0);
        }
    }
}

//=====================================================================================

void Kontakt::on_pushButtoPodglad_clicked()
{
    int item = ui->lista_osob->currentIndex().row();  //pobieramy aktualne id zaznaczonego spotkania

    QString _imie = osoby[item].imie;
    QString _nazwisko = osoby[item].nazwisko;
    QString _firma = osoby[item].firma;
    QString _email = osoby[item].email;
    int _tel = osoby[item].numer_telefonu.toInt();

    kontakt_dodaj okno (_imie, _nazwisko, _firma, _email, _tel, _sql, this);
    okno.exec();
}

void Kontakt::on_pushButtonEdytuj_clicked()
{
    // pobieramy aktualne id zaznaczonego spotkania:
    int item = ui->lista_osob->currentIndex().row();

    QString _imie = osoby[item].imie;
    QString _nazwisko = osoby[item].nazwisko;
    QString _firma = osoby[item].firma;
    QString _email = osoby[item].email;
    QString _tel = osoby[item].numer_telefonu;

    kontakt_dodaj okno(2, _sql, this);
    okno.set_imie(_imie);
    okno.set_nazwisko(_nazwisko);
    okno.set_firma(_firma);
    okno.set_email(_email);
    okno.set_numer_telefonu(_tel);
    okno.set_line_edits_edytuj_button();

    okno.exec();

    getData();
    set_list_widget();
}

void Kontakt::on_pushButtonSkasuj_clicked()
{
    //pobieramy aktualne id zaznaczonej osoby:
    int item = ui->lista_osob->currentIndex().row();

    QString _imie = osoby[item].imie;
    QString _nazwisko = osoby[item].nazwisko;
    QString _firma = osoby[item].firma;
    QString _email = osoby[item].email;
    QString _tel = osoby[item].numer_telefonu;

    qDebug() << "imie: " << _imie;
    qDebug() << "nazwisko: " << _nazwisko;
    qDebug() << "firma: " << _firma;
    qDebug() << "email: " << _email;
    qDebug() << "tel:" << _tel;

    QMessageBox msgBox;
        msgBox.setWindowTitle("Uwaga!");
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setInformativeText("Czy na pewno chcesz usunac kontakt?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);

        // wywołanie okienka z zapytaniem:
        int ret = msgBox.exec();

        // interpretacja wyboru użytkownika:
        switch (ret)
        {
          case QMessageBox::Ok:
          {
            QSqlQuery zapytanie(_sql);
            zapytanie.exec("DELETE FROM kontakty WHERE imie = '" + _imie + "' AND nazwisko = '" + _nazwisko + "' AND firma = '" + _firma + "' AND email = '" + _email + "' AND numer_telefonu = '" + _tel + "';");
            // to działa:
            //zapytanie.exec("delete from kontakty where imie = 'Jan' AND nazwisko = 'Betoniarek' AND firma = 'Southampton' AND email = 'jan.betoniarek@wp.pl' AND numer_telefonu = '23232122';");
            zapytanie.clear();

            getData();
            set_list_widget();

            if(osoby.count() == 0)
            {
                // wygaszamy odpowiednie przyciski:
                ui->pushButtonEdytuj->setEnabled(false);
                ui->pushButtoPodglad->setEnabled(false);
                ui->pushButtonSkasuj->setEnabled(false);
            }
            break;

          }
          case QMessageBox::Cancel:
             // przerwanie działania
             break;

        default:
             // ten fragment nigdy nie powinien być wywołany
             break;
   }
}

//=============================================================================
// funkcje do sortowania:                  !!! nie deklaruj nagłowka w kontakt.h, bo wtedy nie działa !!!
bool sortuj_po_imieniu(const Osoba os1, const Osoba os2)
{
    return os1.imie < os2.imie;
}

bool sortuj_po_nazwisku(const Osoba os1, const Osoba os2)
{
    return os1.nazwisko < os2.nazwisko;
}

bool sortuj_po_firmie(const Osoba os1, const Osoba os2)
{
    return os1.firma < os2.firma;
}

void Kontakt::on_pushButton_sort_Imie_clicked()
{
    // sortujemy liste osob po imieniu:
    std::sort(osoby.begin(), osoby.end(), sortuj_po_imieniu);

    // odswiezamy wyswietlanie listy:
    set_list_widget_po_imieniu();
}

void Kontakt::on_pushButton_sort_Nazwisko_clicked()
{
    // sortujemy liste osob po nazwisku:
    std::sort(osoby.begin(), osoby.end(), sortuj_po_nazwisku);

    // odswiezamy wyswietlanie listy:
    set_list_widget_po_nazwisku();
}

void Kontakt::on_pushButton_sort_firma_clicked()
{
    // sortujemy liste osob po firmie:
    std::sort(osoby.begin(), osoby.end(), sortuj_po_firmie);

    // odswiezamy wyswietlanie listy:
    set_list_widget_po_firmie();
}
