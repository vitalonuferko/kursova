#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table->setHorizontalHeaderLabels(headers);  //встановлюєм заголовки таблиці
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //забороняєм редагування таблиці

    ui->table->resizeColumnsToContents();           //підганяєм розміри стовпця
}

MainWindow::~MainWindow()
{
    db.close(); //закриваєм базу
    delete ui;
}

//перехват всіх подій
bool MainWindow::event(QEvent *event)
{
    // подія - головне вікно активне (перегрузка складів і товарів) (можливо не оптимізовано)
    if (event->type() == QEvent::WindowActivate)
    {
        //якшо база даних відкрита
        if (db.open())
        {
            query.exec("SELECT * from storage");
            ui->ckl_choose->clear();
            while(query.next())
                ui->ckl_choose->addItem(query.value(1).toString());


            query.exec("SELECT * from tovar");

            tovar.clear();
            fields f;

            while(query.next())
            {
                f.name = query.value(1).toString();
                f.code = query.value(2).toString();
                f.kg = query.value(3).toDouble();
                f.storage = query.value(4).toString();
                tovar.push_back(f);
            }
        }

    }
    return QWidget::event(event);
}


// function if db open
bool MainWindow::db_open(){
    if (db.open())
        return true;
    else QMessageBox::warning(0, "error", "База даних не відкрита\nФайл->Відкрити базу даних");
    return false;
}

// function add content to table
void MainWindow::write_to_table(QList<fields> list)
{
    ui->table->clear();
    ui->table->setHorizontalHeaderLabels(headers);
    int row_count = 0; // рахує рядки шоб потім в них вставити
    ui->table->setRowCount(1);
    //прохід по всьому списку
    QList<fields>::const_iterator i;
    for (i = list.begin(); i != list.end(); ++i)
    {
        ui->table->setItem(row_count, 0, new QTableWidgetItem(i->name));
        ui->table->setItem(row_count, 1, new QTableWidgetItem(i->code));
        ui->table->setItem(row_count, 2, new QTableWidgetItem(QString::number(i->kg)));
        ui->table->setItem(row_count, 3, new QTableWidgetItem(i->storage));
        row_count++;
        ui->table->insertRow(row_count);
    }
    ui->table->resizeRowsToContents(); //змінює розміри клітинки під контент
}


//open db file
void MainWindow::on_menu_open_triggered()
{
    QString path_to_db = QFileDialog::getOpenFileName(this,tr("Open db file"), "database", tr("db files (*.db)"));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_to_db);

    if (db_open())
    {
        ui->db_path->setText(path_to_db);
        query = QSqlQuery(db);

        int needed_db = 0;
        for (const QString &tableName : db.tables())
        {
            if (tableName == "tovar" || tableName == "storage")
                needed_db ++;
        }
        if (needed_db < 2)
            QMessageBox::warning(0, "error", "db файл не містить потрібних таблиць.");
    } else
    {
        QMessageBox::warning(0, "error", "db файл не було відкрито");
    }
}

//add storage name to database and change dropdown
void MainWindow::on_menu_settings_add_ckl_triggered()
{
    if (db_open())
    {
        add_storage * as = new add_storage(query, 0);
        as->setModal(true);
        as->show();
    }
}

//button all goods
void MainWindow::on_btn_all_goods_clicked()
{
    if (db_open())
    {
        write_to_table(tovar);
    }
}

//menu add good(додати товар)
void MainWindow::on_menu_settings_add_good_triggered()
{
    if (db_open())
    {
        if (ui->ckl_choose->currentText() != "") {
            _items * it = new _items(query, 0, ui->ckl_choose->currentText());
            it->setModal(true);
            it->show();
        } else QMessageBox::warning(0, "error", "Склад не вибрано\\не створено");
    }
}

//menu del good(видалити товар)
void MainWindow::on_menu_settings_del_good_triggered()
{
    if (db_open())
    {
        if (ui->ckl_choose->currentText() != "") {
            _items * it = new _items(query, 1, ui->ckl_choose->currentText() );
            it->setModal(true);
            it->show();
        } else QMessageBox::warning(0, "error", "Склад не вибрано\\не створено");
    }
}

//menu delete storage (Склад)
void MainWindow::on_menu_settings_del_ckl_triggered()
{
    if (db_open())
    {
        add_storage * as = new add_storage(query, 1);
        as->setModal(true);
        as->show();
    }
}

// buuton end good
void MainWindow::on_btn_ends_goods_clicked()
{
    if (db_open()){
        QList<fields> list;
        //прохід по всьому списку
        QList<fields>::const_iterator i;
        for (i = tovar.begin(); i != tovar.end(); ++i)
            if (i->kg < 100 && i->storage == ui->ckl_choose->currentText())
                list.push_back(*i);
        write_to_table(list);
    }
}


//button left (Залишки)
void MainWindow::on_btn_left_goods_clicked()
{
    if (db_open()){
        QList<fields> list;
        //прохід по всьому списку
        QList<fields>::const_iterator i;
        for (i = tovar.begin(); i != tovar.end(); ++i)
            if (i->kg > 0 && i->storage == ui->ckl_choose->currentText())
                list.push_back(*i);
        write_to_table(list);
    }
}

// if search test changed
void MainWindow::on_search_textChanged(const QString &arg1)
{
    if (db_open())
    {
        QList<fields> list;
        //прохід по всьому списку
        QList<fields>::const_iterator i;
        for (i = tovar.begin(); i != tovar.end(); ++i)
            if (i->name.contains(arg1) || i->code.contains(arg1))
                list.push_back(*i);

        write_to_table(list);
    }
}



//menu close app
void MainWindow::on_menu_close_triggered()
{
    close();
}


// summary tovar change button
void MainWindow::on_btn_add_goods_clicked()
{
    if (db_open())
    {
        quantity_of_goods *qog = new quantity_of_goods(query, 0, ui->ckl_choose->currentText());
        qog->setModal(true);
        qog->show();
    }
}
// tovar - kg
void MainWindow::on_btn_wr_goods_clicked()
{
    if (db_open())
    {
        quantity_of_goods *qog = new quantity_of_goods(query, 1, ui->ckl_choose->currentText());
        qog->setModal(true);
        qog->show();
    }
}

//menu create db
void MainWindow::on_menu_create_triggered()
{
    // save dialog
    QString path_to_db = QFileDialog::getSaveFileName(nullptr, "Crete file", "D:/", "Бази даних (*.db)" );

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_to_db);

    if (db_open())
    {
        query = QSqlQuery(db);
        query.exec("CREATE TABLE tovar ( id INTEGER PRIMARY KEY AUTOINCREMENT, name NVARCHAR(50), code NVARCHAR(20), kg varchar(5), storage NVARCHAR(50));");
        query.exec("CREATE TABLE storage ( id INTEGER PRIMARY KEY AUTOINCREMENT, name NVARCHAR(50))");
    }
}

void MainWindow::on_menu_about_triggered()
{
    QMessageBox::information(0, "Про розробника", "Програму створив студент групи КІ-406 Онуферко Віталій\nДля курсового проекту");
}

// menu help
void MainWindow::on_menu_help_triggered()
{
    Help *h = new Help();
    h->setModal(true);
    h->show();
}
