#ifndef DIALOGDIAGRAM_H
#define DIALOGDIAGRAM_H

#include "tmonitor.h"

#include <QDialog>

namespace Ui {
class DialogDiagram;
}

///
/// \brief Класс диалогового окна с динамическим графиком
/// \details Класс диалоговой формы, отображающий поступающие данные в виде
/// динамического графика. График использует библиотеку qCustomPlot.
///
class DialogDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDiagram(QWidget *parent = nullptr);
    ~DialogDiagram();

    void drawDiagram();
    void setDiagremList(QList<TMonitor> _tempDiagramList);
    void setChartList(QList<TMonitor> _tempChartList);

protected:
    //void paintEvent(QPaintEvent *event);

private slots:
    void on_radioButtonXFramerate_clicked();

    void on_radioButtonXWeight_clicked();

    void on_radioButtonXPrice_clicked();

    void on_radioButtonYFramerate_clicked();

    void on_radioButtonYWeight_clicked();

    void on_radioButtonYPrice_clicked();

private:
    Ui::DialogDiagram *ui;
    QList<TMonitor> _diagramList;
    QList<int> iMinMaxValues;
    QList<float> fMinMaxValues;
};

#endif // DIALOGDIAGRAM_H
