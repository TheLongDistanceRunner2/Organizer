#include "formularz.h"
#include "ui_formularz.h"
#include "kontakt.h"

formularz::formularz(int _typ, int _id, QList<spotkanie> _spotkania, QString _data, QSqlDatabase sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formularz)
{
    ui->setupUi(this);

    typ = _typ;
    id = _id;
    spotkania = _spotkania;
    data = QDate::fromString(_data, "dd-MM-yyyy");

    _sql = sql;

    //=========================================================
    // pobieramy dane osob z listy:

    QSqlQuery zapytanie(_sql);
    zapytanie.exec("SELECT imie, nazwisko, firma FROM kontakty");

    Osoba tmp;

    while(zapytanie.next())
    {
        tmp.imie = zapytanie.value(0).toString();
        tmp.nazwisko = zapytanie.value(1).toString();
        tmp.firma = zapytanie.value(2).toString();

        osoby.append(tmp);
    }

    zapytanie.clear();

    // ustawiamy kontakty w comboboxie:
    for (int i = 0; i < osoby.size(); i++)
    {
        ui->kontaktComboBox->addItem(osoby.at(i).imie + " " + osoby.at(i).nazwisko + " - " + osoby.at(i).firma);
    }


    QString title;

    switch(typ) {
    case 1:
        getFormularzDane();
        title = "Nowe";
        ui->te_godzina->setReadOnly(false);
        ui->le_temat->setReadOnly(false);
        ui->te_opis->setReadOnly(false);
        break;
    case 2:
        getFormularzDane();
        setDane();
        title = "Edycja";
        break;
    case 3:
        getFormularzDane();
        setDane();
        title = "Podgląd";
        break;

    }

    QDialog::setWindowTitle(QDialog::windowTitle() + " - " + title);
}

formularz::~formularz()
{
    delete ui;
}

void formularz::on_bb_ok_anuluj_accepted()
{
    if(typ != 3)
    {
        if(_sql.isOpen())
        {
            QSqlQuery _zapytanie(_sql);
            QString data_godzina;


            switch(typ)
            {
                case 1:
                {
                    data_godzina = data.toString("yyyy-MM-dd")+ " " + ui->te_godzina->time().toString("HH:mm:ss");

                    // lapiemy wybrana wartosc na comboboxie:
                    QString combobox = ui->kontaktComboBox->currentText();

                    // =========================================================================================================
                    // parsujemy QStringa combobox:
                    QString _Imie;
                    QString _Nazwisko;
                    QString _Firma;

                    int i = 0;

                    // dopoki nie napotkamy spacji:
                    while(combobox[i] != " ")
                    {
                        _Imie[i] = combobox[i];
                        i++;
                    }
                    //qDebug() << "_Imie: " << _Imie;

                    // usuwamy imie oraz spację z comboboxa:
                    combobox.remove(0, i + 1);
                    //qDebug() << "bez spacji: " << combobox;

                    // zerujemy i:
                    i = 0;

                    // dopoki nie napotkamy kolejnej spacji:
                    while(combobox[i] != " ")
                    {
                        _Nazwisko[i] = combobox[i];
                        i++;
                    }
                    //qDebug() << "_Nazwisko: " << _Nazwisko;

                    // usuwamy spacje, myslnik i spacje z comboboxa:
                    combobox.remove(0, i + 3);
                    //qDebug() << "bez spacji, myslnika i spacji: " << combobox;

                    // zerujemy i:
                    i = 0;

                    // dopoki jedziemy do konca stringa:
                    for (int i = 0; i < combobox.size(); i++)
                    {
                        _Firma[i] = combobox[i];
                    }
                    //qDebug() << "_Firma: " << _Firma;


                    // =========================================================================================================
                    // robimy selecta na tabeli kontakty, aby uzyskać id kontaktu o okreslonym imieniu, nazwisku i firmie:
                    _zapytanie.clear();
                    _zapytanie.exec("SELECT id FROM kontakty WHERE (imie='" + _Imie + "') AND (nazwisko='" + _Nazwisko + "') AND (firma='" + _Firma + "');");

                    _ID_kontaktu = "0";

                    // i zbieramy to id:
                    while (_zapytanie.next())
                    {
                           _ID_kontaktu = _zapytanie.value(0).toString();
                    }
                    //qDebug() << "_ID_kontaktu: " << _ID_kontaktu;


                    // dodajemy nasze spotkanie (z kontaktem) do bazy:
                    _zapytanie.exec("INSERT INTO spotkania (id, data_godzina, temat, opis, id_kontaktu) "
                                    " VALUES(" + QString::number(getLastID() + 1) + ", '"
                                    + data_godzina + "','" + ui->le_temat->text() + "', '"
                                    + ui->te_opis->toPlainText() + "', '"
                                    + _ID_kontaktu + "')");

                    _zapytanie.exec("");
                    _zapytanie.clear();


                    break;
                }
                case 2:
                {
                    data_godzina = data.toString("yyyy-MM-dd")+ " "
                            + ui->te_godzina->time().toString("HH:mm:ss");

                    // lapiemy id wybranego itemu na comboboxie:
                    int index = ui->kontaktComboBox->currentIndex();
                                              // UWAGA NA INDEKS!!!! INDEKSY W COMBOBOXIE LICZONE SA OD ZERA, A W BAZIE KONTAKTY MAMY OD JEDNEGO
                    QString current_index = QString::number(index + 1);
                    //qDebug() << "index: " << index;
                    //qDebug() << "current_index: " << current_index;

                    // updatujemy baze:
                    _zapytanie.exec("UPDATE spotkania SET data_godzina =  '" + data_godzina
                                                            + "', temat = '" + ui->le_temat->text()
                                                            + "', opis = '" + ui->te_opis->toPlainText() +
                                                            + "', id_kontaktu = '" + current_index +
                                                            + "' WHERE id = " + QString::number(id));
                    _zapytanie.clear();
                }
            }
        }
    }
//    else
//    {
//        _msg.setText("Błąd zapisu do bazy danych: " + _sql.lastError().text());
//        _msg.exec();
//    }
}


void formularz::getFormularzDane()
{
    ui->te_godzina->setTime(QTime::fromString("00:00", "HH:mm"));
    ui->le_temat->setText("");
    ui->te_opis->setText("");

    switch(typ) {
    case 1:
        ui->te_godzina->setEnabled(true);
        ui->le_temat->setEnabled(true);
        ui->te_opis->setEnabled(true);
        ui->kontaktComboBox->setEnabled(true);
        break;
    case 2:
        ui->te_godzina->setEnabled(true);
        ui->le_temat->setEnabled(true);
        ui->te_opis->setEnabled(true);
        ui->kontaktComboBox->setEnabled(true);
        break;
    case 3:
        ui->te_godzina->setEnabled(false);
        ui->le_temat->setEnabled(false);
        ui->te_opis->setEnabled(false);
        ui->kontaktComboBox->setEnabled(false);
        break;
    }
}


void formularz::setDane()
{
    if(id > 0)
    {
        foreach (spotkanie item, spotkania)
        {
            if(item.id == id)
            {
                ui->te_godzina->setTime(item.godzina);
                ui->le_temat->setText(item.temat);
                ui->te_opis->setText(item.opis);

                // lapiemy id tego spotkania:
                QString id_tego_spotkania = QString::number(id);

                // pobieramy z bazy danych id kontaktu przypisanego do tego spotkania:
                QSqlQuery _zapytanie(_sql);
                _zapytanie.clear();
                _zapytanie.exec("SELECT id_kontaktu FROM spotkania WHERE (id='" + id_tego_spotkania + "');");

                QString id_tego_kontaktu = "";

                // lapiemy id kontakt:
                while(_zapytanie.next())
                {
                    id_tego_kontaktu = _zapytanie.value(0).toString();
                }

                // majac id_kontaktu przeszukujemy tabele kontaktow po tym id:
                _zapytanie.clear();
                _zapytanie.exec("SELECT imie, nazwisko, firma FROM kontakty WHERE (id='" + id_tego_kontaktu + "');");

                QString imie;
                QString nazwisko;
                QString firma;

                // odbieramy dane i ładujemy je do zmiennych:
                while(_zapytanie.next())
                {
                    imie = _zapytanie.value(0).toString();
                    nazwisko = _zapytanie.value(1).toString();
                    firma = _zapytanie.value(2).toString();
                }

                // tu wrzucamy tekst do comboboxa:
                QString textToFind = imie + " " + nazwisko + " - " + firma;
                int index = ui->kontaktComboBox->findText(textToFind);
                ui->kontaktComboBox->setCurrentIndex(index);
            }
        }
    }
}


int formularz::getLastID()
{
    if(spotkania.count() > 0)
    {
        return spotkania[spotkania.count()-1].id;
    }
    else
    {
        return 0;
    }
}


