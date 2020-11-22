#include "kontakt_dodaj.h"
#include "ui_kontakt_dodaj.h"
#include <QtSql>
#include <qmessagebox.h>

kontakt_dodaj::kontakt_dodaj(int typ, QSqlDatabase sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kontakt_dodaj)
{
    ui->setupUi(this);
    _sql = sql;
    this->typ_metody = typ;

    // tworzymy tabelę, jeśli nie istnieje:
    QSqlQuery zapytanie(_sql);
    // tworzymy tabelę:
    QString execute = "CREATE TABLE IF NOT EXISTS kontakty (id INTEGER UNIQUE PRIMARY KEY, imie TEXT, nazwisko TEXT, firma TEXT, email TEXT, numer_telefonu INTEGER);";
    zapytanie.exec(execute);
}

// konstruktor do wywołania wyświetlenia:
kontakt_dodaj::kontakt_dodaj(QString imie, QString nazwisko, QString firma, QString email, int tel, QSqlDatabase sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kontakt_dodaj)
{
    ui->setupUi(this);
    _sql = sql;

    int tmp = tel;
    QString telefon = QString::number(tmp);

    ui->lineEdit_Imie->setText(imie);
    ui->lineEdit_Nazwisko->setText(nazwisko);
    ui->lineEdit_Firma->setText(firma);
    ui->lineEdit_Email->setText(email);
    ui->lineEdit_numer_telefonu->setText(telefon);

    // i wygaszamy pola:
    ui->lineEdit_Imie->setEnabled(false);
    ui->lineEdit_Nazwisko->setEnabled(false);
    ui->lineEdit_Firma->setEnabled(false);
    ui->lineEdit_Email->setEnabled(false);
    ui->lineEdit_numer_telefonu->setEnabled(false);
}

kontakt_dodaj::~kontakt_dodaj()
{
    delete ui;
}

void kontakt_dodaj::set_imie(QString imie)
{
    this->imie = imie;
}

void kontakt_dodaj::set_nazwisko(QString nazwisko)
{
    this->nazwisko = nazwisko;
}

void kontakt_dodaj::set_email(QString email)
{
    this->email = email;
}

void kontakt_dodaj::set_numer_telefonu(QString numer_telefonu)
{
    this->numer_telefonu = numer_telefonu;
}

void kontakt_dodaj::set_firma(QString firma)
{
    this->firma = firma;
}

void kontakt_dodaj::set_line_edits_edytuj_button()
{
    // ustawiamy dotychczasowy tekst w okienku:
    this->ui->lineEdit_Imie->setText(imie);
    this->ui->lineEdit_Nazwisko->setText(nazwisko);
    this->ui->lineEdit_Firma->setText(firma);
    this->ui->lineEdit_Email->setText(email);
    this->ui->lineEdit_numer_telefonu->setText(numer_telefonu);
}

QString kontakt_dodaj::get_imie()
{
    return imie;
}

QString kontakt_dodaj::get_nazwisko()
{
    return nazwisko;
}

QString kontakt_dodaj::get_email()
{
    return email;
}

QString kontakt_dodaj::get_numer_telefonu()
{
    return numer_telefonu;
}

QString kontakt_dodaj::get_firma()
{
    return firma;
}

void kontakt_dodaj::on_buttonBox_accepted()
{    
    switch(typ_metody)
    {
        // gdy chcemy dodac kontakt:
        case 1:
        {
            // łapiemy wpisane wartości:
            imie = ui->lineEdit_Imie->text();
            nazwisko = ui->lineEdit_Nazwisko->text();
            firma = ui->lineEdit_Firma->text();
            email = ui->lineEdit_Email->text();
            numer_telefonu = ui->lineEdit_numer_telefonu->text();

            // sprawdzamy, czy dany kontakt istnieje już w bazie danych:
            QSqlQuery _query(_sql);
            _query.clear();

            QString execute = "SELECT COUNT(*) FROM kontakty WHERE (imie='" + imie + "') AND (nazwisko='" + nazwisko + "') AND (firma='" + firma + "') AND (email='" + email + "') AND (numer_telefonu='" + numer_telefonu + "');";
            _query.prepare(execute);
            _query.exec();

            int ile = 0;
            if (_query.next())
            {
                ile = _query.value(0).toInt();
            }
            else
            {
                qDebug() << _query.lastError();
                ile = 1;
            }
            _query.clear();

            // gdy nie znaleziono takiego kontaktu w bazie, zapisujemy:
            if(ile == 0)
            {
                if(_sql.isOpen())
                {
                    QSqlQuery zapytanie(_sql);
                    zapytanie.exec("INSERT INTO kontakty (imie, nazwisko, firma, email, numer_telefonu) VALUES ('" + imie + "', '" + nazwisko + "', '" + firma + "', '" + email  + "', '" + numer_telefonu + "');");
                    zapytanie.clear();
                }
            }
            // gdy nie znaleziono, daj komunikat!
            else if(ile == 1)
            {
                QMessageBox::warning(
                    this,
                    tr("Organizer SQL"),
                    tr("PODANY KONTAKT JUZ ISTNIEJE W BAZIE !!!") );
            }

            break;
        }

        // gdy chcemy edytować kontakt:
        case 2:
        {
            // łapiemy wpisane wartości:
            QString nowe_imie = ui->lineEdit_Imie->text();
            QString nowe_nazwisko = ui->lineEdit_Nazwisko->text();
            QString nowa_firma = ui->lineEdit_Firma->text();
            QString nowy_email = ui->lineEdit_Email->text();
            QString nowy_numer_telefonu = ui->lineEdit_numer_telefonu->text();

            QSqlQuery _query(_sql);
            _query.clear();
            QString execute = "UPDATE kontakty SET imie = '" + nowe_imie +
                                                      "', nazwisko = '" + nowe_nazwisko +
                                                      "', firma = '" + nowa_firma +
                                                      "', email = '" + nowy_email +
                                                      "', numer_telefonu = '" + nowy_numer_telefonu +
                                             "' WHERE (imie='" + imie + "') AND (nazwisko='" + nazwisko + "') AND (firma='" + firma + "') AND (email='" + email + "') AND (numer_telefonu='" + numer_telefonu + "');";
            _query.prepare(execute);
            _query.exec();
            _query.clear();

            break;
        }

        // gdy chcemy usunac kontakt:
        case 3:
        {
            break;
        }
    }
}
