#ifndef NETWORKINFOCLASS_H
#define NETWORKINFOCLASS_H

#include <QDebug>
#include <QFile>
#include <QRegularExpression>

#include "AppConfig.h"

// include the linux platform header section
#ifdef Q_OS_LINUX
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#endif /* End of #ifdef Q_OS_LINUX */

class NetworkInfoClass
{
public:
    NetworkInfoClass();
    NetworkInfoClass(QString ifInfoPath);

    QString getMyIP();
    double getUploadSpeed();
    double getDownloadSpeed();
    int getCurrentNetworkInfo();

    void setPeriod(double time);

private:
    QString ifInfoPath;
    QString myIP;
    double uploadSpeed;
    double downloadSpeed;

    double period;
    uint32_t rxBytesData;
    uint32_t txBytesData;

    int readInterfaceData(QString ifName);

};

#endif // NETWORKINFOCLASS_H
