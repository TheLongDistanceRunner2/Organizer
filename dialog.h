#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <spotkanie.h>
#include <QString>
#include <formularz.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QtXml/QtXml>
#include <QtSql>



namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pb_dodaj_clicked();

    void on_pb_edytuj_clicked();

    void on_pb_podglad_clicked();

    void on_pb_usun_clicked();

    void on_lw_spotkania_itemSelectionChanged();

    void on_cw_kalendarz_selectionChanged();

    void on_pb_kontakty_clicked();

private:
    Ui::Dialog *ui;

    QList<spotkanie> spotkania;

    QList<spotkanie> spotkania_tmp;

    QDomDocument _xml;

    QSqlDatabase _sql;

    QMessageBox _msg;

    bool isOpen;

    void getData();

    void set_list_widget();

    void sort_list_widget();

    void refresh();
};

#endif // DIALOG_H
