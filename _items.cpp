#include "_items.h"
#include "ui__items.h"

_items::_items(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::_items)
{
    ui->setupUi(this);

}

//ctor with parametrs
_items::_items(QSqlQuery q,int mode,QString s, QWidget *parent ):
    QDialog(parent),
    ui(new Ui::_items)
{

    this->query = q;
    storage = s;
    // ui->btn->;
    ui->setupUi(this);
    this->mode = mode;
    if (mode == 1)
    {
        ui->info->setText("*для видалення введіть один із параметрів");
        ui->btn->setText("Видалити товар");
    }
    else ui->btn->setText("Додати товар");
}

_items::~_items()
{
    delete ui;
}

void _items::on_btn_clicked()
{
    QString name = ui->line_name->text(),
            code = ui->line_code->text();
    if (mode == 0)
    {
        if (name == "" || code == "")
            QMessageBox::critical(0, "error", "Заповніть всі поля");
        else{
            bool exist = false;
            query.exec("SELECT * from tovar");
            while(query.next())
            {
                if ((query.value(1).toString() == name) ||
                        (query.value(3).toString() == code))
                    exist = true;
            }
            if (!exist)
            {
                query.exec("INSERT INTO tovar (name, code, kg, storage) VALUES ('" + name +
                           "', '" + code + "', 0,'" + storage + "');");
                close();
            } else
                QMessageBox::warning(0, "error", "Вказане поле вже існує");
        }
    }
    else
    {
        if ((name == "" && code == "") || (name != "" && code != ""))
        {
            QMessageBox::warning(0, "error", "Wrong input");
        } else
        {
            QString str;
            if (name != "")
            {
                str = "DELETE FROM tovar WHERE name = '" + name + "'";
            } else
                if (code != "")
                {
                    str = "DELETE FROM tovar WHERE code = '" + code + "'";
                }
            if (query.exec(str))
                close();
            else QMessageBox::warning(0, "error", "Такого поля не існує");
        }
    }
}
