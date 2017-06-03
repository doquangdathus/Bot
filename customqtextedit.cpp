#include "customqtextedit.h"

CustomQTextEdit::CustomQTextEdit(QWidget *parent)
{

}

void CustomQTextEdit::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return){
        emit enterPress();
    } else {
        return QTextEdit::keyPressEvent(event);
    }
}
