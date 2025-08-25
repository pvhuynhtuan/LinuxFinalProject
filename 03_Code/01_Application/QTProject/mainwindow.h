#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cpuwindow.h"
#include "memorywindow.h"
#include "timewindow.h"

#include "RingMeter.h"
#include "cpuinfoclass.h"
#include "raminfoclass.h"
#include "clickablelabelclass.h"
#include "networkinfoclass.h"

#include "AppConfig.h"

// Specific define for CPU display
#define MAIN_FAST_TIMER_INTERVAL_MS         100
#define MAIN_MEDIAN_TIMER_INTERVAL_MS       500
#define MAIN_SLOW_TIMER_INTERVAL_MS         1000

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimeLabelClicked();
    void onCPUMeterClicked();
    void onRAMMeterClicked();
    void onTimerExceedFast();
    void onTimerExceedMedian();
    void onTimerExceedSlow();

private:
    cpuwindow *cpuScreen;
    MemoryWindow *memoryScreen;
    TimeWindow *timeScreen;
    Ui::MainWindow *ui;
    RingMeter *pCpuMeter;
    RingMeter *pRamMeter;
    CpuInfoClass *glTotalCpuInfo;
    RamInfoClass *gpRamInfo;

    NetworkInfoClass *gpNetworkInfo;
};
#endif // MAINWINDOW_H
