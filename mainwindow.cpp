#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(enterPress()), this, SLOT(process()));
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
void MainWindow::process(){
    QString message = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
    QStringList list = model->stringList();
    list << "YOU: " + message;
    model->setStringList(list);
    if(message.length() > 0){

        if(currentProcess == 10){
            userName = getUserName(message);
            if(userName.length() == 0){
                list << "Bot: Có phải bạn không viết hoa tên của mình không? Tôi không thể nhận ra được tên của bạn, hãy viết hoa tên của mình: ví dụ như Quang Đạt :))";
            } else {
                currentProcess = 11;
                list << "Bot: Chào " + userName + ", rất vui được làm quen với bạn!!";
                list << "Bot: Thế bạn đang học ngành nào trong khoa Toán-Cơ-Tin của trường vậy?";
            }

        } else if(currentProcess == 11){
              if(message.toLower().contains("đúng")||message.toLower().contains("chính xác")){
                  currentProcess = 12;
                  list << "Bot: Vậy là bạn học khoa: " + userMajor;
                  if(userMajor.contains("MT&KHTT")){
                      list << "Bot: Lớp MT&KHTT có 2 lớp, là A2 và A3, bạn học lớp nào thế? Có phải K59A2 không?";
                      currentProcess = 121;
                  } else {
                      list << "Bot:Thế bạn là K59 phải không?";
                      currentProcess = 122;
                  }
              } else {
                  QString tempMess = message.toLower();
                  QString listMajorsInString;
                  qDebug() << tempMess;
                  for(int i = 0; i < majors.length(); i++){
                      QString major = majors.at(i);
                      qDebug() << major.toLower();
                      listMajorsInString += major + ", ";
                      if(tempMess.contains(major.toLower())){
                          userMajor = majors.at(i);
                          break;
                      }
                  }
                  if(userMajor.length() == 0){
                      list << "Bot: Chuyên ngành của bạn hiện không có trong lịch học? Cac nganh hoc hien tai co trong lich hoc la: " + listMajorsInString;
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

        } else if(currentProcess == 121){
            if(message.contains("đúng")){
                userClass = "K59A2";
                list << "Bot: 0.0, tôi cũng đã giúp đỡ rất nhiều bạn ở K59A2";
                list << "Bot: Bạn cần tôi tư vấn gì về lịch học nào?";
                currentProcess = 13;
            } else {
                currentProcess = 12;
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
        }else if(currentProcess == 122){
            if(message.contains("đúng")){
                userClass = "K59";
                list << "Bot: 0.0, tôi cũng đã giúp đỡ rất nhiều các bạn K59";
                list << "Bot: Bạn cần tôi tư vấn gì về lịch học nào?";
                currentProcess = 13;
            } else {
                currentProcess = 12;
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
        }
        else if(currentProcess == 13){
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
    delay(1);
    model->setStringList(list);
}

QString MainWindow::getUserName(QString userInput){
    QRegExp rx("^[A-Z]|Đ");
    QString name;
    QStringList ls = userInput.split(" ");
    for(int i = 0; i < ls.size(); i++){
        if(rx.indexIn(ls.at(i)) != -1){
            name += ls.at(i) + " ";
        }
    }
    name = name.trimmed();
    if(name.split(" ").length() == 1){
        return name;
    } else if(name.split(" ").length() == 2 && !name.contains("Anh")){
        return name.split(" ").at(1);
    } else if(name.split(" ").length() == 2 && !name.contains("Anh")){
        return name;
    } else if(name.contains("Anh")){
        QStringList ls = name.split(" ");
        name = ls.at(ls.length() - 2) + " " + ls.at(ls.length() - 1);
        return name;
    } else {
        return name;
    }
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

    userInput.replace("số tc", "số tín chỉ");
    userInput.replace("tc", "số tín chỉ");
}

void MainWindow::delay(int seconds)
{
    QTime dieTime= QTime::currentTime().addSecs(seconds);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_Return){
        qDebug()<< "Enter";
    } else {

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    userName = "";
    userClass = "";
    userMajor = "";

    currentProcess = 10;
    QStringList ls = model->stringList();
    ls = QStringList();
    ls << "Bot: Chào mừng đến với chatbot hỗ trợ đăng ký môn học :))";
    ls << "Bot: Hãy cho tôi biết tên được không?";
    model->setStringList(ls);
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this, "Hướng dẫn import file lịch học vào CSDL", "B1: chuyển lịch học sang dạng file .csv \n B2: Chọn file .csv vừa chuyển từ hệ thống, sau khi xong bước này coi như là hoàn thành");
    QSqlQuery q(db);
    QString filePath = QFileDialog::getOpenFileName(this);
    if(filePath.trimmed().length() == 0 || !filePath.contains(".csv")){
        QMessageBox::information(this, "Thất bại", "Quá trình import lịch học đã bị huỷ do bạn chưa chọn đúng file theo yêu cầu");
    } else {
        q.exec("DELETE FROM `LichHoc` WHERE 1");
        q.exec("load data infile '" + filePath + "' into table LichHoc fields terminated by ',' enclosed by '\"' lines terminated by '\n' (LichHoc.MMH, LichHoc.TMH, LichHoc.SoTC, LichHoc.Khoa, LichHoc.Nganh, LichHoc.Lop, LichHoc.SoSV, LichHoc.KhoaTrucThuoc, LichHoc.MaLopMH, LichHoc.Thu, LichHoc.Tiet, LichHoc.SoCho, LichHoc.GiangDuong)");
        if(q.lastError().text().trimmed().length() == 0){
            QMessageBox::information(this, "Thành công", "Bạn đã thêm thành công thời khoá biểu mới vào CSDL");
        } else {
            QMessageBox::information(this, "Lỗi", q.lastError().text());
        }
    }
}
