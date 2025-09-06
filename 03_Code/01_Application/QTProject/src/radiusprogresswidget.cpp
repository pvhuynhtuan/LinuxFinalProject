#include "radiusprogresswidget.h"
#include <QPainter>

#define RADIUS_PROGRESS_TEXT_WIDTH      30

RadiusProgressWidget::RadiusProgressWidget(QWidget *parent)
    : QProgressBar(parent),
    m_backColor(Qt::white),
    m_progressColor(29, 157, 255),
    m_textColor(255, 255, 0)
{
    setMinimum(0);
    setMaximum(100);
    setValue(0);
}

void RadiusProgressWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    const QRect &rect = QRect(0, 0, width() - RADIUS_PROGRESS_TEXT_WIDTH - 1, height());
    const QRect &textRect = QRect(width() - RADIUS_PROGRESS_TEXT_WIDTH, 0, RADIUS_PROGRESS_TEXT_WIDTH, height());

    const double k = (value() - minimum()) * 1.0 / (maximum() - minimum());
    const int x = (int)(rect.width() * k);
    const QRect &fillRect = rect.adjusted(0, 0, x - rect.width(), 0);

    // Draw the back round rectangle
    painter.setRenderHint(QPainter::Antialiasing); // smooth edges
    painter.setPen(Qt::NoPen); // no outline
    painter.setBrush(QBrush(m_backColor)); // fill

    painter.drawRoundedRect(rect, height() / 2, height() / 2);

    // Draw the progress round rectangle
    painter.setRenderHint(QPainter::Antialiasing); // smooth edges
    painter.setPen(Qt::NoPen); // no outline
    painter.setBrush(QBrush(m_progressColor)); // fill
    painter.drawRoundedRect(fillRect, height() / 2, height() / 2);

    setFont(QFont("Segoe UI", 7, QFont::Bold));
    painter.setPen(m_textColor);
    painter.drawText(textRect, Qt::AlignCenter, QString("%1%").arg(value()));
}


void RadiusProgressWidget::setBackColor(const QColor &backColor)
{
    m_backColor = backColor;
}

void RadiusProgressWidget::setProgressColor(const QColor &progressColor)
{
    m_progressColor = progressColor;
}

void RadiusProgressWidget::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
}
