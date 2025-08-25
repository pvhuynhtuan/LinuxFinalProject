#include "clickablelabelclass.h"

ClickableLabelClass::ClickableLabelClass(QWidget *parent) : QLabel(parent)
{

}

ClickableLabelClass::~ClickableLabelClass()
{

}

void ClickableLabelClass::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        emit onClicked();  // emit our custom signal
    }
    QLabel::mousePressEvent(event); // call base implementation
}
