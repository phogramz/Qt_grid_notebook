#include "dialogdiagram.h"
#include "ui_dialogdiagram.h"

#include <math.h>
#include <QWidget>
#include <QPainter>

///
/// @page
/// \brief Конструктор окна с графиком
/// \param parent
///
DialogDiagram::DialogDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDiagram)
{
    ui->setupUi(this);
    this->setWindowTitle("Diagrams");
}

///
/// \brief Деструктор окна с графиком
///
DialogDiagram::~DialogDiagram()
{
    delete ui;
}

void DialogDiagram::setDiagremList(QList<TMonitor> _tempDiagramList)
{
    //Инициализация глобального списка
    _diagramList = _tempDiagramList;

    //Получение максимальных значений для выбора диапазона графика
    int minFrame = 1000
      , maxFrame = 0
      , minPrice = 1000000
      , maxPrice = 0;

    float minWeight = 1000
        , maxWeight = 0;

    for (int i = 0; i < _diagramList.count(); i++){
       if (_diagramList[i].getFrameRate() > maxFrame)
           maxFrame = _diagramList[i].getFrameRate();
       if (_diagramList[i].getFrameRate() < minFrame)
           minFrame = _diagramList[i].getFrameRate();

       if (_diagramList[i].getPrice() > maxPrice)
           maxPrice = _diagramList[i].getPrice();
       if (_diagramList[i].getPrice() < minPrice)
           minPrice = _diagramList[i].getPrice();

       if (_diagramList[i].getWeight() > maxWeight)
           maxWeight = _diagramList[i].getWeight();
       if (_diagramList[i].getWeight() < minWeight)
           minWeight = _diagramList[i].getWeight();
    }
    iMinMaxValues.push_back(maxFrame);  //0
    iMinMaxValues.push_back(minFrame);  //1
    iMinMaxValues.push_back(maxPrice);  //2
    iMinMaxValues.push_back(minPrice);  //3
    fMinMaxValues.push_back(maxWeight); //0
    fMinMaxValues.push_back(minWeight); //1
}

//график зависимости диагонали от цены
void DialogDiagram::drawDiagram()
{
        //1. Подготовить данные x,y:
             const int size = _diagramList.count();
             QVector <double> x(size),y(size);
             for (int i = 0; i < size; i++) {
                 if (ui->radioButtonXFramerate->isChecked()){
                     x[i] = _diagramList[i].getFrameRate();
                 }
                 else if (ui->radioButtonXPrice->isChecked()){
                     x[i] = _diagramList[i].getPrice();
                 }
                 else if (ui->radioButtonXWeight->isChecked()){
                     x[i] = _diagramList[i].getWeight();
                 }

                 if (ui->radioButtonYFramerate->isChecked()){
                     y[i] = _diagramList[i].getFrameRate();
                 }
                 else if (ui->radioButtonYPrice->isChecked()){
                     y[i] = _diagramList[i].getPrice();
                 }
                 else if (ui->radioButtonYWeight->isChecked()){
                     y[i] = _diagramList[i].getWeight();
                 }
                //sin(qDegreesToRadians(x[i]));
             }
        //2. Создать график и добавить данные:
             ui->widget->addGraph();
             ui->widget->graph(0)->setData(x,y);
        //3. Настроить оси и подписи:
             if (ui->radioButtonXFramerate->isChecked()){
                 ui->widget->xAxis->setLabel(tr("Framerate"));
                 ui->widget->xAxis->setRange(iMinMaxValues[1], iMinMaxValues[0]);
             }
             else if (ui->radioButtonXPrice->isChecked()){
                 ui->widget->xAxis->setLabel(tr("Price"));
                 ui->widget->xAxis->setRange(iMinMaxValues[3], iMinMaxValues[2]);
             }
             else if (ui->radioButtonXWeight->isChecked()){
                 ui->widget->xAxis->setLabel(tr("Weight"));
                 ui->widget->xAxis->setRange(fMinMaxValues[1], fMinMaxValues[0]);
             }

             if (ui->radioButtonYFramerate->isChecked()){
                 ui->widget->yAxis->setLabel(tr("Framerate"));
                 ui->widget->yAxis->setRange(iMinMaxValues[1], iMinMaxValues[0]);
             }
             else if (ui->radioButtonYPrice->isChecked()){
                 ui->widget->yAxis->setLabel(tr("Price"));
                 ui->widget->yAxis->setRange(iMinMaxValues[3], iMinMaxValues[2]);
             }
             else if (ui->radioButtonYWeight->isChecked()){
                 ui->widget->yAxis->setLabel(tr("Weight"));
                 ui->widget->yAxis->setRange(fMinMaxValues[1], fMinMaxValues[0]);
             }

       //4. Показать всё это:
       ui->widget->replot();
}

//void DialogDiagram::paintEvent(QPaintEvent *event)
//{
//    //Q_UNUSED(event);
//    QPainter p(this); // Создаём новый объект рисовальщика
//       p.setPen(QPen(Qt::red,5,Qt::SolidLine)); // Настройки рисования
//       //p.drawLine(0,0,width(),height()); // Рисование линии
//       p.drawPie(100, 100, 200, 200, 70, 5000);
//       p.drawLine(0,0, 200, 200);

//}

void DialogDiagram::on_radioButtonXFramerate_clicked()
{
    drawDiagram();
}

void DialogDiagram::on_radioButtonXWeight_clicked()
{
    drawDiagram();
}

void DialogDiagram::on_radioButtonXPrice_clicked()
{
    drawDiagram();
}

void DialogDiagram::on_radioButtonYFramerate_clicked()
{
    drawDiagram();
}

void DialogDiagram::on_radioButtonYWeight_clicked()
{
    drawDiagram();
}

void DialogDiagram::on_radioButtonYPrice_clicked()
{
    drawDiagram();
}
