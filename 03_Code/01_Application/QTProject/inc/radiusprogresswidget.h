#ifndef RADIUSPROGRESSWIDGET_H
#define RADIUSPROGRESSWIDGET_H

#include <QProgressBar>

class RadiusProgressWidget : public QProgressBar
{
    Q_OBJECT
public:
    RadiusProgressWidget(QWidget *parent = nullptr);

private:
    QColor m_backColor;
    QColor m_progressColor;
    QColor m_textColor;
    virtual void paintEvent(QPaintEvent *event) override final;
    void setBackColor(const QColor &backColor);
    void setProgressColor(const QColor &progressColor);
    void setTextColor(const QColor &textColor);
};

#endif // RADIUSPROGRESSWIDGET_H
