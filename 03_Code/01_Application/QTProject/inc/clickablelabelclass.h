#ifndef CLICKABLELABELCLASS_H
#define CLICKABLELABELCLASS_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class ClickableLabelClass : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableLabelClass(QWidget *parent = nullptr);
    ~ClickableLabelClass();

signals:
    void onClicked();   // custom signal

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLELABELCLASS_H
