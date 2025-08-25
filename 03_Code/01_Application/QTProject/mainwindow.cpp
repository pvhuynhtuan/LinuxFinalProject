#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QDate>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , cpuScreen(nullptr)
    , memoryScreen(nullptr)
    , timeScreen(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     * Setting the overal CPU information
     * Note: In this time, the fixed paths are provided, need improve later if any
     */
    glTotalCpuInfo = new CpuInfoClass("cpu ", CPU_USAGE_PATH, CPU_LOAD_PATH, CPU_TEMPER_PATH);
    #ifdef Q_OS_LINUX
    glTotalCpuInfo->CalculateCpuUsage(); // dummy read to start initial data
    #endif

    gpRamInfo = new RamInfoClass();
    #ifdef Q_OS_LINUX
    gpRamInfo->CalculateRamUsage(); // dummy read to start initial data
    #endif

    gpNetworkInfo = new NetworkInfoClass(NETWORK_DATA_PATH);
    gpNetworkInfo->setPeriod((double)MAIN_MEDIAN_TIMER_INTERVAL_MS / 1000.0);

    pCpuMeter = findChild<RingMeter*>("meCPU");
    pCpuMeter->setName("CPU");
    pRamMeter = findChild<RingMeter*>("meRam");
    pRamMeter->setName("RAM");

    QTimer *lpTimerFast = new QTimer(this);
    connect(lpTimerFast, &QTimer::timeout, this, &MainWindow::onTimerExceedFast);
    lpTimerFast->start(MAIN_FAST_TIMER_INTERVAL_MS);

    QTimer *lpTimerMedian = new QTimer(this);
    connect(lpTimerMedian, &QTimer::timeout, this, &MainWindow::onTimerExceedMedian);
    lpTimerMedian->start(MAIN_MEDIAN_TIMER_INTERVAL_MS);

    QTimer *lpTimerSlow = new QTimer(this);
    connect(lpTimerSlow, &QTimer::timeout, this, &MainWindow::onTimerExceedSlow);
    lpTimerSlow->start(MAIN_SLOW_TIMER_INTERVAL_MS);

    connect(ui->lbCurrentTime, &ClickableLabelClass::onClicked, this, &MainWindow::onTimeLabelClicked);
    connect(pCpuMeter, &RingMeter::onClicked, this, &MainWindow::onCPUMeterClicked);
    connect(pRamMeter, &RingMeter::onClicked, this, &MainWindow::onRAMMeterClicked);
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
        timeScreen = new TimeWindow(this);  // or nullptr for independent window
    }
    timeScreen->show();
    timeScreen->raise();
    timeScreen->activateWindow();
}

void MainWindow::onCPUMeterClicked()
{
    if (!cpuScreen) {
        cpuScreen = new cpuwindow(this);  // or nullptr for independent window
    }
    cpuScreen->show();
    cpuScreen->raise();
    cpuScreen->activateWindow();
}

void MainWindow::onRAMMeterClicked()
{
    if (!memoryScreen) {
        memoryScreen = new MemoryWindow(this);  // or nullptr for independent window
    }
    memoryScreen->show();
    memoryScreen->raise();
    memoryScreen->activateWindow();
}

void MainWindow::onTimerExceedFast()
{
    double ldLoadAvg, ldTotalCpuUsage;
    double ldRamUsage;
    #ifdef Q_OS_LINUX
    double ldRamTotal, ldRamAvailable;
    #endif

    // Display the CPU usage
    #ifdef Q_OS_LINUX
    ldTotalCpuUsage = glTotalCpuInfo->CalculateCpuUsage();
    if (ldTotalCpuUsage >= 0)
    {
        pCpuMeter->setValue(ldTotalCpuUsage);
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldTotalCpuUsage = rand() % 100; // Fake data for now => to test on the window
    pCpuMeter->setValue(ldTotalCpuUsage);
    #endif

    // Display the Load Average
    #ifdef Q_OS_LINUX
    ldLoadAvg = glTotalCpuInfo->CalculateLoadAverage();
    if (ldLoadAvg >= 0)
    {
        ui->lbCpuInfo->setText(QString("Load Avg: %1").arg(ldLoadAvg, 0, 'f', 2));
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldLoadAvg = rand() % 10; // Fake data for now => to test on the window
    ui->lbCpuInfo->setText(QString("Load Avg: %1").arg(ldLoadAvg, 0, 'f', 2));
    #endif

    // Display the RAM usage
    #ifdef Q_OS_LINUX
    ldRamUsage = gpRamInfo->CalculateRamUsage();
    if (ldRamUsage >= 0)
    {
        pRamMeter->setValue(ldRamUsage);
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldRamUsage = rand() % 100; // Fake data for now => to test on the window
    pRamMeter->setValue(ldRamUsage);
    #endif

    // Display the RAM data  
    #ifdef Q_OS_LINUX
    ldRamTotal = (double)gpRamInfo->getRamTotal() / 1024.0;
    ldRamAvailable = (double)gpRamInfo->getRamAvailable() / 1024.0;
    ui->lbRamInfo->setText(QString("RAM: %1/%2 MB").arg(ldRamTotal - ldRamAvailable, 0, 'f', 0).arg(ldRamTotal, 0, 'f', 0));
    #endif
}

void MainWindow::onTimerExceedMedian()
{
    double ldTemper;
    // Display the cpu temperature
    #ifdef Q_OS_LINUX
    ldTemper = glTotalCpuInfo->CalculateTemperature();
    if (ldTemper >= 0)
    {
        ui->lbCpuTemp->setText(QString::number(ldTemper, 'f', 1));
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldTemper = rand() % 100; // Fake data for now => to test on the window
    ui->lbCpuTemp->setText(QString::number(ldTemper, 'f', 1));
    #endif

    // Display the network data
    #ifdef Q_OS_LINUX
    int liReturnValue = gpNetworkInfo->getCurrentNetworkInfo();
    if (0 < liReturnValue)
    {
        #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
        qDebug() << "New IP address: " << gpNetworkInfo->getMyIP();
        qDebug() << "Upload Speed: " << gpNetworkInfo->getUploadSpeed();
        qDebug() << "Download Speed: " << gpNetworkInfo->getDownloadSpeed();
        #endif

        ui->lbMyIP->setText(gpNetworkInfo->getMyIP());
    }
    else if (0 == liReturnValue)
    {
        #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
        qDebug() << "Upload Speed: " << gpNetworkInfo->getUploadSpeed();
        qDebug() << "Download Speed: " << gpNetworkInfo->getDownloadSpeed();
        #endif

        if (gpNetworkInfo->getUploadSpeed() < 1024.0)
        {
            ui->lbNetworkUp->setText(QString("%1 KB/s").arg(gpNetworkInfo->getUploadSpeed(), 0, 'f', 2));
        }
        else
        {
            ui->lbNetworkUp->setText(QString("%1 MB/s").arg(gpNetworkInfo->getUploadSpeed() / 1024.0, 0, 'f', 2));
        }

        if (gpNetworkInfo->getDownloadSpeed() < 1024.0)
        {
            ui->lbNetworkDown->setText(QString("%1 KB/s").arg(gpNetworkInfo->getDownloadSpeed(), 0, 'f', 2));
        }
        else
        {
            ui->lbNetworkDown->setText(QString("%1 MB/s").arg(gpNetworkInfo->getDownloadSpeed() / 1024.0, 0, 'f', 2));
        }

    }
    else
    {
        // Do nothing
    }
    #endif /* End of #ifdef Q_OS_LINUX */
}

void MainWindow::onTimerExceedSlow()
{
    ui->lbCurrentTime->setText(QTime::currentTime().toString());
    ui->lbCurrentDate->setText(QDate::currentDate().toString("dd/MM/yyyy"));
}
