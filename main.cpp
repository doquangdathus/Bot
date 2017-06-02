#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>

QSqlDatabase db;
void create_connect(){
    QFile file("/home/zic/Documents/C++Code/ChatBot/.env");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << file.errorString();
        file.close();
    } else {
        QTextStream read(&file);
        QString port = read.readLine().split("=").at(1);
        QString url = read.readLine().split("=").at(1);
        QString db_name = read.readLine().split("=").at(1);
        QString db_user = read.readLine().split("=").at(1);
        QString db_pass = read.readLine().split("=").at(1);
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setPort(port.toInt());
        db.setHostName(url);
        db.setDatabaseName(db_name);
        db.setUserName(db_user);
        db.setPassword(db_pass);

        if(db.open()){
//            QSqlQuery q(db);
//            if(q.exec("select * from LichHoc where 1")){
//                while(q.next()){
//                    qDebug() << q.value(2).toString();
//                }
//               } else {
//                qDebug() << db.lastError().text();
//            }
            qDebug() << "Connect success!!";
        } else {
            qDebug() << db.lastError().text();
        }
    }

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    create_connect();
    MainWindow w;
    w.setDatabase(db);
    w.show();

    return a.exec();
}
