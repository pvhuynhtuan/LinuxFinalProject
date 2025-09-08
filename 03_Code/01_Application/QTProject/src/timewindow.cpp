#include "timewindow.h"
#include "ui_timewindow.h"
#include <QTimer>
#include <QTime>
#include <QDate>

TimeWindow::TimeWindow(AppDataProcessing* lpProcessor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeWindow)
{
    ui->setupUi(this);

    QTimer *lpTimerSlow = new QTimer(this);
    connect(lpTimerSlow, &QTimer::timeout, this, &TimeWindow::onTimerExceedSlow);
    lpTimerSlow->start(TIME_SLOW_TIMER_INTERVAL_MS);

    connect(lpProcessor, &AppDataProcessing::onTotalCpuUsageDataReady, this, &TimeWindow::onTotalCpuUsageDataUpdate);
    connect(lpProcessor, &AppDataProcessing::onRamDataReady, this, &TimeWindow::onRamDataUpdate);
    connect(lpProcessor, &AppDataProcessing::onNetworkDataReady, this, &TimeWindow::onNetworkDataUpdate);
}

TimeWindow::~TimeWindow()
{
    delete ui;
}

void TimeWindow::on_pushButton_clicked()
{
    this->hide();
}

void TimeWindow::onTimerExceedSlow()
{
    QLocale loLocale;
    QDate loToday = QDate::currentDate();
    int liDayOfWeek = loToday.dayOfWeek();

    QString lsDayName = loLocale.dayName(liDayOfWeek, QLocale::LongFormat);

    ui->lbCurrentTime->setText(QTime::currentTime().toString());
    ui->lbCurrentDate->setText(QString("%1, %2").arg(lsDayName, loToday.toString("dd/MM/yyyy")));
}

void TimeWindow::onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad)
{
    // unused parameter
    (void)ldTotalCpuLoad;

    // Display the CPU usage
    ui->pbCPU->setValue((int)ldTotalCpuUsage);
}

void TimeWindow::onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree)
{
    // Unused parameters
    (void)ldRamTotal;
    (void)ldRamAvailable;
    (void)ldSwapTotal;
    (void)ldSwapFree;

    // Display the RAM usage
    ui->pbRAM->setValue((int)ldRamUsage);
}

void TimeWindow::onNetworkDataUpdate(QString lsMyIP, double ldUploadSpeed, double ldDownloadSpeed)
{
    // Unused parameters
    (void)lsMyIP;

    // Display the network upload speed
    if (ldUploadSpeed < 1024.0)
    {
        ui->lbUpSpeed->setText(QString("%1 KB/s").arg(ldUploadSpeed, 0, 'f', 2));
    }
    else
    {
        ui->lbUpSpeed->setText(QString("%1 MB/s").arg(ldUploadSpeed / 1024.0, 0, 'f', 2));
    }

    // Display the network download speed
    if (ldDownloadSpeed < 1024.0)
    {
        ui->lbDownSpeed->setText(QString("%1 KB/s").arg(ldDownloadSpeed, 0, 'f', 2));
    }
    else
    {
        ui->lbDownSpeed->setText(QString("%1 MB/s").arg(ldDownloadSpeed / 1024.0, 0, 'f', 2));
    }
}
