#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H

#include <QMainWindow>

#include "cpuinfoclass.h"
#include "raminfoclass.h"
#include "networkinfoclass.h"
#include "AppConfig.h"

// Specific define for CPU display
#define TIME_FAST_TIMER_INTERVAL_MS         100
#define TIME_MEDIAN_TIMER_INTERVAL_MS       500
#define TIME_SLOW_TIMER_INTERVAL_MS         1000

namespace Ui {
class TimeWindow;
}

class TimeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TimeWindow(QWidget *parent = nullptr);
    ~TimeWindow();

private slots:
    void on_pushButton_clicked();
    void onTimerExceedFast();
    void onTimerExceedMedian();
    void onTimerExceedSlow();

private:
    CpuInfoClass *glCpuInfo;
    RamInfoClass *gpRamInfo;
    NetworkInfoClass *gpNetworkInfo;
    Ui::TimeWindow *ui;
};

#endif // TIMEWINDOW_H
