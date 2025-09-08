#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H

#include <QMainWindow>

#include "appdataprocessing.h"
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
    explicit TimeWindow(AppDataProcessing* lpProcessor, QWidget *parent = nullptr);
    ~TimeWindow();

private slots:
    void on_pushButton_clicked();
    void onTimerExceedSlow();
    void onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad);
    void onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree);
    void onNetworkDataUpdate(QString lsMyIP, double ldUploadSpeed, double ldDownloadSpeed);

private:
    Ui::TimeWindow *ui;
};

#endif // TIMEWINDOW_H
