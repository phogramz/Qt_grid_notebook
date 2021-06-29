#ifndef DIALOGCHART_H
#define DIALOGCHART_H

#include "mainwindow.h"

#include <QDialog>

namespace Ui {
class DialogChart;
}

class DialogChart : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChart(QWidget *parent = nullptr);
    ~DialogChart();
     void setChartList(QList<TMonitor> _tempChartList);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogChart *ui;
};

#endif // DIALOGCHART_H
