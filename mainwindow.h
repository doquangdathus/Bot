#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QtCore>
#include <QMainWindow>
#include <QSqlDatabase>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDatabase(QSqlDatabase *dbOut);
    int max(int a, int b);
    int samechar(QString s1, QString s2);
    int longestString(QStringList a, QStringList b, int m, int n);
    int handleUserQuestion();

private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    QSqlDatabase *db;
};

#endif // MAINWINDOW_H
