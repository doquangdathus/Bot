#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->textEdit, SIGNAL(enterPress()), this, SLOT(process()));
    connect(ui->textEdit, SIGNAL(upPress()), this, SLOT(processUpPress()));
    connect(ui->textEdit, SIGNAL(downPress()), this, SLOT(processDownPress()));
    model = new QStringListModel(this);
    QStringList list;
    list << "Bot: Chào mừng đến với chatbot hỗ trợ đăng ký môn học :))";
    list << "Bot: Hãy cho tôi biết tên được không?";
    userClass = "K59A2";
    userMajor = "MT&KHTT";
    currentProcess = 10;

    majors = listMajor();
    listQuestion();
    buildHashMajorClass();
    model->setStringList(list);
    ui->listView->setModel(model);

    oldMark << "òa" << "óa" << "ỏa" << "õa" << "ọa" << "òe" << "óe" << "ỏe" << "õe" << "ọe" << "ùy" << "úy" << "ủy" << "ũy" << "ụy";
    newMark << "oà" << "oá" << "oả" << "oã" << "oạ" << "oè" << "oé" << "oẻ" << "oẽ" << "oẹ" << "uỳ" << "uý" << "uỷ" << "uỹ" << "uỵ";
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
    qDebug() << maxLength << " " << iMax << " " << questions.at(iMax) << " " << questionsProcessId.at(iMax);
    return iMax;
}
//Thay the cho viec bam nut
void MainWindow::process(){
    QString message = ui->textEdit->toPlainText();
    ui->textEdit->setText("");
    QStringList list = model->stringList();
    list << "YOU: " + message;
    lastIndexUserMessage ++;
    currIndexUserMessage ++;
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
              if(confirm(message) == 1){
                  list << "Bot: Vậy là bạn học khoa: " + userMajor;
                  QString clas = hashMajorClass.value(userMajor);
                  list << "Bot: Lớp của bạn là K59" + clas + " " + userMajor + "?";
                  currentProcess = 12;
                  userClass = "K59" + clas;
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
                  if(userMajor.length() == 0 && confirm(message) == 0){
                      list << "Bot: Chuyên ngành của bạn hiện không có trong lịch học? Cac nganh hoc hien tai co trong lich hoc la: " + listMajorsInString;
                  } else if(userMajor.length() == 0 && confirm(message) == -1){
                      list << "Bot: Thế chuyên ngành của bạn là gì nào?";
                  } else {
                      list << "Bot: Thế chuyên ngành của bạn là " + userMajor + " có phải không?";
                  }
              }
        } else if(currentProcess == 12){
            if(confirm(message) == 1){
                currentProcess = 13;
                list << "Bot: Vậy bạn là: " + userName + " học tại lớp: " + userClass + " " + userMajor;
                list << "Bot: Thế bây giờ bạn muốn chúng tôi tư vấn cho bạn vấn đề gì nào?";
                qDebug() << userMajor;
            } else {
                if(getUserClass(message) == -1 && confirm(message) == 0){
                    list << "Bot: Có vẻ như bạn đã nhập sai lớp? Hiện tại trong thời khoá biểu có các lớp của K57-K61";
                } else {
                    int pos = getUserClass(message);
                    if(pos == -1 && confirm(message) == -1){
                        list << "Bot: Vậy hãy cho tôi biết chính xác lớp mà bạn đang học?";
                    } else {
                        userClass = message.split(" ").at(pos);
                        list << "Bot: Lớp của bạn có phải là " + userClass + " " + userMajor + "?";
                    }

                }
            }

        } else if(currentProcess == 13){
                int pos = handleUserQuestion(message);
                qDebug() << pos;
                if(pos == -1){
                    list <<  "Bot: Tôi chưa xác định được câu trả lời, bạn có câu hỏi khác không?";
                } else {
                   QString idInString = questionsProcessId.at(pos);
                   int id = idInString.toInt();

                   switch (id) {
                   //xem lich hoc tieu chuan
                   case 14:{
                       getfirstSubjects();
                       setFirstSheduleModel();
                       setCurrenSheduleModel();
                       setUserFreeTime(getUserFreeTime());
                       list << "Bot: Bên trái màn hình là lịch học của bạn theo nhà trường";
                       list << "Bot: Dưới đây là một số tổng hợp của tôi về lịch học của bạn: ";
                       list << "Bot: " + summary();
                       list << "Bot: Bạn có muốn thay đổi gì trong thời khoá biểu của mình không?";
                       break;
                   }

                   //xem mon bat ki
                   case 4:{
                       QString sql = generateQueryString(queryString(parameter(message)));
                       QSqlQuery q(db);
                       if(q.exec(sql)){
                           QSqlQueryModel *userQueryModel = new QSqlQueryModel();
                           userQueryModel->setQuery(q);
                           ui->tableViewQuery->setModel(userQueryModel);
                       }
                       break;
                   }
                   //them mot mon nao do
                   case 3: {
                       QString sql = generateQueryString(queryString(parameter(message)));
                       QSqlQuery q(db);
                       qDebug() << sql;
                       if(q.exec(sql)){
                           QSqlQueryModel *userQueryModel = new QSqlQueryModel();
                           userQueryModel->setQuery(q);
                           ui->tableViewQuery->setModel(userQueryModel);
                           addSubjects(sql);
                           setCurrenSheduleModel();
                           setUserFreeTime(getUserFreeTime());
                       }
                       break;
                   }
                   //bo mot mon nao do
                   case 5: {
                       QString sql = generateQueryString(queryString(parameter(message)));
                       QSqlQuery q(db);
                       qDebug() << sql;
                       if(q.exec(sql)){
                           QSqlQueryModel *userQueryModel = new QSqlQueryModel();
                           userQueryModel->setQuery(q);
                           ui->tableViewQuery->setModel(userQueryModel);
                           deleteSubjects(sql);
                           setCurrenSheduleModel();
                           setUserFreeTime(getUserFreeTime());
                       }
                       break;
                   }
                   //danh gia lich hoc
                   default:
                       break;
                   }
                }

        } else if(currentProcess == 99){
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
    ui->listView->scrollToBottom();
}

void MainWindow::processUpPress()
{
    if(currIndexUserMessage > 0){
        currIndexUserMessage --;
        QStringList ls = model->stringList();
        int k = lastIndexUserMessage;
        for(int i = ls.size() - 1; i >= 0; i --){
            QString mess = ls.at(i);
            if(mess.contains("YOU:")){
                k--;
                if(k == currIndexUserMessage) if(k == currIndexUserMessage) ui->textEdit->setText(mess.replace("YOU: ", ""));
            }
        }
    }else {
        ui->textEdit->setText("");
    }
}

void MainWindow::processDownPress()
{
    if(currIndexUserMessage < lastIndexUserMessage){
        currIndexUserMessage ++;
        QStringList ls = model->stringList();
        int k = lastIndexUserMessage;
        for(int i = ls.size() - 1; i >= 0; i --){
            QString mess = ls.at(i);
            if(mess.contains("YOU:")){
                k--;
                if(k == currIndexUserMessage) ui->textEdit->setText(mess.replace("YOU: ", ""));
            }
        }
    } else {
        ui->textEdit->setText("");
    }
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
    QRegExp rx("^K[5-6][0-9]A[1-3](S|T|C)?$");
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

//reset chat
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

//import TKB vao CSDL
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

int MainWindow::confirm(QString userInput){
    userInput = converseToDownCaste(userInput);
    if(userInput.contains("dung") | userInput.contains("uk")
            | userInput.contains("chuan") | userInput.contains("phai")
            |userInput.contains("chinh xac")){
        return 1;
    } else if(userInput.contains("sai roi") | userInput.contains("khong")){
        return -1;
    } else {
        return 0;
    }
}

void MainWindow::buildHashMajorClass(){
    QSqlQuery q(db);
    q.exec("SELECT DISTINCT LichHoc.Nganh, LichHoc.Lop FROM `LichHoc` WHERE 1");
    while(q.next()){
        QString mj = q.value(0).toString();
        QString clas = q.value(1).toString();
        if(!clas.contains(",")){
            hashMajorClass.insert(mj, clas);
        }
    }
}
QHash<QString, QStringList> MainWindow::parameter(QString string){
    string = changeMarkPrinciple(string);
    QSet<QString> monhoc;
    QSet<QString> tenmon;
    QSet<QString> sotin;
    QSet<QString> khoahoc;
    QSet<QString> nganhhoc;
    QSet<QString> lophoc;
    QSet<QString> sosinhvien;
    QSet<QString> khoaquanly;
    QSet<QString> malopmon;
    QSet<QString> ngayhoc;
    QSet<QString> tiethoc;
    QSet<QString> solgcho;
    QSet<QString> tengiangduong;

    QHash<QString, QStringList> para;
    QSqlQuery query(db);
    query.exec("Select * from LichHoc");
    while(query.next()){
        monhoc << query.value(1).toString();
        tenmon << query.value(2).toString();
        sotin << query.value(3).toString();
        khoahoc << query.value(4).toString();
        nganhhoc << query.value(5).toString();
        if(query.value(6).toString().length()>0)
            lophoc << query.value(6).toString();
        sosinhvien << query.value(7).toString();
        khoaquanly << query.value(8).toString();
        malopmon << query.value(9).toString();
        ngayhoc << query.value(10).toString();
        tiethoc << query.value(11).toString();
        solgcho << query.value(12).toString();
        tengiangduong << query.value(13).toString();
    }
    QStringList list = string.split(" ");
    QStringList mh;
    QStringList tm;
    QStringList k;
    QStringList nganh;
    QStringList lp;
    QStringList khoaql;
    QStringList mlm;
    QStringList tiet;
    QStringList gd;
    QStringList tc;
    QStringList sosv;
    QStringList thu;
    QStringList socho;

    for (QString s: monhoc){
        if(string.contains(s)){
            mh << s;
        }
    }
    for (QString s: tenmon){
        if(string.contains(s)){
            tm << s;
        }
    }
    for (QString s: khoahoc){
        if(string.contains(s)){
            k << s;
        }
    }
    for (QString s: nganhhoc){
        if(string.contains(s)){
            nganh << s;
        }
    }
    for (QString s: lophoc){
        if(string.contains(s)){
            lp << s;
        }
    }
    for (QString s: khoaquanly){
        if(string.contains(s)){
            khoaql << s;
        }
    }
    for (QString s: malopmon){
        if(string.contains(s)){
            mlm << s;
        }
    }
    for (QString s: tiethoc){
        if(string.contains(s)){
            tiet << s;
        }
    }
    for (QString s: tengiangduong){
        if(string.contains(s)){
            gd << s;
        }
    }

    for (QString s : list){
        if (s=="thứ"){
            int i = list.indexOf(s);
            thu << list.at(i+1);
        }
        if (s=="sotc"){
            int i = list.indexOf(s);
            tc << list.at(i+1);
        }
        if (s=="sosv"){
            int i = list.indexOf(s);
            sosv << list.at(i+1);
        }
        if(s=="socho"){
            int i = list.indexOf(s);
            socho << list.at(i+1);
        }
    }
    if(mh.length()>0){
        para.insert("MMH", mh);
    }
    if(tm.length()>0){
        para.insert("TMH", tm);
    }
    qDebug() << para.value("TMH");
    if(k.length()>0){
        para.insert("Khoa", k);
    }
    if(nganh.length()>0){
        para.insert("Nganh", nganh);
    }
    if(lp.length()>0){
        para.insert("Lop", lp);
    }
    if(khoaql.length()>0){
        para.insert("KhoaTrucThuoc", khoaql);
    }
    if(mlm.length()>0){
        para.insert("MaLopMH", mlm);
    }
    if(tiet.length()>0){
        para.insert("Tiet", tiet);
    }
    if(gd.length()>0){
        para.insert("GiangDuong", gd);
    }
    if(tc.length()>0){
        para.insert("SoTC", tc);
    }
    if(sosv.length()>0){
        para.insert("SoSV", sosv);
    }
    if(thu.length()>0){
        para.insert("Thu", thu);
    }
    if(socho.length()>0){
        para.insert("SoCho", socho);
    }
    return para;
}
QString MainWindow::queryString(QHash<QString, QStringList> par){
    QStringList querylist;
    for (QString s : par.keys()){
        QString list;
        for (int i = 0; i< par.value(s).length(); i++){
            if(i == par.value(s).length()-1)
                list += "LichHoc."+s+" LIKE '%"+par.value(s).at(i)+"%'";
            else{
                list +=  "LichHoc."+s+" LIKE '%"+par.value(s).at(i)+"%' OR ";
            }
        }
        querylist << "("+list+")";
    }
    QString query = "";
    for(int i = 0; i<querylist.length(); i++){
        if(i == querylist.length()-1){
            query += querylist.at(i);
        }
        else
            query += querylist.at(i)+" AND ";
    }
    return query;
}
void MainWindow::getfirstSubjects(){
    // lấy ra tất cả các môn học ban đầu
    QString sql ="";
    QString tempVar = userClass;
    QString clas = userClass.mid(3, userClass.length() - 3);
    QString k = userClass.mid(1, 2);
    sql = "SELECT * FROM LichHoc WHERE LichHoc.Lop like '"+clas+"' AND LichHoc.Khoa = " + k ;

    QSqlQuery q(db);
    if(q.exec(sql)){
        while(q.next()){
            userSubjects << q.value(0).toString();
        }
    }
}

void MainWindow::getcurrentSubjects(){

    // for trong cái cần xóa removeSubject để remove userSubject
    // for trong cái cần thêm addSubject để remove userSubject

    QString id = "(";
    for(int i = 0; i<userSubjects.length()-1; i++){
        id.append(userSubjects.at(i));
        id.append(",");
    }
    id.append(userSubjects.at(userSubjects.length()-1));
    id.append(")");
    qDebug() << id;

    QString sql = "SELECT * FROM LichHoc WHERE LichHoc.ID in ";
    sql.append(id);
    viewQuery(sql);

}
void MainWindow::deleteSubjects(QString sql){
    qDebug() <<sql;
    QStringList temp;
    QSqlQuery q(db);
    if(q.exec(sql)){
        while(q.next()){
            temp << q.value(0).toString();
        }
    }

    for(int i = 0; i<temp.length();i++){
        userSubjects.removeOne(temp.at(i));
    }

}
void MainWindow::addSubjects(QString sql){
    QStringList temp;
    QSqlQuery q(db);
    if(q.exec(sql)){
        while(q.next()){
            temp << q.value(0).toString();
        }
    }
    QStringList repeatedSubjectId;
    for(int i = 0; i<temp.length();i++){
        if(!userSubjects.contains(temp.at(i)) && checkToAdd(temp.at(i))){
            userSubjects << temp.at(i);
        } else {
            repeatedSubjectId << temp.at(i);
            QMessageBox::information(this, "Không thể thêm môn học", "Môn học đã có trong danh sách hoặc trùng lịch học " + temp.at(i));
        }
    }

    foreach (QString s, repeatedSubjectId) {
        removeSubjectsNotAdd(s);
    }
}

void MainWindow::removeSubjectsNotAdd(QString idSubjects)
{
    QSqlQuery q(db);
    q.exec("Select Distinct LichHoc.MMH from LichHoc where LichHoc.ID = " + idSubjects);
    QString mmh;
    while(q.next())
        mmh = q.value(0).toString();
    qDebug() << mmh;
    QStringList copyUserSubjects;

    foreach (QString s, userSubjects) {
        copyUserSubjects << s;
    }

    foreach (QString s, copyUserSubjects) {
        q.exec("Select Distinct LichHoc.MMh from LichHoc where LichHoc.ID = " + s);
        QString mmh2;
        while(q.next())
            mmh2 = q.value(0).toString();
        if( mmh2.compare(mmh)== 0){
            userSubjects.removeOne(s);
        }
    }
}
void MainWindow::viewSubjects(QStringList listSubjectsId){
    QSqlTableModel tableModel;
    tableModel.setTable("Thời khoá biểu");
    tableModel.setHeaderData(0, Qt::Horizontal, QObject::tr("Thứ 2"));
    tableModel.setHeaderData(1, Qt::Horizontal, QObject::tr("Thứ 3"));
    tableModel.setHeaderData(2, Qt::Horizontal, QObject::tr("Thứ 4"));
    tableModel.setHeaderData(3, Qt::Horizontal, QObject::tr("Thứ 5"));
    tableModel.setHeaderData(4, Qt::Horizontal, QObject::tr("Thứ 6"));
    tableModel.setHeaderData(5, Qt::Horizontal, QObject::tr("Thứ 7"));
    tableModel.setHeaderData(6, Qt::Horizontal, QObject::tr("Chủ Nhật"));

    QTableView *tableView = new QTableView;
    tableView->setModel(&tableModel);

    tableView->show();
}

void MainWindow::setFirstSheduleModel(){
    QSqlQueryModel *firstSheduleModel = new QSqlQueryModel;
    QString sql = "SELECT * FROM LichHoc WHERE LichHoc.ID in ";
    sql.append(getListIdInString(userSubjects));
    QSqlQuery query(db);
    query.exec(sql);
    firstSheduleModel->setQuery(query);
    ui->tableViewFirstShedule->setModel(firstSheduleModel);
}

void MainWindow::setCurrenSheduleModel()
{
    QSqlQueryModel *firstSheduleModel = new QSqlQueryModel;
    QString sql = "SELECT * FROM LichHoc WHERE LichHoc.ID in ";
    sql.append(getListIdInString(userSubjects));
    QSqlQuery query(db);
    query.exec(sql);
    firstSheduleModel->setQuery(query);
    ui->tableViewCurrenShedule->setModel(firstSheduleModel);
}

QStringList MainWindow::getUserFreeTime()
{
    QStringList freeTime;
    QSqlQuery q(db);
    if(q.exec("SELECT LichHoc.Thu, LichHoc.Tiet FROM LichHoc WHERE LichHoc.ID in " + getListIdInString(userSubjects))){
        int shedule[9][11];
        while(q.next()){
            QString day = q.value(0).toString();
            QString hour = q.value(1).toString();

            if(!day.contains("x") && !hour.contains("Tự chọn")){
                int row = day.toInt();
                QStringList hs = hour.split("-");
                QString beginString = hs.at(0);
                QString endString = hs.at(1);
                int begin = beginString.toInt();
                int end = endString.toInt();

                for(int i = begin; i <= end; i++){
                    shedule[row][i] = 1;
                }
            }
        }

        for(int i = 2; i <= 8; i++){

            QString d;
            d.append("Thứ ");
            d.append(QString::number(i));
            d.append(": ");
            for(int j = 1; j <= 10; j++){
                if(shedule[i][j] != 1){
                    d.append(QString::number(j));
                    d.append(" ,");
                }
            }
            freeTime << d;
            freeTime << "";
        }

    } else {
        qDebug() << q.lastError().text();

    }
    return freeTime;
}

void MainWindow::setUserFreeTime(QStringList listFreeTime)
{
    QStringListModel *freeTimeModel = new QStringListModel;
    freeTimeModel->setStringList(listFreeTime);
    ui->listViewFreeTime->setModel(freeTimeModel);
}


QString MainWindow::getListIdInString(QStringList list){
    QString id = "(";
    for(int i = 0; i<list.length()-1; i++){
        id.append(list.at(i));
        id.append(",");
    }
    id.append(list.at(list.length()-1));
    id.append(")");
    return id;
}

QString MainWindow::generateQueryString(QString condition)
{
    QString sql;
    sql.append("select LichHoc.ID, LichHoc.MMH, LichHoc.TMH, LichHoc.SoTc, LichHoc.Khoa, LichHoc.Nganh, LichHoc.Lop, LichHoc.MaLopMH, LichHoc.Thu, LichHoc.Tiet, LichHoc.GiangDuong");
    sql.append(" from LichHoc");
    sql.append(" where ");
    sql.append(condition);
    return sql;
}

bool MainWindow::checkToAdd(QString newid){

    for(int j = 0; j < userSubjects.length(); j++){
        QString id = userSubjects.at(j);
        QString sql = "SELECT Thu FROM `LichHoc` WHERE id = ";

        if(getSqlResult(sql+newid) == getSqlResult(sql+id)){
            QString sql2 = "SELECT Tiet FROM `LichHoc` WHERE ID = ";
            QString tiet1 = getSqlResult(sql2+newid);
            if(tiet1 == "Tự chọn"){
                return true;
            }
            QString tiet2 = getSqlResult(sql2+id);
            if(tiet2 != "Tự chọn"){
                int s1 = tiet1.split("-")[0].toInt(); //thoi gian bat dau mon 1
                int f1 = tiet1.split("-")[1].toInt(); // thoi gian ket thuc mon 1
                int s2 = tiet2.split("-")[0].toInt(); // thoi gian bat dau mon 2
                int f2 = tiet2.split("-")[1].toInt(); // thoi gian ket thuc mon 2

                if(((s1<s2 & f1 <s2) == false) & ((s2 < s1 & f2 < s1) == false)){
                    qDebug() << "dm 1";
                    qDebug() << s1 <<"-"<< f1<<";" << s2<<"-"<<f2;
                    return false;
                }
            }
        }

    }
    return true;
}

QString MainWindow::getSqlResult(QString sql){
    QString result;
    QSqlQuery q(db);
    if(q.exec(sql)){
        while(q.next()){
            result = q.value(0).toString();
        }
    }
    return result;
}

QString MainWindow::changeMarkPrinciple(QString message)
{
    for(int i = 0; i < oldMark.length(); i++){
        message = message.replace(newMark.at(i), oldMark.at(i));
    }
    return message;
}

QString MainWindow::summary()
{
    QSqlQuery q(db);

    q.exec("select DISTINCT LichHoc.MMH, LichHoc.SoTc from LichHoc where LichHoc.ID in" + getListIdInString(userSubjects));

    int totalCredit = 0;
    int numberOfSubject = 0;

    while(q.next()){
        totalCredit += q.value(1).toString().toInt();
        numberOfSubject ++;
    }
    qDebug() << q.lastQuery();
    int freeHours = 0;
    int max = 0;
    int min = 12;
    QString dayMax;
    QString dayMin;
    QStringList ls = getUserFreeTime();
    for(int i = 0; i <= 8; i++){
        QString day = ls.at(i);
        if(day.length() > 0){
            QStringList hours = day.split(",");
            qDebug() << hours.length();
            freeHours += hours.length() - 1;
            if(max < 11 - hours.length()){
                dayMax = day.mid(0, 5);
                max = 11 - hours.length();
            }
            if(min > 11 - hours.length()){
                dayMin = day.mid(0, 5);
                min = 11 - hours.length();
            }
        }


    }

    QString s;
    s.append("Tổng số môn học trên trường là: " + QString::number(numberOfSubject) + "\n");
    s.append("với " + QString::number(totalCredit) + " tín chỉ \n");
    s.append("Thời gian rảnh rỗi(không học) trên trường là: " + QString::number(freeHours) + "\n");
    s.append("trong đó ngày nhiều nhất là: " + dayMax + " với " + QString::number(max) + " giờ \n");
    s.append("ngày ít nhất là: " + dayMin + " với " + QString::number(min) + " giờ \n");
    return s;
}
