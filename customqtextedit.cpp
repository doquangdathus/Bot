#include "customqtextedit.h"

CustomQTextEdit::CustomQTextEdit(QWidget *parent)
{

}

void CustomQTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return){
        emit enterPress();
    } else if(event->key() == Qt::Key_Up){
        emit upPress();
    } else if(event->key() == Qt::Key_Down){
        emit downPress();
    }
    else {
        return QTextEdit::keyPressEvent(event);
    }
}
