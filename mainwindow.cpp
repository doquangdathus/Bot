#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    QStringList list;
    list << "Bot: Chào mừng đến với chatbot hỗ trợ đăng ký môn học :))";
    model->setStringList(list);
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonSend_clicked()
{
    QString message = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
//    QTextStream(stdout) << message << endl;

    if(message.length() > 0){
        int row = model->rowCount();
        model->insertRows(row, 1);
        QModelIndex index = model->index(row);
        QStringList list = model->stringList();
        list << "YOU: " + message;

        if(message.compare("Xin chao") || message.compare("Hi")){
            list << "Bot: Xin Chao";
        }
         model->setStringList(list);
    }
}
