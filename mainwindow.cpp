#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pushtodatabase.h"

PGconn *conn;                   // Указатель на соединение
QString gl__table_name;         // Костыль для передачи во 2-ю форму


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


// Кнопка авторизации
void MainWindow::on_pushButton_clicked()
{
    char conn_info[CONN_LIMIT];
    extern PGconn *conn;
    extern QString gl__table_name;

    // Записываем параметры авторизации в объекты типа QString
    QString user_name = ui->user_name->text();
    QString data_base_name = ui->data_base_name->text();
    QString table_name = ui->table_name->text();
    gl__table_name = table_name;

    // Connect to data base ("str_stock" for me)
    sprintf(conn_info, "user=%s dbname=%s", user_name.toStdString().c_str(), data_base_name.toStdString().c_str());
    conn = PQconnectdb(conn_info);


    // Ошибка соединения
    if (PQstatus(conn) == CONNECTION_BAD) {
        ui->statusbar->showMessage(PQerrorMessage(conn));
    }
    // Успешное соединение
    else {
        ui->statusbar->showMessage("Authorization is successful!");
        // Открытие второго окна
        PushToDataBase window;
        window.setModal(true);
        window.exec();
    }


}
