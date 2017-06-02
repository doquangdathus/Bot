#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QtCore>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDatabase(QSqlDatabase dbOut);
    int max(int a, int b);
    int samechar(QString s1, QString s2);
    int longestString(QStringList a, QStringList b, int m, int n);
    int handleUserQuestion();
    int getUserName(QString userInput);
    int getUserClass(QString userInput);
    int getUserMajor();
    void listQuestion();
    QStringList listMajor();
    QString converseToDownCaste(QString str);

private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    QSqlDatabase db;
    int currentProcess;
    QString userName;
    QString userClass;
    QString userMajor;
    QStringList majors;
    QStringList questions;
};

#endif // MAINWINDOW_H
