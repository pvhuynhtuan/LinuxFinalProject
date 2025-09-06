#include "networkinfoclass.h"

NetworkInfoClass::NetworkInfoClass(QString ifInfoPath)
{
    myIP = QString();
    uploadSpeed = 0.0;
    downloadSpeed = 0.0;
    period = 1.0;
    rxBytesData = 0.0;
    txBytesData = 0.0;
    this->ifInfoPath = ifInfoPath;
}

QString NetworkInfoClass::getMyIP()
{
    return this->myIP;
}

double NetworkInfoClass::getUploadSpeed()
{
    return this->uploadSpeed;
}

double NetworkInfoClass::getDownloadSpeed()
{
    return this->downloadSpeed;
}

int NetworkInfoClass::getCurrentNetworkInfo()
{
    int32_t lsReturnValue;

    lsReturnValue = -1; // Invalid as initial value
    #ifdef Q_OS_LINUX
    struct ifaddrs *ifaddr, *ifa_index;
    char ip[INET_ADDRSTRLEN]; // The variable to store the ip as string

    if (getifaddrs(&ifaddr) == -1)
    {
        #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
        qDebug() << "Can't get the list of interface";
        #endif
        return lsReturnValue;
    }

    for (ifa_index = ifaddr; ifa_index != NULL; ifa_index = ifa_index->ifa_next)
    {
        if ((ifa_index->ifa_addr != NULL) && (ifa_index->ifa_addr->sa_family == AF_INET))
        {
            // Store the address from the interface
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa_index->ifa_addr;

            // Convert the network address structure into a character string.
            inet_ntop(AF_INET, &addr->sin_addr, ip, INET_ADDRSTRLEN);

            // Store the my ip but ignore loop address
            if (strcmp(ifa_index->ifa_name, "lo") != 0)
            {
                if(this->myIP != QString::fromUtf8(ip))
                {
                    this->myIP = QString::fromUtf8(ip);

                    //Reset the speed data in case of new IP detected
                    this->uploadSpeed = 0.0;
                    this->downloadSpeed = 0.0;
                    this->rxBytesData = 0.0;
                    this->txBytesData = 0.0;

                    lsReturnValue = 1;
                }
                else
                {
                    lsReturnValue = 0;
                }

                if (ifa_index->ifa_data != nullptr)
                {
                    struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa_index->ifa_data;
                    this->downloadSpeed = (double)(stats->rx_bytes - this->rxBytesData) / this->period;
                    this->uploadSpeed = (double)(stats->tx_bytes - this->txBytesData) / this->period;
                    this->rxBytesData = stats->rx_bytes;
                    this->txBytesData = stats->tx_bytes;
                    #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
                    qDebug() << "Getting the bytes data " << this->rxBytesData << "; " << this->txBytesData;
                    #endif
                }
                else
                {
                    #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
                    qDebug() << "no ifa_data found";
                    #endif
                    readInterfaceData(QString("%1:").arg(ifa_index->ifa_name));
                }

                // Free the memory which allocated by getifaddrs, close the file
                freeifaddrs(ifaddr);

                return lsReturnValue;
            }
        }
        else
        {
            // Do nothing
        }
    }
    // Free the memory which allocated by getifaddrs, close the file
    freeifaddrs(ifaddr);
    #endif /* End of #ifdef Q_OS_LINUX */
    return lsReturnValue;
}

void NetworkInfoClass::setPeriod(double time)
{
    this->period = time;
}

int NetworkInfoClass::readInterfaceData(QString ifName)
{
    static const QRegularExpression WHITESPACE("\\s+");
    uint64_t lulRxBytes, lulTxBytes;

    QFile loFile(this->ifInfoPath);
    // Read the file
    if (loFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Read entire file content at once
        QString lsData = loFile.readAll();
        loFile.close();

        // Split into lines (skip empty ones)
        QStringList lolines = lsData.split("\n", Qt::SkipEmptyParts);

        for (const QString &lsLine : lolines)
        {
            #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
            qDebug() << "lsLine: " << lsLine;
            #endif
            QStringList lsParts = lsLine.split(WHITESPACE, Qt::SkipEmptyParts);
            if (lsParts[0] == ifName)
            {
                lulRxBytes = lsParts[1].toULongLong();
                lulTxBytes = lsParts[9].toULongLong();

                this->uploadSpeed = ((double)(lulTxBytes - this->txBytesData) / 1024.0) / this->period; // Using the KB/s unit
                this->downloadSpeed = ((double)(lulRxBytes - this->rxBytesData) / 1024.0) / this->period; // Using the KB/s unit
                this->rxBytesData = lulRxBytes;
                this->txBytesData = lulTxBytes;

                #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
                qDebug() << "lulRxBytes: " << lulRxBytes;
                qDebug() << "lulTxBytes: " << lulTxBytes;
                #endif

                return 1;
            }
            else
            {
                // Do nothing
            }
        }
        return -1;
    }
    else
    {
        #if (APP_DEBUG_PRINT_ENABLE == STD_ON)
        qDebug() << "Failed to read the file: " << this->ifInfoPath;
        #endif
        return -1;
    }
}
