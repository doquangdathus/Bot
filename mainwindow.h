#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtGui>
#include <QtCore>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QKeyEvent>
#include <QTextEdit>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlError>
#include <QHash>
#include <QSqlTableModel>

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
    int handleUserQuestion(QString userQuestion);
    QString getUserName(QString userInput);
    int getUserClass(QString userInput);
    int getUserMajor();
    void listQuestion();
    QStringList listMajor();
    QString converseToDownCaste(QString str);
    void viewQuery(QString query);
    QString processDayInWeek(QString userInput);
    QString processSubjectName(QString userInput);
    QString processUserInput(QString userInput);

    void delay(int seconds);

    int confirm(QString userInput);

    void buildHashMajorClass();
    QHash<QString, QStringList> parameter(QString string);

    QString queryString(QHash<QString, QStringList> par);

    void getfirstSubjects(); // danh sách các môn học ban đầu
    void getcurrentSubjects(); // các môn học đến thời điểm hiện tại

    void viewSubjects(QStringList listSubjectsId);
    void deleteSubjects(QString sql); //xóa môn học
    void addSubjects(QString sql); // thêm môn học
    void removeSubjectsNotAdd(QString idSubjects);

    void setFirstSheduleModel();
    void setCurrenSheduleModel();
    void setUserQueryModel();
    void setUserFreeTime(QStringList ls);

    QStringList getUserFreeTime();
    QString getListIdInString(QStringList list);
    QString generateQueryString(QString condition);

    bool checkToAdd(QString newid); // Kiểm tra xem lịch có bị trùng, đè hay không khi thêm vào
    QString getSqlResult(QString sql); // Trả về kết quả của 1 câu lệnh truy vấn

    QString changeMarkPrinciple(QString message);
    QString summary();
    bool saveResult();
    QStringList warningWrongMajor(QStringList userIdSubject);
    QString warningNumberCredit(QStringList userIdSubject);

private slots:
    void process();
    void processUpPress();
    void processDownPress();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *model;

    QSqlDatabase db;
    int currentProcess;
    QString userName;
    QString userClass;
    QString userMajor;
    QStringList majors;
    QStringList questionsProcessId;
    QStringList questions;
    QHash<QString, QString> hashMajorClass;
    QStringList userSubjects; // mang danh sach
    QStringList removeSubjects; // môn học xóa ra
    QStringList addListSubjects; // môn học thêm vào
    QStringList subjects; // tất cả các môn học tới thời điểm hiện tại

    QStringList oldMark;
    QStringList newMark;

    int lastIndexUserMessage;
    int currIndexUserMessage;

    bool change;
};

#endif // MAINWINDOW_H
