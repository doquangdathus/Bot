#ifndef CUSTOMQTEXTEDIT_H
#define CUSTOMQTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QDebug>
#include <mainwindow.h>
class CustomQTextEdit : public
       QTextEdit
{
    Q_OBJECT
public:
    CustomQTextEdit(QWidget *parent = 0);
signals:
    void enterPress();
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // CUSTOMQTEXTEDIT_H
