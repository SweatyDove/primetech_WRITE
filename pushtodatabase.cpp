#include "pushtodatabase.h"
#include "ui_pushtodatabase.h"




PushToDataBase::PushToDataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PushToDataBase)
{
    ui->setupUi(this);
}

PushToDataBase::~PushToDataBase()
{
    delete ui;
}

void PushToDataBase::on_pushButton_clicked()
{

    extern PGconn *conn;
    extern QString gl__table_name;
    const char *paramValues[2];             // Параметры, к-е передаются в ком-ду PQexecParams()
    PGresult *res;

    int last_row_id = 0;                    // Id последней строки в таблице
    char row_num[ROW_TEXT_MAX];             // Для записи номера строки в текстовом виде (для передачи затем в *paramValues[0])
    char format_str[FORMAT_LIMIT];          // Вспомагательная строка формата
    int no_empty = 0;                       // Таблица не пустая (=1) или пустая (=0)

    // Записываем введённую строку
    QString new_string = ui->new_string->text();


    // Создаем таблицу если она ещё не существует
    sprintf(format_str, "CREATE TABLE IF NOT EXISTS %s(id INTEGER PRIMARY KEY, content VARCHAR(255))", gl__table_name.toStdString().c_str());
    res = PQexec(conn, format_str);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        ui->textBrowser->append(PQerrorMessage(conn));
    }
    PQclear(res);


    // Определяем, пустая ли таблица (запрос возвращает 1, если таблица не пуста; 0 в противном случае)
    sprintf(format_str, "SELECT count(*) FROM (SELECT 1 FROM %s LIMIT 1) AS t", gl__table_name.toStdString().c_str());
    res = PQexec(conn, format_str);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
        ui->textBrowser->append(PQerrorMessage(conn));
    sscanf(PQgetvalue(res, 0, 0), "%d", &no_empty);         // Запись рез-та запроса
    PQclear(res);



    // Находим Id последней строки в таблице string (если таблица не пустая)
    if (no_empty == 1) {
        sprintf(format_str, "SELECT id FROM %s ORDER BY id DESC LIMIT 1", gl__table_name.toStdString().c_str());
        res = PQexec(conn, format_str);
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
            ui->textBrowser->append(PQerrorMessage(conn));
        sscanf(PQgetvalue(res, 0, 0), "%d", &last_row_id);
    }
    // else { last_row_id = 0 by default }


    // Записываем строку в таблицу
    sprintf(row_num, "%d", last_row_id + 1);                // Записываем номер строки в тесктовом формате
    paramValues[0] = row_num;                               // Записываем 1-ый и 2-й параметры
    paramValues[1] = new_string.toStdString().c_str();      //  команды INSERT (так же в текстовом формате)
    sprintf(format_str, "INSERT INTO %s VALUES($1, $2)", gl__table_name.toStdString().c_str());
    res = PQexecParams(conn, format_str, 2, NULL, paramValues, NULL, NULL, 0);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
        ui->textBrowser->append(PQerrorMessage(conn));
    PQclear(res);
}
