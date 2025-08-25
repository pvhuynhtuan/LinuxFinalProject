#include "memorywindow.h"
#include "ui_memorywindow.h"
#include <QTimer>
#include <QTime>
#include <QStorageInfo>

MemoryWindow::MemoryWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MemoryWindow)
{
    ui->setupUi(this);


    pRamMeter = findChild<StatusCircle*>("wRamCircle");
    pMemMeter = findChild<StatusCircle*>("wMemCircle");

    gpRamInfo = new RamInfoClass();
    #ifdef Q_OS_LINUX
    gpRamInfo->CalculateRamUsage(); // dummy read to start initial data
    #endif

    qint64 llBytesTotal, llBytesAvail;
    QStorageInfo root = QStorageInfo::root();
    llBytesTotal = root.bytesTotal();
    llBytesAvail = root.bytesAvailable();
    ui->lbMemRootPath->setText(root.rootPath());
    ui->lbMemDevice->setText(root.device());
    ui->lbMemFileSysType->setText(root.fileSystemType());
    ui->lbMemTotal->setText(QString("%1").arg(llBytesTotal / 1024));
    ui->lbMemAvail->setText(QString("%1").arg(llBytesAvail / 1024));
    pMemMeter->setValue(((double)(llBytesTotal - llBytesAvail) * 100.0) / (double)llBytesTotal);

    QTimer *lpTimerSlow = new QTimer(this);
    connect(lpTimerSlow, &QTimer::timeout, this, &MemoryWindow::onTimerExceedSlow);
    lpTimerSlow->start(MEM_SLOW_TIMER_INTERVAL_MS);

    QTimer *lpTimerFast = new QTimer(this);
    connect(lpTimerFast, &QTimer::timeout, this, &MemoryWindow::onTimerExceedFast);
    lpTimerFast->start(MEM_FAST_TIMER_INTERVAL_MS);
}

MemoryWindow::~MemoryWindow()
{
    delete pRamMeter;
    delete pMemMeter;
    delete ui;
}

void MemoryWindow::on_btnMemBack_clicked()
{
    this->hide();
}

void MemoryWindow::onTimerExceedSlow()
{
    qint64 llBytesTotal, llBytesAvail;
    QStorageInfo root = QStorageInfo::root();
    llBytesTotal = root.bytesTotal();
    llBytesAvail = root.bytesAvailable();
    ui->lbMemRootPath->setText(root.rootPath());
    ui->lbMemDevice->setText(root.device());
    ui->lbMemFileSysType->setText(root.fileSystemType());
    ui->lbMemTotal->setText(QString("%1").arg(llBytesTotal / 1024));
    ui->lbMemAvail->setText(QString("%1").arg(llBytesAvail / 1024));
    pMemMeter->setValue(((double)(llBytesTotal - llBytesAvail) * 100.0) / (double)llBytesTotal);
}

void MemoryWindow::onTimerExceedFast()
{
    double ldRamUsage;
    #ifdef Q_OS_LINUX
    double ldRamTotal, ldRamAvailable, ldSwapTotal, ldSwapFree;
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
    ldSwapTotal = (double)gpRamInfo->getSwapTotal() / 1024.0;
    ldSwapFree = (double)gpRamInfo->getSwapFree() / 1024.0;
    ui->lbRamTotal->setText(QString("%1").arg(ldRamTotal, 0, 'f', 0));
    ui->lbRamAvail->setText(QString("%1").arg(ldRamAvailable, 0, 'f', 0));
    ui->lbSwapTotal->setText(QString("%1").arg(ldSwapTotal, 0, 'f', 0));
    ui->lbSwapFree->setText(QString("%1").arg(ldSwapFree, 0, 'f', 0));
    #endif
}

