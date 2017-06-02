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
    list << "Bot: Hãy cho tôi biết tên được không?";
    currentProcess = 10;
    majors = listMajor();
    listQuestion();
    model->setStringList(list);
    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setDatabase(QSqlDatabase dbOut){
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

int MainWindow::handleUserQuestion(QString userQuestion){
    QString userQuestionToAscii = converseToDownCaste(userQuestion);
    int iMax = -1;
    int maxLength = 0;
    for(int i = 0; i < questions.length(); i++){
        int sameChar = samechar(userQuestionToAscii, questions.at(i));
        if(sameChar > maxLength){
               maxLength = sameChar;
               iMax = i;
        }
    }
    qDebug() << maxLength << " " << iMax;
    return iMax;
}

void MainWindow::on_pushButton_clicked()
{
    QString message = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
    QStringList list = model->stringList();
    list << "YOU: " + message;
    if(message.length() > 0){

        if(currentProcess == 10){
            if(message.contains("đúng") || message.contains("chuẩn")){
                currentProcess = 11;
                list << "Bot: Xin chào " + userName;
                list  << "Bot: Bạn học khoa nào thế?";
            } else {
                QStringList ls = message.split(" ");
                userName = ls.at(ls.length() - 1);
                list << "Bot: Tên của bạn có phải là " + userName + " ?";
            }
        } else if(currentProcess == 11){
              if(message.toLower().contains("đúng")||message.toLower().contains("chính xác")){
                  currentProcess = 12;
                  list << "Bot: Vậy là bạn học khoa: " + userMajor;
                  if(userMajor.contains("MT&KHTT")){
                      list << "Bot: Lớp MT&KHTT có 2 lớp, là A2 và A3, bạn học lớp nào thế? Có phải K59A2 không?";
                  } else {
                      list << "Bot:Thế bạn là K59 phải không?";
                  }
              } else {

                  for(int i = 0; i < majors.length(); i++){
                      if(message.contains(majors.at(i))){
                          userMajor = majors.at(i);
                          break;
                      }
                  }

                  list << "Bot: Thế chuyên ngành của bạn là " + userMajor + " có phải không?";
              }
        } else if(currentProcess == 12){
            if(message.contains("đúng")| message.contains("chuẩn")){
                currentProcess = 13;
                list << "Bot: Vậy bạn là: " +userName + " học tại lớp: " + userClass + " " + userMajor;
                list << "Bot: Thế bây giờ bạn muốn chúng tôi tư vấn cho bạn vấn đề gì nào?";
            } else {
                if(getUserClass(message) == -1){
                    list << "Bot: Có vẻ như bạn đã nhập sai lớp? Hiện tại trong thời khoá biểu có các lớp của K57-K61";
                } else {
                    int pos = getUserClass(message);
                    QStringList l = message.split(" ");
                    if(userMajor.contains("MT&KHTT")){
                        userClass = l.at(pos).mid(0, 5);
                    } else {
                        userClass = l.at(pos).mid(0, 3);
                    }
                    list << "Bot: Lớp của bạn có phải là " + userClass;
                }
            }

        } else if(currentProcess == 13){
                int pos = handleUserQuestion(message);
                qDebug() << pos;
                if(pos == -1){
                    list <<  "Bot: Tôi chưa xác định được câu trả lời, bạn có câu hỏi khác không?";
                } else {
                  QString id = questionsProcessId.at(pos);
                  currentProcess = id.toInt();
                  QString userLop;
                  QString userK;
                  QString queryInString;
                  if(userClass.length() == 5){
                      userLop = userClass.mid(3, 2);
                      userK = userClass.mid(1,2);
                      queryInString = "select * from LichHoc where LichHoc.Nganh='" + userMajor
                              + "' && LichHoc.Lop='" + userLop + "' && LichHoc.Khoa='" + userK + "'";

                  } else {
                      userK = userClass.mid(1,2);
                      queryInString = "select * from LichHoc where LichHoc.Nganh='" + userMajor
                              + "' && LichHoc.Khoa='" + userK + "'";
                  }

                  viewQuery(queryInString);
                }

        } else if(currentProcess == 14){
            QString userLop;
            QString userK;
            QString queryInString;
            if(userClass.length() == 5){
                userLop = userClass.mid(3, 2);
                userK = userClass.mid(1,2);
                queryInString = "select * from LichHoc where LichHoc.Nganh='" + userMajor
                        + "' && LichHoc.Lop='" + userLop + "' && LichHoc.Khoa='" + userK + "'";

            } else {
                userK = userClass.mid(1,2);
                queryInString = "select * from LichHoc where LichHoc.Nganh='" + userMajor
                        + "' && LichHoc.Khoa='" + userK + "'";
            }

            viewQuery(queryInString);
        }
        else if(currentProcess == 99){
            if(message.toLower().contains("co")|| message.toLower().contains("yes")||message.toLower().contains("tiep tuc")){
                 // Goi tiep ham xu ly
             }
             else{
                 list << "Bot: Vay hen gap lai ban dip khac nhe";
             }
        }
    } else {
        list << "Bot: Tôi không hiểu bạn đang muốn nói gì?";
    }

    model->setStringList(list);
}
int MainWindow::getUserName(QString userInput){
    QRegExp rx("[^A-Za-z]");
    return rx.indexIn(userInput);
}
int MainWindow::getUserClass(QString userInput){
    QRegExp rx(".*K[5-6]");
    QStringList ls = userInput.split(" ");
    for(int i = 0; i < ls.size(); i++){
        if(rx.indexIn(ls.at(i)) != -1){
            return i;
        }
    }
    return -1;
}

void MainWindow::listQuestion(){
    QSqlQuery q(db);
    if(q.exec("select * from DSCauHoi")){
        while (q.next()) {
           QString question = q.value(1).toString();
           QString questionProcessId = q.value(2).toString();

           questions << question;
           questionsProcessId << questionProcessId;
        }
    }
}

QStringList MainWindow::listMajor(){
    QStringList list;
    QSqlQuery q(db);
    if(q.exec("SELECT DISTINCT LichHoc.Nganh FROM LichHoc")){
        while(q.next()){
            list << q.value(0).toString();
        }
    }
    return list;
}
QString MainWindow::converseToDownCaste(QString str){
    QRegExp rxa("à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ");
    QRegExp rxe("è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ");
    QRegExp rxi("ì|í|ị|ỉ|ĩ");
    QRegExp rxo("ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ");
    QRegExp rxu("ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ");
    QRegExp rxy("ỳ|ý|ỵ|ỷ|ỹ");
    QRegExp rxSpace("\\s{2,}");
    str = str.toLower();
    str = str.trimmed();
    str = str.replace(rxa,"a");
    str = str.replace(rxe,"e");
    str = str.replace(rxi,"i");
    str = str.replace(rxo,"o");
    str = str.replace(rxu,"u");
    str = str.replace(rxy,"y");
    str = str.replace("đ","d");
    str = str.replace(rxSpace," ");

    return str;
}
void MainWindow::viewQuery(QString q){

    QSqlQueryModel *model2 = new QSqlQueryModel();
    QSqlQuery query(db);
    query.exec(q);
    qDebug() << query.lastQuery();
    model2->setQuery(query);
    QTableView* tableView = new QTableView;
    tableView->setModel(model2);
    tableView->show();
}
QString MainWindow::processUserInput(QString userInput){
    userInput = processDayInWeek(userInput);
    userInput = processSubjectName(userInput);
    return userInput;
}

QString MainWindow::processDayInWeek(QString userInput){
    userInput.replace("thứ hai","thứ 2");
    userInput.replace("thứ ba", "thứ 3");
    userInput.replace("thứ tư", "thứ 4");
    userInput.replace("thứ năm", "thứ 5");
    userInput.replace("thứ sáu", "thứ 6");
    userInput.replace("thứ bảy", "thứ 7");

    userInput.replace("Thứ hai","thứ 2");
    userInput.replace("Thứ ba", "thứ 3");
    userInput.replace("Thứ tư", "thứ 4");
    userInput.replace("Thứ năm", "thứ 5");
    userInput.replace("Thứ sáu", "thứ 6");
    userInput.replace("Thứ bảy", "thứ 7");

    return userInput;
}

QString MainWindow::processSubjectName(QString userInput){
    //Chuyen tu viet tat -> sang viet chuan CSDL
    userInput.replace("gt1",  "Giải Tích 1");
    userInput.replace("gt2",  "Giải Tích 2");
    userInput.replace("gt3",  "Giải Tích 3");

    userInput.replace("pttknc", "Phân tích thống kê nhiều chiều");
    userInput.replace("PTTKNC", "Phân tích thống kê nhiều chiều");

    userInput.replace("tkdgtt", "Thiết kế đánh giá thuật toán");
    userInput.replace("TKDGTT", "Thiết kế đánh giá thuật toán");

    userInput.replace("ktmt", "Kiến trúc máy tính");
    userInput.replace("KTMT", "Kiến trúc máy tính");

    userInput.replace("đstt", "Đại số tuyến tính");
    userInput.replace("ĐSTT", "Đại số tuyến tính");
}

