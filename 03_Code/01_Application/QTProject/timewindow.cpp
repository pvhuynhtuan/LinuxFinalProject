#include "timewindow.h"
#include "ui_timewindow.h"
#include <QTimer>
#include <QTime>
#include <QDate>

TimeWindow::TimeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeWindow)
{
    ui->setupUi(this);


    glCpuInfo = new CpuInfoClass("cpu ", CPU_USAGE_PATH, CPU_LOAD_PATH, CPU_TEMPER_PATH);
    #ifdef Q_OS_LINUX
    glCpuInfo->CalculateCpuUsage(); // dummy read to start initial data
    #endif

    gpRamInfo = new RamInfoClass();
    #ifdef Q_OS_LINUX
    gpRamInfo->CalculateRamUsage(); // dummy read to start initial data
    #endif

    gpNetworkInfo = new NetworkInfoClass(NETWORK_DATA_PATH);
    gpNetworkInfo->setPeriod((double)TIME_MEDIAN_TIMER_INTERVAL_MS / 1000.0);

    QTimer *lpTimerFast = new QTimer(this);
    connect(lpTimerFast, &QTimer::timeout, this, &TimeWindow::onTimerExceedFast);
    lpTimerFast->start(TIME_FAST_TIMER_INTERVAL_MS);

    QTimer *lpTimerMedian = new QTimer(this);
    connect(lpTimerMedian, &QTimer::timeout, this, &TimeWindow::onTimerExceedMedian);
    lpTimerMedian->start(TIME_MEDIAN_TIMER_INTERVAL_MS);

    QTimer *lpTimerSlow = new QTimer(this);
    connect(lpTimerSlow, &QTimer::timeout, this, &TimeWindow::onTimerExceedSlow);
    lpTimerSlow->start(TIME_SLOW_TIMER_INTERVAL_MS);
}

TimeWindow::~TimeWindow()
{
    delete ui;
}

void TimeWindow::on_pushButton_clicked()
{
    this->hide();
}

void TimeWindow::onTimerExceedFast()
{
    double ldCpuUsage;
    double ldRamUsage;

    // Display the CPU usage
    #ifdef Q_OS_LINUX
    ldCpuUsage = glCpuInfo->CalculateCpuUsage();
    if (ldCpuUsage >= 0)
    {
        ui->pbCPU->setValue((int)ldCpuUsage);
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldCpuUsage = rand() % 100; // Fake data for now => to test on the window
    ui->pbCPU->setValue((int)ldCpuUsage);
    #endif

    // Display the RAM usage
    #ifdef Q_OS_LINUX
    ldRamUsage = gpRamInfo->CalculateRamUsage();
    if (ldRamUsage >= 0)
    {
        ui->pbRAM->setValue((int)ldRamUsage);
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    ldRamUsage = rand() % 100; // Fake data for now => to test on the window
    ui->pbRAM->setValue((int)ldRamUsage);
    #endif
}

void TimeWindow::onTimerExceedMedian()
{
    // Display the network data
    #ifdef Q_OS_LINUX
    int liReturnValue = gpNetworkInfo->getCurrentNetworkInfo();
    if (0 <= liReturnValue)
    {
        if (gpNetworkInfo->getUploadSpeed() < 1024.0)
        {
            ui->lbUpSpeed->setText(QString("%1 KB/s").arg(gpNetworkInfo->getUploadSpeed(), 0, 'f', 2));
        }
        else
        {
            ui->lbUpSpeed->setText(QString("%1 MB/s").arg(gpNetworkInfo->getUploadSpeed() / 1024.0, 0, 'f', 2));
        }

        if (gpNetworkInfo->getDownloadSpeed() < 1024.0)
        {
            ui->lbDownSpeed->setText(QString("%1 KB/s").arg(gpNetworkInfo->getDownloadSpeed(), 0, 'f', 2));
        }
        else
        {
            ui->lbDownSpeed->setText(QString("%1 MB/s").arg(gpNetworkInfo->getDownloadSpeed() / 1024.0, 0, 'f', 2));
        }

    }
    else
    {
        // Do nothing
    }
    #endif /* End of #ifdef Q_OS_LINUX */
}

void TimeWindow::onTimerExceedSlow()
{
    QLocale loLocale;
    QDate loToday = QDate::currentDate();
    int liDayOfWeek = loToday.dayOfWeek();

    QString lsDayName = loLocale.dayName(liDayOfWeek, QLocale::LongFormat);

    ui->lbCurrentTime->setText(QTime::currentTime().toString());
    ui->lbCurrentDate->setText(QString("%1, %2").arg(lsDayName).arg(loToday.toString("dd/MM/yyyy")));
}

