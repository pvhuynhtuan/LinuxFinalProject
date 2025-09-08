#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "cpuwindow.h"
#include "memorywindow.h"
#include "timewindow.h"

#include "RingMeter.h"
#include "clickablelabelclass.h"
#include "appdataprocessing.h"

#include "AppConfig.h"

// Specific define for CPU display
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
    MainWindow(AppDataProcessing* lpProcessor, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTimeLabelClicked();
    void onCPUMeterClicked();
    void onRAMMeterClicked();
    void onTimerExceedSlow();
    void onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad);
    void onTotalCpuTemperDataUpdate(double ldTotalCpuTemper);
    void onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree);
    void onNetworkDataUpdate(QString lsMyIP, double ldUploadSpeed, double ldDownloadSpeed);

private:
    AppDataProcessing* gpProcessor;
    cpuwindow *cpuScreen;
    MemoryWindow *memoryScreen;
    TimeWindow *timeScreen;
    Ui::MainWindow *ui;
    RingMeter *pCpuMeter;
    RingMeter *pRamMeter;
};
#endif // MAINWINDOW_H
