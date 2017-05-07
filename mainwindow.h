#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QtCore>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonSend_clicked();

    void on_pushButtonSend_pressed();

private:
    Ui::MainWindow *ui;
    QStringListModel *model;
};

#endif // MAINWINDOW_H
