#include "appdataprocessing.h"

AppDataProcessing::AppDataProcessing(QObject *parent) : QObject(parent)
{

}

void AppDataProcessing::startDataProcessing()
{
    goTotalCpuInfo = CpuInfoClass("cpu ", CPU_USAGE_PATH, CPU_LOAD_PATH, CPU_TEMPER_PATH);
    goTotalCpuInfo.CalculateCpuUsage(); // dummy read to start initial data

    goRamInfo = RamInfoClass();
    goRamInfo.CalculateRamUsage(); // dummy read to start initial data

    goNetworkInfo = NetworkInfoClass(NETWORK_DATA_PATH);
    goNetworkInfo.setPeriod((double)DATA_MEDIAN_TIMER_INTERVAL_MS / 1000.0);

    int liCoreCount = QThread::idealThreadCount();
    if (liCoreCount > 0)
    {
        qDebug() << "AppDataProcessing: Number of CPU cores:" << liCoreCount;
        for (int liIndex = 0; liIndex < liCoreCount; liIndex++)
        {
            // Create the object information of CPU core
            QString lsPrefix = QString("cpu%1").arg(liIndex);
            CpuInfoClass loCpuInfo(lsPrefix, CPU_USAGE_PATH, CPU_LOAD_PATH, CPU_TEMPER_PATH);
            loCpuInfo.CalculateCpuUsage(); // dummy read to start initial data

            // Append to glCpusInfo list
            glCpusInfo.append(loCpuInfo);
        }
    } else
    {
        qDebug() << "AppDataProcessing: Unable to detect CPU core count.";
    }

    QTimer *lpTimerFast = new QTimer(this);
    connect(lpTimerFast, &QTimer::timeout, this, &AppDataProcessing::onTimerExceedFast);
    lpTimerFast->start(DATA_FAST_TIMER_INTERVAL_MS);

    QTimer *lpTimerMedian = new QTimer(this);
    connect(lpTimerMedian, &QTimer::timeout, this, &AppDataProcessing::onTimerExceedMedian);
    lpTimerMedian->start(DATA_MEDIAN_TIMER_INTERVAL_MS);

    // QTimer *lpTimerSlow = new QTimer(this);
    // connect(lpTimerSlow, &QTimer::timeout, this, &AppDataProcessing::onTimerExceedSlow);
    // lpTimerSlow->start(DATA_SLOW_TIMER_INTERVAL_MS);
}

void AppDataProcessing::onTimerExceedFast()
{
    double ldLoadAvg, ldTotalCpuUsage;
    double ldRamUsage, ldRamTotal, ldRamAvailable, ldSwapTotal, ldSwapFree;
    double ldUsage;
    QList<double> ldCpusUsage;

    // calculate the CPU usage
    #ifdef Q_OS_LINUX
    ldTotalCpuUsage = goTotalCpuInfo.CalculateCpuUsage();
    ldLoadAvg = goTotalCpuInfo.CalculateLoadAverage();
    #elif defined(Q_OS_WIN)
    // Fake data for now => to test on the window
    ldTotalCpuUsage = rand() % 100;
    ldLoadAvg = rand() % 10;
    #endif

    // Trigger the CPU data sending
    emit onTotalCpuUsageDataReady(ldTotalCpuUsage, ldLoadAvg);

    // calculate the RAM data
    #ifdef Q_OS_LINUX
    ldRamUsage = goRamInfo.CalculateRamUsage();
    ldRamTotal = (double)goRamInfo.getRamTotal() / 1024.0;
    ldRamAvailable = (double)goRamInfo.getRamAvailable() / 1024.0;
    ldSwapTotal = (double)goRamInfo.getSwapTotal() / 1024.0;
    ldSwapFree = (double)goRamInfo.getSwapFree() / 1024.0;
    #elif defined(Q_OS_WIN)
    // Fake data for now => to test on the window
    ldRamUsage = rand() % 100;
    ldRamTotal = rand() % 512;
    ldRamAvailable = rand() % 512;
    ldSwapTotal = rand() % 512;;
    ldSwapFree = rand() % 512;;
    #endif

    // Trigger the RAM data sending
    emit onRamDataReady(ldRamUsage, ldRamTotal, ldRamAvailable, ldSwapTotal, ldSwapFree);

    // Calculate the sub-CPUs data
    for (int liIndex = 0; liIndex < glCpusInfo.size(); liIndex++)
    {
        #ifdef Q_OS_LINUX
        ldUsage = glCpusInfo[liIndex].CalculateCpuUsage();
        #elif defined(Q_OS_WIN)
        ldUsage = rand() % 100; // Fake data for now => to test on the window
        #endif

        //Checking the return value
        if (0 <= ldUsage)
        {
            ldCpusUsage.append(ldUsage);
        }
        else
        {
            ldCpusUsage.append(0.0);
        }
    }

    // Trigger the sub-CPUs data sending
    emit onSubCpusUsageDataReady(ldCpusUsage);
}

void AppDataProcessing::onTimerExceedMedian()
{
    double ldTemper;
    QString lsMyIP;
    double ldUploadSpeed, ldDownloadSpeed;

    // Calculate the cpu temperature
    #ifdef Q_OS_LINUX
    ldTemper = goTotalCpuInfo.CalculateTemperature();
    #elif defined(Q_OS_WIN)
    // Fake data for now => to test on the window
    ldTemper = rand() % 100;
    #endif

    // Trigger the CPU's temperature data sending
    emit onTotalCpuTemperDataReady(ldTemper);

    // calculate the network data
    #ifdef Q_OS_LINUX
    int liReturnValue = goNetworkInfo.getCurrentNetworkInfo();
    if (0 <= liReturnValue)
    {
        lsMyIP = goNetworkInfo.getMyIP();
        ldUploadSpeed = goNetworkInfo.getUploadSpeed();
        ldDownloadSpeed = goNetworkInfo.getDownloadSpeed();

        #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
        qDebug() << "New IP address: " << lsMyIP;
        qDebug() << "Upload Speed: " << ldUploadSpeed;
        qDebug() << "Download Speed: " << ldDownloadSpeed;
        #endif
    }
    else
    {
        // Do nothing
    }
    #elif defined(Q_OS_WIN)
    lsMyIP = QString("%1.%2.%3.%4").arg(rand() % 100).arg(rand() % 100).arg(rand() % 100).arg(rand() % 100);
    ldUploadSpeed = rand() % 6000;
    ldDownloadSpeed = rand() % 6000;
    #endif /* End of #ifdef Q_OS_LINUX */
    emit onNetworkDataReady(lsMyIP, ldUploadSpeed, ldDownloadSpeed);
}

// void AppDataProcessing::onTimerExceedSlow()
// {

// }
