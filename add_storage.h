#ifndef ADD_STORAGE_H
#define ADD_STORAGE_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class add_storage;
}

class add_storage : public QDialog
{
    Q_OBJECT

public:
    explicit add_storage(QWidget *parent = nullptr);
    add_storage(QSqlQuery q,int, QWidget *parent = nullptr);
    ~add_storage();
public slots:
    void on_btn_add_clicked();

private:
    Ui::add_storage *ui;
    QSqlQuery query;
    int mode;
};

#endif // ADD_STORAGE_H
