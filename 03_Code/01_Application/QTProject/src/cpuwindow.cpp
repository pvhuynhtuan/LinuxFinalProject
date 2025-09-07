#include "cpuwindow.h"
#include "ui_cpuwindow.h"
#include <QThread>
#include <QDebug>

cpuwindow::cpuwindow(AppDataProcessing* lpProcessor, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cpuwindow)
{
    ui->setupUi(this);

    gpCpuChart = ui->cCpuChart->chart();
    gpCpuChart->setMargins(QMargins(0, 0, 0, 0)); // smaller padding around plot
    gpCpuChart->setBackgroundBrush(QBrush(Qt::black));       // Solid color
    gpCpuChart->setBackgroundRoundness(0);                   // No rounded corners
    // gpCpuChart->setTitle("CPU Usage");
    gpCpuChart->removeAllSeries();

    QFont lLabelFont;
    lLabelFont.setPointSize(7);   // font size in points

    // Setting the axis:
    gpAxisX = new QValueAxis();
    gpAxisX->setLinePen(QColor(255, 170, 0)); // Line (axis) color
    gpAxisX->setGridLinePen(QPen(Qt::gray)); // Grid line color
    gpAxisX->setLabelsColor(QColor(255, 170, 0)); // Label (text) color
    gpAxisX->setLabelsVisible(false);
    gpAxisX->setLabelsFont(lLabelFont);
    gpAxisX->setRange(0, CPU_MAX_POINTS);
    gpCpuChart->addAxis(gpAxisX, Qt::AlignBottom);

    gpAxisY = new QValueAxis();
    gpAxisY->setLinePen(QColor(255, 170, 0)); // Line (axis) color
    gpAxisY->setGridLinePen(QPen(Qt::gray)); // Grid line color
    gpAxisY->setLabelsColor(QColor(255, 170, 0)); // Label (text) color
    gpAxisY->setLabelsFont(lLabelFont);
    gpAxisY->setRange(0, 100);
    gpCpuChart->addAxis(gpAxisY, Qt::AlignLeft);

    connect(lpProcessor, &AppDataProcessing::onTotalCpuUsageDataReady, this, &cpuwindow::onTotalCpuUsageDataUpdate);
    connect(lpProcessor, &AppDataProcessing::onTotalCpuTemperDataReady, this, &cpuwindow::onTotalCpuTemperDataUpdate);
    connect(lpProcessor, &AppDataProcessing::onSubCpusUsageDataReady, this, &cpuwindow::onSubCpusUsageDataUpdate);
}

cpuwindow::~cpuwindow()
{
    // Free memory
    glCpuSeries.clear();
    delete gpCpuChart;
    delete ui;
}

void cpuwindow::on_btnBackButton_clicked()
{
    this->hide();
}

void cpuwindow::onTotalCpuUsageDataUpdate(double ldTotalCpuUsage, double ldTotalCpuLoad)
{
    // Display the Load Average
    ui->lbLoadAvg->setText(QString::number(ldTotalCpuLoad, 'f', 2));
    // Display the CPU usage
    ui->lbTotalCpuUsage->setText(QString::number(ldTotalCpuUsage, 'f', 2));
}

void cpuwindow::onTotalCpuTemperDataUpdate(double ldTotalCpuTemper)
{
    // Display the CPU temperature
    ui->lbTemperature->setText(QString::number(ldTotalCpuTemper, 'f', 2));
}

void cpuwindow::onSubCpusUsageDataUpdate(QList<double> ldCpusUsage)
{
    static qint64 timeCounter = 0;
    int liIndex;
    timeCounter++;

    // Initial the series in the first time
    if (0 == glCpuSeries.size())
    {
        if (0 < ldCpusUsage.size())
        {
            for (liIndex = 0; liIndex < ldCpusUsage.size(); liIndex++)
            {
                // Create the series of CPU core
                QLineSeries *lpSeries = new QLineSeries();

                lpSeries->setName(QString("%1").arg(liIndex));
                gpCpuChart->addSeries(lpSeries);
                lpSeries->attachAxis(gpAxisX);
                lpSeries->attachAxis(gpAxisY);

                glCpuSeries.append(lpSeries);  // store for later use
            }
        } else
        {
            qDebug() << "CPU count is wrong";
        }
    }
    else
    {
        // Do nothing
    }

    for (liIndex = 0; liIndex < ldCpusUsage.size(); ++liIndex)
    {
        //Checking the return value
        if (0 <= ldCpusUsage[liIndex])
        {
            glCpuSeries[liIndex]->append(timeCounter, ldCpusUsage[liIndex]);
            if (CPU_MAX_POINTS < glCpuSeries[liIndex]->count())
            {
                glCpuSeries[liIndex]->removePoints(0, glCpuSeries[liIndex]->count() - CPU_MAX_POINTS);
            }
            else
            {
                // Do nothing
            }
        }
        else
        {
            // Do nothing
        }
    }

    // Keep only the last CPU_MAX_POINTS points for each series
    if (timeCounter > CPU_MAX_POINTS)
    {
        gpAxisX->setRange(timeCounter - CPU_MAX_POINTS, timeCounter);
    }
}
