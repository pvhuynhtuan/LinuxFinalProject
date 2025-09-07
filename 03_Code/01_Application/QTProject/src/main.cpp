#include "mainwindow.h"
#include "appdataprocessing.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* USER CODE BEGIN */
    // Worker thread
    QThread* lpWorkerThread = new QThread;
    AppDataProcessing* lpProcessor = new AppDataProcessing();

    lpProcessor->moveToThread(lpWorkerThread);

    // Start thread
    QObject::connect(lpWorkerThread, &QThread::started, lpProcessor, &AppDataProcessing::startDataProcessing);
    lpWorkerThread->start();
    /* USER CODE END */

    MainWindow w(lpProcessor);
    w.show();

    int ret =  a.exec();

    // Free the memory
    lpWorkerThread->quit();
    lpWorkerThread->wait();
    lpProcessor->deleteLater();
    return ret;
}
