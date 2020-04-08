#ifndef _ITEMS_H
#define _ITEMS_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class _items;
}

class _items : public QDialog
{
    Q_OBJECT

public:
    explicit _items(QWidget *parent = nullptr);
        _items(QSqlQuery q,int mode, QString, QWidget *parent = nullptr);
    ~_items();

private slots:
        void on_btn_clicked();

private:
    Ui::_items *ui;
    QSqlQuery query;
    int mode;
    QString storage;
};

#endif // _ITEMS_H
