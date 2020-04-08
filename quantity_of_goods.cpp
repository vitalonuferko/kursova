#include "quantity_of_goods.h"
#include "ui_quantity_of_goods.h"

quantity_of_goods::quantity_of_goods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::quantity_of_goods)
{
    ui->setupUi(this);
}


//ctor with parametrs
quantity_of_goods::quantity_of_goods(QSqlQuery q,int mode,QString s, QWidget *parent ):
    QDialog(parent),
    ui(new Ui::quantity_of_goods)
{

    this->query = q;
    storage = s;

    ui->setupUi(this);
    this->mode = mode;

    if (mode == 0)
        ui->btn->setText("Додати");
    else ui->btn->setText("Списати");

    query.exec("SELECT name, code, kg FROM tovar WHERE storage = '" + storage +  "';");
    while(query.next())
    {
        ui->drop_name->addItem(query.value(0).toString());
        ui->drop_code->addItem(query.value(1).toString());
    }
}

quantity_of_goods::~quantity_of_goods()
{
    delete ui;
}

// change dropdown index (name - code)
void quantity_of_goods::on_drop_name_currentIndexChanged(int index)
{
    ui->drop_code->setCurrentIndex(index);
}
void quantity_of_goods::on_drop_code_currentIndexChanged(int index)
{
    ui->drop_name->setCurrentIndex(index);
}

void quantity_of_goods::on_btn_clicked()
{
    double kg =ui->line_sum->text().toDouble();
    QString name = ui->drop_name->currentText(),
            code =  ui->drop_code->currentText();
    if (kg > 0)
    {
        if (mode == 0)
        {
            query.exec("UPDATE tovar SET kg = kg +" + QString::number(kg) + " WHERE name = '" + name +
                       "' and code = '" + code + "'");
            close();
        } else
        {
            query.exec("SELECT name, code, kg FROM tovar WHERE storage = '" + storage +  "' and name = '"
                       + name + "' and code = '" + code + "';");
            query.next();
            if (query.value(2).toDouble() > kg)
            {
                QString str = "UPDATE tovar SET kg = kg - " + QString::number(kg) + " WHERE name = '" + name +
                        "' and code = '" + code + "'";
                query.exec(str);
                close();
            }
            else QMessageBox::warning(0, "warning", "Недостатньо товару (кг) на складі: " + storage);
        }
    } else {
        QMessageBox::critical(0, "error", "Неправильний ввід даних. \nДив. Інформація->Інструкція");
    }
}
