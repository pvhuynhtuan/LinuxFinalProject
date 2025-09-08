#include "memorywindow.h"
#include "ui_memorywindow.h"
#include <QTimer>
#include <QTime>
#include <QStorageInfo>

MemoryWindow::MemoryWindow(AppDataProcessing* lpProcessor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MemoryWindow)
{
    ui->setupUi(this);

    pRamMeter = findChild<WaveProgressWidget*>("wRamCircle");
    pMemMeter = findChild<WaveProgressWidget*>("wMemCircle");

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

    connect(lpProcessor, &AppDataProcessing::onRamDataReady, this, &MemoryWindow::onRamDataUpdate);
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

void MemoryWindow::onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree)
{
    // Display the RAM usage
    if (ldRamUsage >= 0)
    {
        pRamMeter->setValue(ldRamUsage);
    }
    else
    {
        // Do nothing
    }

    // Display the RAM data
    ui->lbRamTotal->setText(QString("%1").arg(ldRamTotal, 0, 'f', 0));
    ui->lbRamAvail->setText(QString("%1").arg(ldRamAvailable, 0, 'f', 0));
    ui->lbSwapTotal->setText(QString("%1").arg(ldSwapTotal, 0, 'f', 0));
    ui->lbSwapFree->setText(QString("%1").arg(ldSwapFree, 0, 'f', 0));
}

