#include "dialog.h"
#include "ui_dialog.h"
#include "kontakt.h"

            // QDialog ???
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    _sql = QSqlDatabase::addDatabase("QSQLITE","baza");
    _sql.setDatabaseName("C:\\Users\\Marcin\\Desktop\\Organizator_sql\\Organizator_sql\\baza_spotkan.sqlite3");
    _sql.open();

    // wlaczamy obsluge kluczy obcych:
    QSqlQuery zapytanie(_sql);
    zapytanie.exec("PRAGMA foreign_keys = ON;");
    zapytanie.clear();

    isOpen = _sql.open();

    refresh();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pb_dodaj_clicked()
{
    formularz okno(1, 0, spotkania, ui->cw_kalendarz->selectedDate().toString("dd-MM-yyyy"), _sql, this);
    okno.exec();

    refresh();
}

void Dialog::on_pb_edytuj_clicked()
{
    spotkanie tmp = spotkania_tmp[ui->lw_spotkania->currentIndex().row()];

    formularz okno(2, tmp.id, spotkania, ui->cw_kalendarz->selectedDate().toString("dd-MM-yyyy"), _sql, this);
    okno.exec();

    refresh();
}

void Dialog::on_pb_podglad_clicked()
{
    spotkanie tmp = spotkania_tmp[ui->lw_spotkania->currentIndex().row()];

    formularz okno(3, tmp.id, spotkania, ui->cw_kalendarz->selectedDate().toString("dd-MM-yyyy"), _sql, this);
    okno.exec();

    refresh();
}

void Dialog::on_pb_usun_clicked()
{

    QMessageBox::StandardButton przycisk;

    przycisk = QMessageBox::question(this, "Usuwanie spotkania", "Czy na pewno chcesz usunac spotkanie?", QMessageBox::Yes | QMessageBox::No);

    if(przycisk == QMessageBox::Yes)
    {
       spotkanie tmp = spotkania_tmp[ui->lw_spotkania->currentIndex().row()];

       QSqlQuery _zapytanie(_sql);
       _zapytanie.exec("DELETE FROM spotkania WHERE id= " + QString::number(tmp.id));

       _zapytanie.clear();
    }

    refresh();
}

void Dialog::on_lw_spotkania_itemSelectionChanged()
{
    if(ui->lw_spotkania->count() > 0)
    {
        ui->pb_edytuj->setEnabled(true);
        ui->pb_podglad->setEnabled(true);
        ui->pb_usun->setEnabled(true);
    }
    else
    {
        ui->pb_edytuj->setEnabled(false);
        ui->pb_podglad->setEnabled(false);
        ui->pb_usun->setEnabled(false);
    }
}

void Dialog::on_cw_kalendarz_selectionChanged()
{
    refresh();
}



void Dialog::getData()
{
    if(isOpen)
    {
        QSqlQuery zapytanie(_sql);
        zapytanie.exec("PRAGMA foreign_keys = ON;");
        zapytanie.clear();

        // tworzymy tabele kontaktow:
        QString execute = "CREATE TABLE IF NOT EXISTS kontakty (id INTEGER UNIQUE PRIMARY KEY, imie TEXT, nazwisko TEXT, firma TEXT, email TEXT, numer_telefonu INTEGER);";
        zapytanie.exec(execute);
        zapytanie.clear();

        // tworzymy tabele spotkan:
        zapytanie.exec("CREATE TABLE IF NOT EXISTS spotkania (id INTEGER UNIQUE PRIMARY KEY, data_godzina TEXT, temat TEXT, opis TEXT, id_kontaktu INTEGER, FOREIGN KEY(id_kontaktu) REFERENCES kontakty(id))");
        zapytanie.clear();

        ui->lw_spotkania->clear();

        spotkania.clear();
        spotkanie tmp;

        zapytanie.exec("SELECT id, date(data_godzina), time(data_godzina), temat, opis, id_kontaktu FROM spotkania");


        while(zapytanie.next())
        {
            tmp.id = zapytanie.value(0).toInt();
            tmp.data = zapytanie.value(1).toDate();
            tmp.godzina = zapytanie.value(2).toTime();
            tmp.temat = zapytanie.value(3).toString();
            tmp.opis = zapytanie.value(4).toString();
            tmp.id_osoby = zapytanie.value(5).toInt();

            spotkania.append(tmp);
        }

        zapytanie.clear();

    }
    else
    {
        _msg.setText("Błąd odczytu bazy danych: " + _sql.lastError().text());
        _msg.exec();
    }

}



void Dialog::set_list_widget()
{
    if(spotkania.count() > 0)
    {
        spotkania_tmp.clear();

        ui->lw_spotkania->clear();

        for(int i=0; i<spotkania.count(); i++)
        {
            if(spotkania[i].data.toString("dd-MM-yyyy") == ui->cw_kalendarz->selectedDate().toString("dd-MM-yyyy"))
            {
                spotkania_tmp.append(spotkania[i]);
            }
        }


        sort_list_widget();

        for(int i=0; i<spotkania_tmp.count(); i++)
        {
           ui->lw_spotkania->addItem(spotkania_tmp[i].godzina.toString("HH:mm")+ " " + spotkania_tmp[i].temat);
        }

        if(spotkania_tmp.count() > 0)
        {
            ui->lw_spotkania->setCurrentRow(0);
        }
    }
}


void Dialog::refresh()
{
    getData();

    set_list_widget();

    on_lw_spotkania_itemSelectionChanged();
}


void Dialog::sort_list_widget()
{
    if(spotkania_tmp.count() >0)
    {
        bool flaga = false;

        do
        {
            flaga = false;

            for(int i = (spotkania_tmp.count()-1); 0<i ; i--)
            {
                spotkanie _item1 = spotkania_tmp[i-1];
                spotkanie _item2 = spotkania_tmp[i];

                if(_item1.godzina > _item2.godzina)
                {
                    spotkania_tmp[i-1] = _item2;
                    spotkania_tmp[i] = _item1;
                    flaga = true;
                }
            }
        }
        while(flaga != false);
    }
}

void Dialog::on_pb_kontakty_clicked()
{
    Kontakt okno(_sql, this);
    okno.exec();

    refresh();
}
