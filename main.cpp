#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setPort(3305);
    db.setHostName("127.0.0.1");
    db.setDatabaseName("Bot");
    db.setUserName("chatbot");
    db.setPassword("123456");

    if(db.open()){
        QSqlQuery q(db);
        if(q.exec("select * from LichHoc where 1")){
            while(q.next()){
                qDebug() << q.value(2).toString();
            }
        } else {
            qDebug() << db.lastError().text();
        }
    } else {
        qDebug() << db.lastError().text();
    }
    return a.exec();
}
