#ifndef CPUWINDOW_H
#define CPUWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QTime>   // for QTime

#include "appdataprocessing.h"
#include "cpuinfoclass.h"
#include "AppConfig.h"

// Specific define for CPU display
#define CPU_GRAPH_TIMER_INTERVAL_MS         100
#define CPU_INFO_TIMER_INTERVAL_MS          1000
#define CPU_MAX_POINTS                      20

namespace Ui {
class cpuwindow;
}

class cpuwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cpuwindow(AppDataProcessing* lpProcessor, QWidget *parent = nullptr);
    ~cpuwindow();

private slots:
    void on_btnBackButton_clicked();
    void onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad);
    void onTotalCpuTemperDataUpdate(double ldTotalCpuTemper);
    void onSubCpusUsageDataUpdate(QList<double> ldCpusUsage);

private:
    Ui::cpuwindow *ui;
    QChart *gpCpuChart;
    QList<QLineSeries *> glCpuSeries;
    QValueAxis *gpAxisX;
    QValueAxis *gpAxisY;
};

#endif // CPUWINDOW_H
