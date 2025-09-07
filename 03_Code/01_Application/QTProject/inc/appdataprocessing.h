#ifndef APPDATAPROCESSING_H
#define APPDATAPROCESSING_H

#include <QObject>
#include <QTimer>
#include <QTime>   // for QTime

#include "cpuinfoclass.h"
#include "raminfoclass.h"
#include "networkinfoclass.h"
#include "AppConfig.h"

// Specific define for CPU display
#define DATA_FAST_TIMER_INTERVAL_MS         100
#define DATA_MEDIAN_TIMER_INTERVAL_MS       500
#define DATA_SLOW_TIMER_INTERVAL_MS         1000

class AppDataProcessing : public QObject
{
    Q_OBJECT
public:
    explicit AppDataProcessing(QObject *parent = nullptr);

public slots:
    void startDataProcessing();
    void onTimerExceedFast();
    void onTimerExceedMedian();
    // void onTimerExceedSlow();

signals:
    void onTotalCpuUsageDataReady(double ldTotalCpuUsage, double ldTotalCpuLoad);
    void onTotalCpuTemperDataReady(double ldTotalCpuTemper);
    void onRamDataReady(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree);
    void onNetworkDataReady(QString lsMyIP, double ldUploadSpeed, double ldDownloadSpeed);
    void onSubCpusUsageDataReady(QList<double> ldCpusUsage);

private:
    CpuInfoClass goTotalCpuInfo;
    QList<CpuInfoClass> glCpusInfo;
    RamInfoClass goRamInfo;
    NetworkInfoClass goNetworkInfo;
};

#endif // APPDATAPROCESSING_H
