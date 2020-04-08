#ifndef TOVAR_H
#define TOVAR_H

#include <QDialog>
#include <QSqlQuery>
#include <QString>
#include <QMessageBox>

namespace Ui {
class quantity_of_goods;
}

class quantity_of_goods : public QDialog
{
    Q_OBJECT

public:
    explicit quantity_of_goods(QWidget *parent = nullptr);
    quantity_of_goods(QSqlQuery, int, QString, QWidget *parent = nullptr);
    ~quantity_of_goods();

private slots:
    void on_drop_name_currentIndexChanged(int index);

    void on_drop_code_currentIndexChanged(int index);

    void on_btn_clicked();

private:
    Ui::quantity_of_goods *ui;
    QSqlQuery query;
    QString storage = "";
    int mode;

};

#endif // TOVAR_H
