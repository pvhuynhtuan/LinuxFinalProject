#ifndef MEMORYWINDOW_H
#define MEMORYWINDOW_H

#include <QMainWindow>

#include "waveprogresswidget.h"
#include "appdataprocessing.h"
#include "AppConfig.h"

#define MEM_SLOW_TIMER_INTERVAL_MS          5000

namespace Ui {
class MemoryWindow;
}

class MemoryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MemoryWindow(AppDataProcessing* lpProcessor, QWidget *parent = nullptr);
    ~MemoryWindow();

private slots:
    void on_btnMemBack_clicked();
    void onTimerExceedSlow();
    void onRamDataUpdate(double ldRamUsage, double ldRamTotal, double ldRamAvailable, double ldSwapTotal, double ldSwapFree);

private:
    Ui::MemoryWindow *ui;
    WaveProgressWidget *pRamMeter;
    WaveProgressWidget *pMemMeter;
};

#endif // MEMORYWINDOW_H
