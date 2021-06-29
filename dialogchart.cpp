#include "dialogchart.h"
#include "ui_dialogchart.h"

//Графика
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

DialogChart::DialogChart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChart)
{
    ui->setupUi(this);
}

DialogChart::~DialogChart()
{
    delete ui;
}

void DialogChart::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogChart::setChartList(QList<TMonitor> _tempChartList)
{
    QPieSeries *series = new QPieSeries();
        series->append("Jane", 1);
        series->append("Joe", 2);
        series->append("Andy", 3);
        series->append("Barbara", 4);
        series->append("Axel", 5);
    //![1]

    //![2]
        QPieSlice *slice = series->slices().at(1);
        slice->setExploded();
        slice->setLabelVisible();
        slice->setPen(QPen(Qt::darkGreen, 2));
        slice->setBrush(Qt::green);
    //![2]

    //![3]
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Simple piechart example");
        chart->legend()->hide();
    //![3]

    //![4]
        ui->widget = new QChartView(chart);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
    //![4]

    //![5]
        DialogChart *chartWindow = new DialogChart;
        //chartWindow.setCentralWidget(chartView);
        chartWindow->resize(400, 300);
        chartWindow->show();
        ui->widget->show();

        //ui->widget->set(chartView);
        //window.setCentralWidget(chartView);
        //window.resize(400, 300);
        //ui->widget->show();
}
