#ifndef WAVEPROGRESSWIDGET_H
#define WAVEPROGRESSWIDGET_H

#include <QWidget>

class WaveProgressWidget : public QWidget
{
    Q_OBJECT

public:
    enum class PercentStyle
    {
        Rect = 0,
        Circle,
        Ellipse
    };

    explicit WaveProgressWidget(QWidget *parent = nullptr);
    ~WaveProgressWidget();

    void setRange(int minValue, int maxValue);

    void setMinValue(int minValue);
    void setMaxValue(int maxValue);

    void setValue(int value);

    void setWaterDensity(int waterDensity);
    void setWaterHeight(double waterHeight);

    void setOffset(int offset);
    void setBorderWidth(double borderWidth);
    void setBackgroundColor(const QColor &backgroundColor);

    void setPointerStyle(const PercentStyle &percentStyle);

    virtual QSize sizeHint() const override final;

private Q_SLOTS:
    void timeout();

private:
    virtual void paintEvent(QPaintEvent *event) override final;

    void drawValue(QPainter *painter);

    int m_value;
    int m_minValue;
    int m_maxValue;
    int m_waterDensity;
    double m_waterHeight;
    double m_offset;
    int m_borderWidth;

    QColor m_backgroundColor;
    QTimer *m_timer;
    PercentStyle m_percentStyle;
};
#endif // WAVEPROGRESSWIDGET_H
