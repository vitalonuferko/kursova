#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QFileDialog>
#include <QMessageBox>
#include <QList>

#include <QEvent>

#include "add_storage.h"
#include "_items.h"
#include "quantity_of_goods.h"
#include "help.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct fields {
    QString name = "",
        code = "";
    double kg = 0;
    QString storage = "";
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase db;
    QSqlQuery query;

    bool db_open();
    void write_to_table(QList <fields>);

private slots:
    void on_menu_open_triggered();
    void on_menu_settings_add_ckl_triggered();
    void on_btn_all_goods_clicked();
    void on_menu_settings_add_good_triggered();
    void on_menu_settings_del_good_triggered();
    void on_menu_settings_del_ckl_triggered();
    void on_btn_ends_goods_clicked();
    void on_btn_left_goods_clicked();
    void on_search_textChanged(const QString &arg1);
    void on_menu_close_triggered();
    void on_btn_add_goods_clicked();
    void on_btn_wr_goods_clicked();
    void on_menu_create_triggered();

    void on_menu_about_triggered();

    void on_menu_help_triggered();

private:
    Ui::MainWindow *ui;
    QList <fields> tovar;
    QStringList headers = {"Найменування","штрих-код", "кількість товару (кг)", "Склад"};

protected:
    bool event(QEvent * event);

};
#endif // MAINWINDOW_H
