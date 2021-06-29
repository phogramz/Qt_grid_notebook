#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timer.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QProgressBar>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QLibraryInfo>
#include <QMessageBox>

//�������
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

///
/// @mainpage
/// \brief ����������� �������� ����
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , myTimer(new Timer(this))
{   //������� ����
    setlocale(LC_CTYPE, "Russian");
    ui->setupUi(this);
    this->setWindowTitle("Monitor's base");

    //����������� ����
    // ������������� ����������� �������� ����������� ����
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // ���������� ���� ������ ������������ ����
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

    //�������
    //���������� ���������� ��������
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    // ��������� ���� � ������ ��������
    qmPath = qApp->applicationDirPath() + "/translations";

    // ���������� ������ � progressBar
    connect(myTimer, &Timer::step,
            ui->progressBar, &QProgressBar::setValue);

    //���������� ����� ��������
//    myDialogSettings = new DialogSettings(); //�������� ����� ��������
//    connect(ui->actionSettings, SIGNAL(triggered()),
//            myDialogSettings, SLOT(show())); //���������� ����������� ����� ��������
//    isAddUp = myDialogSettings->isAddUpModeOn;

    //����� ��������
//    myDialogDiagram = new DialogDiagram();
//    connect(ui->actionDiagram1, SIGNAL(triggered()),
//            myDialogDiagram, SLOT(show()));

    //����� "� ���������"
//    myDialogAbout = new DialogAbout();
//    connect(ui->actionAbout, SIGNAL(triggered()),
//            myDialogAbout, SLOT(show()));

    //��������� �������� ComboBox
    ui->tableWidget->setItemDelegateForColumn(3, new ComboBox());

    //��������� �������� ����
    settings = new QSettings("MGSU", "Monitors_QT", this); //���������� �������� �������� ����
    loadSettings(); //�������� �������� ��� �������� ����(������ ����, ...)
    ui->tableWidget->setDragDropMode(QAbstractItemView::InternalMove);

    //��������� ����������
    ui->tableWidget->setSortingEnabled(1);//��������� ����������
}

///
/// \brief ���������� �������� ����
///
MainWindow::~MainWindow()
{
    saveSettings(); //���������� �������� ��� �������� ����
    delete myTimer;
    delete ui;
}

///
/// \brief ��������� ������� ������������ ����
/// \param pos
///
void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* ������� ������ ������������ ���� */
       QMenu * menu = new QMenu(this);

       /* ������������� �������� � ���� */
       menu->addAction(ui->actionOpen);
       menu->addAction(ui->actionSave);
       menu->addAction(ui->actionSaveAs);
       menu->addAction(ui->actionAdd);
       menu->addAction(ui->actionDelete);
       /* �������� ����������� ���� */
       menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

///
/// \brief ��������� ������ ��������� ���������� � ���������� �������
/// \param filePath
///
QList<TMonitor> MainWindow::SetData(const QString filePath)
{            
    QList<TMonitor> _mainList;
    SetHeaders(QStringList()
               << ("id") //trUtf8("id")
               << ("Manufacturer")
               << ("Model")
               << ("Type of Matrix")
               << ("Resolution")
               << ("Diagonal")
               << ("Frame rate")
               << ("Weight")
               << ("Loudspeakers")
               << ("Price"));

    ReadFile(&_mainList, filePath);

    SetTableData(_mainList);
    return _mainList;
}

///
/// \brief ��������� ������� ������� �� ������
/// \details ������� ��������� ������� ������ � tableWidget, ������� �� ������
/// � �������� ��������� ������� �� ����������� ������ QList
/// \param _mainList
///
void MainWindow::SetTableData(QList<TMonitor> &_mainList)
{
    for(int i = 0; i < _mainList.count(); i++){
       if (ui->tableWidget->rowCount() < _mainList.count()) { //��������� ���������� ������ �����...
        ui->tableWidget->insertRow(i); //...(�.�. ������ 2�� ����� ������� � ������ ������)
       }

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QVariant(i+1).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(_mainList[i].getManufacturer()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(_mainList[i].getModel()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(_mainList[i].getMatrixType()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem
                                 (QVariant(_mainList[i].getResolution().widht).toString()
                          + ":" + QVariant(_mainList[0].getResolution().height).toString()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem
                                 (QVariant(_mainList[i].getDiagonal()).toString()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem
                                 (QVariant(_mainList[i].getFrameRate()).toString()));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem
                                 (QVariant(_mainList[i].getWeight()).toString()));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem
                                 (QVariant(_mainList[i].getLoudspeakers()).toString()));
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem
                                 (QVariant(_mainList[i].getPrice()).toString()));
    }

    //�������� ������ ����� ��� �������� ����� ������ �����������
    if (ui->tableWidget->rowCount() > _mainList.count()){
        for (int i = ui->tableWidget->rowCount(); i > _mainList.count(); i--){
            ui->tableWidget->removeRow(i-1);
        }
    }
}

///
/// \brief ��������� ������ �� ������� �������� ����
/// \details ������� ��������� ������ �� ������ ������ �������
/// � ���������� �� � ������ QList
/// \param saveList
///
void MainWindow::ReadTable(QList<TMonitor> *saveList)
{
    TMonitor _tempMonitor;
    TMonitor::TResolution _tempResolution;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
    _tempMonitor.setManufacturer(ui->tableWidget->item(i,1)->text());
    _tempMonitor.setModel(ui->tableWidget->item(i,2)->text());
    _tempMonitor.setMatrixType(ui->tableWidget->item(i,3)->text());
        _tempResolution.widht = ui->tableWidget->item(i,4)->text().left(4).toInt();
        _tempResolution.height = ui->tableWidget->item(i,4)->text().right(4).toInt();
    _tempMonitor.setResolution(_tempResolution);
    _tempMonitor.setDiagonal(ui->tableWidget->item(i,5)->text().toInt());
    _tempMonitor.setFrameRate(ui->tableWidget->item(i,6)->text().toInt());
    _tempMonitor.setWeight(ui->tableWidget->item(i,7)->text().toFloat());
    _tempMonitor.setLoudspeakers(
                QVariant(ui->tableWidget->item(i,8)->text()).toBool());
    _tempMonitor.setPrice(ui->tableWidget->item(i,9)->text().toInt());
    saveList->push_back(_tempMonitor);
    }
}

///
/// \brief ��������� ������ � ������� ����(���������)
///
void MainWindow::on_actionSave_triggered()
{
    QList<TMonitor> _saveList;
    QString filePath = MainWindow::filename;

    ReadTable(&_saveList);
    WriteFile(&_saveList, filePath);
    myTimer->start(100, 1);
}


///
/// \brief ����: �������� ���� ��������
/// \details ������� ��������� ���� ��������, � ����� �������� ��������
/// ������� ���������� ���������� �������� � ������� ���� (SettingsPreset).
///
void MainWindow::on_actionSettings_triggered()
{
    DialogSettings _dialogWindow;
    _dialogWindow.setModal(true);
    _dialogWindow.exec();

    SettingsPreset(_dialogWindow);
}

///
/// \brief ������������� �������� � ���������� ��������� ��� �������� ����
/// \param _dialogSettingWindow
///
void MainWindow::SettingsPreset(DialogSettings &_dialogSettingWindow)
{
    isAddUp = _dialogSettingWindow.isAddUpModeOn;
    isDarkMode = _dialogSettingWindow.isDarkModeOn;
    isSelectItem = _dialogSettingWindow.isItemSelectMode;

    //��������� ���� �������� ����
    if (isDarkMode == true){
        this->setStyleSheet("color: white;"
                            "selection-color: white;"
                            "background-color: gray;"
                            "selection-background-color: lightgray;");
        ui->tableWidget->setStyleSheet("QHeaderView::section { background-color:darkgray }");
        ui->lineEdit->setStyleSheet("QLineEdit { background-color: lightgray }");

//        QPalette p(ui->centralwidget->palette());
//        p.setColor(QPalette::Background, QColor(122,222,222,240));
//        ui->centralwidget->setAutoFillBackground(true);
//        ui->centralwidget->setPalette(p);
    }
    else if(isDarkMode == false){
        this->setStyleSheet("color: black;"
                            "selection-color: white;"
                            "selection-background-color: blue;"
                            "background-color: white;"
                            "border-color: gray;");
        ui->centralwidget->setStyleSheet("background-color: white;");
        ui->tableWidget->setStyleSheet("QHeaderView::section { background-color: white }");
        ui->lineEdit->setStyleSheet("QLineEdit { background-color: white }");
    }

    //��������� ������ ������ � �������
    if (isSelectItem == true){
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    else if (isSelectItem == false){
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

///
/// \brief ���������(���) ������ � ����� ����
///
void MainWindow::on_actionSaveAs_triggered()
{
    QList<TMonitor> _saveList;
    QString filePath
            = QFileDialog::getSaveFileName(this,
                            tr("QFileDialog::getOpenFileName()"),
                            QDir::homePath(),
                            tr("All Files (*);Text Files (*.txt *.log)"));
    ReadTable(&_saveList);
    WriteFile(&_saveList, filePath);
    myTimer->start(100, 1);
}

///
/// \brief ������������� ��������� ������� �������� ����
/// \param headers
///
void MainWindow::SetHeaders(const QStringList &headers)
{
    ui->tableWidget->setColumnCount(10); //��������� ����� �������
    ui->tableWidget->setShowGrid(true); //�������� ��������� �����

    //�������� ����� ��������� ������ ������ �������
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //�������� ���������� ���������
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //������������� ��������� �������
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //����������� ��������� ������� �� �� ����
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //�������� ������� �������
    ui->tableWidget->hideColumn(0);
}

///
/// \brief �������� ������ �� ������� � �������� �� ������
/// \return
///
QList<TMonitor> MainWindow::SetDiagramData()
{
    QList<TMonitor> _diagramList;

    ReadTable(&_diagramList);
    return _diagramList;
}

///
/// \brief ��������� ������ �� ����� � ���������� � ������
/// \param tempList
/// \param filePath
///
void MainWindow::ReadFile(QList<TMonitor> *tempList, const QString filePath)
{
    TMonitor rMonitor;//��������� ������
    TMonitor::TResolution _resolution{0, 0};

    QFile file(filePath);//(QCoreApplication::applicationDirPath() + "/monitorcopy.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd()){
            //������ ������
            QString str = file.readLine();
            //����� �� �����
            QStringList lst = str.split(";");
            //����� ��� �����

            rMonitor.setManufacturer(lst.at(0));
            rMonitor.setModel(lst.at(1)); //erase ����� 1-��
            rMonitor.setMatrixType(lst.at(2));
                _resolution.widht = lst.at(3).toInt();
                _resolution.height = lst.at(4).toInt();
            rMonitor.setResolution(_resolution);
            rMonitor.setDiagonal(lst.at(5).toInt()); //QVariant(lst.at(4).toString());
            rMonitor.setFrameRate(lst.at(6).toInt());
            rMonitor.setWeight(qRound(lst.at(7).toFloat()));//QString::number(lst.at(7).toFloat(), 'f', 3));
            rMonitor.setLoudspeakers(bool(lst.at(8).toInt()));//or static_cast<QVariant>(str).toBool();
            rMonitor.setPrice(lst.at(9).toInt());
            tempList->push_back(rMonitor);
        }
    }
    else {
        qDebug() << "OpenFileError";
    }
    file.close();
}

///
/// \brief ��������� ���� ������� �� ������
/// \param tempList
/// \param filePath
///
void MainWindow::WriteFile(QList<TMonitor> *tempList, const QString filePath)
{
    TMonitor tempMonitor;
    QList<TMonitor> tList = *tempList;

    QFile file(filePath);//(QCoreApplication::applicationDirPath() + "/monitorcopy.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&file); // ������� ������ ������ QTextStream
        // � �������� ��� ����� ������� fileOut
        for (int i = 0; i < ui->tableWidget->rowCount(); i++){
            tempMonitor = tList[i];
            writeStream << tempMonitor.getManufacturer() << ";"
                        << tempMonitor.getModel() << ";"
                        << tempMonitor.getMatrixType() << ";"
                        << tempMonitor.getResolution().widht << ";"
                        << tempMonitor.getResolution().height << ";"
                        << tempMonitor.getDiagonal() << ";"
                        << tempMonitor.getFrameRate() << ";"
                        << tempMonitor.getWeight() << ";"
                        << tempMonitor.getLoudspeakers() << ";"
                        << tempMonitor.getPrice() << "\n";
        }
    }
    else {
        qDebug() << tr("OpenFileError");
    }
    file.close();
}

///
/// \brief ��������� ��������� �������� ���� � �������
///
void MainWindow::saveSettings()
{
    settings->setValue("title", windowTitle());
    settings->setValue("geometry", geometry());
}

///
/// \brief ��������� ��������� �������� ���� �� �������
///
void MainWindow::loadSettings()
{
    setWindowTitle(settings->value("title", "Monitor's base").toString());
    setGeometry(settings->value("geometry", QRect(100, 100, 800, 600)).toRect());
}

///
/// \brief ����: �������� ����� � ����� ���������� �������
///
void MainWindow::on_actionOpen_triggered()
{

    MainWindow::filename
            = QFileDialog::getOpenFileName(this,
                            tr("QFileDialog::getOpenFileName()"),
                            QDir::homePath(),
                            tr("All Files (*);Text Files (*.txt *.log)"));
    //QFileDialog::DontUseNativeDialog
    QString tempFilename = MainWindow::filename;
    if (!tempFilename.isEmpty()) {
        //ui->labelGetOpenFileName->setText(fileName);
        ui->statusbar->showMessage("Success: File opened!");
        ui->tableWidget->clear();
        this->SetHeaders(QStringList()
                         << ("id") //trUtf8("id")
                         << ("Manufacturer")
                         << ("Model")
                         << ("Type of Matrix")
                         << ("Resolution")
                         << ("Diagonal")
                         << ("Frame rate")
                         << ("Weight")
                         << ("Loudspeakers")
                         << ("Price"));
        myTimer->start(100, 1);
        this->SetData(tempFilename);
    }
    else {
        ui->statusbar->showMessage(tr("Error: File not opened!"));
    }
//    for (int i = 0; i <= 100 ; i=i+10) {
//        delay(100);
//        ui->progressBar->setValue(i);
//    }
}

///
/// \brief ����: �������� ������ �� �������
///
void MainWindow::on_actionDelete_triggered()
{
    if (isSelectItem == false){
    //������� ������ �������� ���������� �����
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedRows.count() != 0){
        //������� ������ ������� � ���� ������ (�.�. ���������� ������ ���� ������)
        ui->tableWidget->removeRow(selectedRows[0].row());
        ui->statusbar->showMessage(tr("Delete: row"));
        }
    else {
        ui->statusbar->showMessage(tr("Not selected"));
    }
    }
    else if(isSelectItem == true){
    ui->tableWidget->setItem(ui->tableWidget->currentRow(),
                             ui->tableWidget->currentColumn(),
                             new QTableWidgetItem(""));
    }
}

///
/// \brief ����: ���������� ������ ������ � �������
///
void MainWindow::on_actionAdd_triggered()
{
    //DialogSettings dialogform(this);
    //DialogSettings *dialogform = new DialogSettings(this);
    SetHeaders(QStringList()
               << ("id") //trUtf8("id")
               << ("Manufacturer")
               << ("Model")
               << ("Type of Matrix")
               << ("Resolution")
               << ("Diagonal")
               << ("Frame rate")
               << ("Weight")
               << ("Loudspeakers")
               << ("Price"));
    //������� ������ �������� ���������� ����� TODO ������ �� ������� �����
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();

    if (!selectedRows.isEmpty()){
        if (isAddUp == true){
            //��������� ������ ����� ���������� �������
            ui->tableWidget->insertRow(selectedRows[0].row());
            ui->statusbar->showMessage(tr("Add: row"));
        }
        else {
            //��������� ������ ����� ���������� ������
            ui->tableWidget->insertRow(selectedRows[0].row() + 1);
            ui->statusbar->showMessage(tr("Add: row"));
        }
    }
    else {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    }
    //delete dialogform;
}

///
/// \brief ����: ��������� �������� �����
///
void MainWindow::on_actionRussian_triggered()
{
    /// ��������� ��� ������� ������� ������ ������������
    appTranslator.load("Monitors_QT_ru_RU", qmPath);
    /**
     * ��� ��������� ����������� ��������� Qt ����� ��������� � �������
     * �� ��������������� ����� ����� "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_ru_RU.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * ������� "retranslateUi()" ��� ���������� ���������� ����������
     * ��������� ����������� �� ���������������� ����� "qm" ����
     */
    ui->retranslateUi(this);
}

///
/// \brief ����: ��������� ����������� �����
///
void MainWindow::on_actionEnglish_triggered()
{
    /// ��������� ��� ������� ������� ������ ������������
    appTranslator.load("Monitors_QT_en_US", qmPath);
    /**
     * ��� ��������� ����������� ��������� Qt ����� ��������� � �������
     * �� ��������������� ����� ����� "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_en_US.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * ������� "retranslateUi()" ��� ���������� ���������� ����������
     * ��������� ����������� �� ���������������� ����� "qm" ����
     */
    ui->retranslateUi(this);
}

///
/// \brief ����: ��������� �������� �����
///
void MainWindow::on_actionFinland_triggered()
{
    /// ��������� ��� ������� ������� ������ ������������
    appTranslator.load("Monitors_QT_fi_FN", qmPath);
    /**
     * ��� ��������� ����������� ��������� Qt ����� ��������� � �������
     * �� ��������������� ����� ����� "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_fi_FN.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * ������� "retranslateUi()" ��� ���������� ���������� ����������
     * ��������� ����������� �� ���������������� ����� "qm" ����
     */
    ui->retranslateUi(this);
}

///
/// \brief ����: ����� ������ � ���������
///
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Author: Maxim Morozov        \n             2021"));
}

///
/// \brief ����: ����� �� �������������
///
void MainWindow::on_pushButton_Search_clicked()
{
    bool error = false;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++){
        if (!ui->tableWidget->item(i, 1) ||
            !ui->tableWidget->item(i, 2) ||
            !ui->tableWidget->item(i, 6)){
            error = true;
        }
    }

    if (error == false){
    QString searchLine = ui->lineEdit->text();
    QList<TMonitor> _searchList;// = SetData(filename);
    QList<TMonitor> _searchedList;

    ReadTable(&_searchList);

    for (int i = 0; i < _searchList.count(); i++){
        if (searchLine == _searchList[i].getManufacturer() ||
            searchLine == _searchList[i].getModel() ||
            searchLine == _searchList[i].getMatrixType()){
            _searchedList.push_back(_searchList[i]);
        }
    }
    myTimer->start(100, 1);
    SetTableData(_searchedList);

    if (ui->lineEdit->text().isEmpty()){
        myTimer->start(100, 1);
        SetData(filename);
    }

    }
    else {
        ui->statusbar->showMessage("Error: Delete empty strings");
    }
}

///
/// \brief ����: ��������� ������������� ������� �����������
///
void MainWindow::on_actionDiagram1_triggered()
{
    bool error = false;
    for (int i = 0; i < ui->tableWidget->rowCount(); i++){
        if (!ui->tableWidget->item(i, 1) ||
            !ui->tableWidget->item(i, 2) ||
            !ui->tableWidget->item(i, 6)){
            error = true;
        }
    }

    if (error == false){
        DialogDiagram myDialogDiagram;
        myDialogDiagram.setDiagremList(SetDiagramData());
        //myDialogDiagram.paintEvent(SetDiagramData());
        myDialogDiagram.setModal(true);
        myDialogDiagram.exec();
    }
    else {
        ui->statusbar->showMessage("Error: Delete empty strings");
    }
}

///
/// \brief ����: ��������� ��������� ������� ��������������
///
void MainWindow::on_actionChart1_triggered()
{
    struct chartElement{ //��������� ��� ������ ��������� ���������
        QString brand;
        int countOf;
    } currentElem;

    bool error = false;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++){
        if (!ui->tableWidget->item(i, 1) ||
            !ui->tableWidget->item(i, 2) ||
            !ui->tableWidget->item(i, 6)){
            error = true;
        }
    }

    if (error == false){
    QList<TMonitor> _monitorList; //= SetData(filename);
    ReadTable(&_monitorList);
    QList<chartElement> _chartList; //������ ��������� ��� ���������
    bool exist = false;

    //���������� ������ �������������� � �� ����������
    for(int i = 0; i < _monitorList.count(); i++){ exist = false;
        for(int j = 0; j < _chartList.count(); j++){
            if (_chartList[j].brand == _monitorList[i].getManufacturer()){
               _chartList[j].countOf++;
               exist = true;
            }
        }
        if (exist == false){
                currentElem.brand = _monitorList[i].getManufacturer();
                currentElem.countOf = 1;
                _chartList.push_back(currentElem);
        }
    }

    //���������� ������ �������
        QPieSeries *series = new QPieSeries();
        for (int i = 0; i < _chartList.count(); i++){
            series->append(_chartList[i].brand, _chartList[i].countOf);
        }

    //��������� ���������� ������� �� ����� (pie)
        for (int i = 0; i < _chartList.count(); i++){
        QPieSlice *slice = series->slices().at(i);
        slice->setExploded();
        slice->setLabelVisible();
        //slice->setPen(QPen(Qt::darkGreen, 2));
        //slice->setBrush(Qt::green);
        }

    //�������� ������ ������� � ��������� ��������
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(tr("Manufacturer popularity graph"));
        //chart->legend()->hide();

    //��������� ������� �� �������
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

    //����� ���� �� �����������
        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->show();
    }
    else {
        ui->statusbar->showMessage("Error: Delete empty strings");
    }
}
