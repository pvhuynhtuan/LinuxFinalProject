#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDate>

#include <QMessageBox>

MainWindow::MainWindow(AppDataProcessing* lpProcessor, QWidget *parent)
    : QMainWindow(parent)
    , gpProcessor(lpProcessor)
    , cpuScreen(nullptr)
    , memoryScreen(nullptr)
    , timeScreen(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pCpuMeter = findChild<RingMeter*>("meCPU");
    pCpuMeter->setName("CPU");
    pRamMeter = findChild<RingMeter*>("meRam");
    pRamMeter->setName("RAM");

    QTimer *lpTimerSlow = new QTimer(this);
    connect(lpTimerSlow, &QTimer::timeout, this, &MainWindow::onTimerExceedSlow);
    lpTimerSlow->start(MAIN_SLOW_TIMER_INTERVAL_MS);

    connect(ui->lbCurrentTime, &ClickableLabelClass::onClicked, this, &MainWindow::onTimeLabelClicked);
    connect(pCpuMeter, &RingMeter::onClicked, this, &MainWindow::onCPUMeterClicked);
    connect(pRamMeter, &RingMeter::onClicked, this, &MainWindow::onRAMMeterClicked);

    connect(gpProcessor, &AppDataProcessing::onTotalCpuUsageDataReady, this, &MainWindow::onTotalCpuUsageDataUpdate);
    connect(gpProcessor, &AppDataProcessing::onTotalCpuTemperDataReady, this, &MainWindow::onTotalCpuTemperDataUpdate);
    connect(gpProcessor, &AppDataProcessing::onRamDataReady, this, &MainWindow::onRamDataUpdate);
    connect(gpProcessor, &AppDataProcessing::onNetworkDataReady, this, &MainWindow::onNetworkDataUpdate);
}

MainWindow::~MainWindow()
{
    // Free memory
    delete cpuScreen;
    delete memoryScreen;
    delete pCpuMeter;
    delete pRamMeter;
    delete ui;
}

void MainWindow::onTimeLabelClicked()
{
    if (!timeScreen) {
        timeScreen = new TimeWindow(gpProcessor, this);  // or nullptr for independent window
    }
    timeScreen->show();
    timeScreen->raise();
    timeScreen->activateWindow();
}

void MainWindow::onCPUMeterClicked()
{
    if (!cpuScreen) {
        cpuScreen = new cpuwindow(gpProcessor, this);  // or nullptr for independent window
    }
    cpuScreen->show();
    cpuScreen->raise();
    cpuScreen->activateWindow();
}

void MainWindow::onRAMMeterClicked()
{
    if (!memoryScreen) {
        memoryScreen = new MemoryWindow(gpProcessor, this);  // or nullptr for independent window
    }
    memoryScreen->show();
    memoryScreen->raise();
    memoryScreen->activateWindow();
}

void MainWindow::onTimerExceedSlow()
{
    ui->lbCurrentTime->setText(QTime::currentTime().toString());
    ui->lbCurrentDate->setText(QDate::currentDate().toString("dd/MM/yyyy"));
}

void MainWindow::onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad)
{
    // Display the CPU usage
    pCpuMeter->setValue(ldTotalCpuUsage);
    // Display the Load Average
    ui->lbCpuInfo->setText(QString("Load Avg: %1").arg(ldTotalCpuLoad, 0, 'f', 2));
}

void MainWindow::onTotalCpuTemperDataUpdate(double ldTotalCpuTemper)
{
    // Display the cpu temperature
    ui->lbCpuTemp->setText(QString::number(ldTotalCpuTemper, 'f', 1));
}

void MainWindow::onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree)
{
    // Unused parameters
    (void)ldSwapTotal;
    (void) ldSwapFree;

    // Display the RAM usage
    pRamMeter->setValue(ldRamUsage);

    // Display the RAM data
    ui->lbRamInfo->setText(QString("RAM: %1/%2 MB").arg(ldRamTotal - ldRamAvailable, 0, 'f', 0).arg(ldRamTotal, 0, 'f', 0));
}

void MainWindow::onNetworkDataUpdate(QString lsMyIP, double ldUploadSpeed, double ldDownloadSpeed)
{
    // Display the network IP

    ui->lbMyIP->setText(lsMyIP);

    // Display the network upload speed
    if (ldUploadSpeed < 1024.0)
    {
        ui->lbNetworkUp->setText(QString("%1 KB/s").arg(ldUploadSpeed, 0, 'f', 2));
    }
    else
    {
        ui->lbNetworkUp->setText(QString("%1 MB/s").arg(ldUploadSpeed / 1024.0, 0, 'f', 2));
    }

    // Display the network download speed
    if (ldDownloadSpeed < 1024.0)
    {
        ui->lbNetworkDown->setText(QString("%1 KB/s").arg(ldDownloadSpeed, 0, 'f', 2));
    }
    else
    {
        ui->lbNetworkDown->setText(QString("%1 MB/s").arg(ldDownloadSpeed / 1024.0, 0, 'f', 2));
    }
}
