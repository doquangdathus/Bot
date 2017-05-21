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
void MainWindow::setDatabase(QSqlDatabase *dbOut){
    db = dbOut;
}
int MainWindow::max(int a, int b){
    return (a>b)? a:b;
}

int MainWindow::samechar(QString s1, QString s2){
    QStringList str1 = s1.split(" ");
    QStringList str2 = s2.split(" ");
    int len1 = str1.length();
    int len2 = str2.length();
    return longestString(str1,str2,len1,len2);
}
int MainWindow::longestString(QStringList a, QStringList b, int m, int n){
    int longestSub[m+1][n+1];
    int longest = 0;
    for (int i=0; i<=m; i++){
        for(int j = 0; j<=n; j++){
            if(i==0||j==0)
                longestSub[i][j]=0;
            else if (a[i-1]==b[j-1]){
                longestSub[i][j]= longestSub[i-1][j-1] +1;
                longest = max(longest, longestSub[i][j]);
            }
            else
                longestSub[i][j]=0;
        }
    }
    return longest;
}
int MainWindow::handleUserQuestion(){
    return 0;
}

void MainWindow::on_pushButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    ui->textEdit->setText("");

    if(message.length() > 0){
        QStringList list = model->stringList();
        list << "YOU: " + message;

        if(message.compare("Xin chao") || message.compare("Hi")){
            list << "Bot: Xin Chao";
        }
         model->setStringList(list);
    }
}
