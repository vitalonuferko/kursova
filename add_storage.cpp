#include "add_storage.h"
#include "ui_add_storage.h"

add_storage::add_storage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_storage)
{
    ui->setupUi(this);
}

//ctor with parametrs
add_storage::add_storage(QSqlQuery q,int m, QWidget *parent ):
    QDialog(parent),
    ui(new Ui::add_storage)
{
    this->query = q;
    mode = m;

    ui->setupUi(this);

    if (mode == 1)
    {
        ui->title->setText("Видалити склад");
        ui->btn_add->setText("Видалити");
    }

}

add_storage::~add_storage()
{
    delete ui;
}


void add_storage::on_btn_add_clicked()
{
    if (mode == 0)
    {
        if(query.exec("INSERT INTO storage (name) VALUES ('" + ui->name_edit->text() + "');"))
            close();
        else QMessageBox::critical(0, "error", "Unknown error");
    } else {
        bool exist = false;
        query.exec("SELECT * from storage");

        while(query.next())
            if(query.value(1).toString() == ui->name_edit->text())
                exist = true;
        if (exist)
        {
            query.exec("DELETE FROM storage WHERE name = '" + ui->name_edit->text() + "'");
            close();
        }
        else QMessageBox::warning(0, "error", "Такого складу не існує");

    }
}
