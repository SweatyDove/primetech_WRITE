#ifndef PUSHTODATABASE_H
#define PUSHTODATABASE_H

#include <QDialog>
#include <stdio.h>
#include <stdlib.h>
#include "/usr/include/postgresql/libpq-fe.h"
#include <QTextEdit>



#define CONN_LIMIT      40
#define FORMAT_LIMIT    100         // Максимальная длина строки
#define ROW_TEXT_MAX    40


void do_exit(PGconn *, PGresult *);
int ReadString(char *line, int limit);





namespace Ui {
class PushToDataBase;
}

class PushToDataBase : public QDialog
{
    Q_OBJECT

public:
    explicit PushToDataBase(QWidget *parent = nullptr);
    ~PushToDataBase();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PushToDataBase *ui;
};

#endif // PUSHTODATABASE_H
