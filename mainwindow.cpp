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

//Графика
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
/// \brief Конструктор главного окна
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , myTimer(new Timer(this))
{   //главное окно
    setlocale(LC_CTYPE, "Russian");
    ui->setupUi(this);
    this->setWindowTitle("Monitor's base");

    //Контекстное меню
    // устанавливаем специальную политику отображения меню
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));

    //перевод
    //Переменные содержащие переводы
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    // Указывает путь к файлам перевода
    qmPath = qApp->applicationDirPath() + "/translations";

    // Подключаем таймер к progressBar
    connect(myTimer, &Timer::step,
            ui->progressBar, &QProgressBar::setValue);

    //диалоговая форма настроек
//    myDialogSettings = new DialogSettings(); //создание формы настроек
//    connect(ui->actionSettings, SIGNAL(triggered()),
//            myDialogSettings, SLOT(show())); //подключаем отображение формы настроек
//    isAddUp = myDialogSettings->isAddUpModeOn;

    //форма графиков
//    myDialogDiagram = new DialogDiagram();
//    connect(ui->actionDiagram1, SIGNAL(triggered()),
//            myDialogDiagram, SLOT(show()));

    //форма "О программе"
//    myDialogAbout = new DialogAbout();
//    connect(ui->actionAbout, SIGNAL(triggered()),
//            myDialogAbout, SLOT(show()));

    //включение делегата ComboBox
    ui->tableWidget->setItemDelegateForColumn(3, new ComboBox());

    //настройки главного окна
    settings = new QSettings("MGSU", "Monitors_QT", this); //сохранение настроек главного окна
    loadSettings(); //загрузка настроек для главного окна(размер окна, ...)
    ui->tableWidget->setDragDropMode(QAbstractItemView::InternalMove);

    //включение сортировки
    ui->tableWidget->setSortingEnabled(1);//включение сортировки
}

///
/// \brief Деструктор главного окна
///
MainWindow::~MainWindow()
{
    saveSettings(); //сохранение настроек для главного окна
    delete myTimer;
    delete ui;
}

///
/// \brief Настройка пунктов контекстного меню
/// \param pos
///
void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
       QMenu * menu = new QMenu(this);

       /* Устанавливаем действия в меню */
       menu->addAction(ui->actionOpen);
       menu->addAction(ui->actionSave);
       menu->addAction(ui->actionSaveAs);
       menu->addAction(ui->actionAdd);
       menu->addAction(ui->actionDelete);
       /* Вызываем контекстное меню */
       menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
}

///
/// \brief Выполняет методы установки заголовков и заполнение таблицы
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
/// \brief Заполняет таблицу данными из списка
/// \details Функция построчно создает ячейки в tableWidget, выделяя им память
/// и заполняя исходными данными из полученного списка QList
/// \param _mainList
///
void MainWindow::SetTableData(QList<TMonitor> &_mainList)
{
    for(int i = 0; i < _mainList.count(); i++){
       if (ui->tableWidget->rowCount() < _mainList.count()) { //избежание добавления пустых строк...
        ui->tableWidget->insertRow(i); //...(т.к. данные 2го файла пишутся в старые строки)
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

    //Удаление лишних строк при открытии файла меньше предыдущего
    if (ui->tableWidget->rowCount() > _mainList.count()){
        for (int i = ui->tableWidget->rowCount(); i > _mainList.count(); i--){
            ui->tableWidget->removeRow(i-1);
        }
    }
}

///
/// \brief Считывает данные из таблицы главного окна
/// \details Функция считывает данный из каждой ячейки таблицы
/// и записывает их в список QList
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
/// \brief Сохраняет данные в текущий файл(обновляет)
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
/// \brief Слот: Открытие окна настроек
/// \details Функция открывает окно настроек, а после закрытия вызывает
/// функцию применения полученных значений в главном окне (SettingsPreset).
///
void MainWindow::on_actionSettings_triggered()
{
    DialogSettings _dialogWindow;
    _dialogWindow.setModal(true);
    _dialogWindow.exec();

    SettingsPreset(_dialogWindow);
}

///
/// \brief Устанавливает выбраные в настройках параметры для главного окна
/// \param _dialogSettingWindow
///
void MainWindow::SettingsPreset(DialogSettings &_dialogSettingWindow)
{
    isAddUp = _dialogSettingWindow.isAddUpModeOn;
    isDarkMode = _dialogSettingWindow.isDarkModeOn;
    isSelectItem = _dialogSettingWindow.isItemSelectMode;

    //Установка темы главного окна
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

    //Установка режима выбора в таблице
    if (isSelectItem == true){
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    else if (isSelectItem == false){
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
}

///
/// \brief Сохраняет(как) данные в новый файл
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
/// \brief Устанавливает заголовки таблицы главного окна
/// \param headers
///
void MainWindow::SetHeaders(const QStringList &headers)
{
    ui->tableWidget->setColumnCount(10); //указываем число колонок
    ui->tableWidget->setShowGrid(true); //включаем видимость сетки

    //Включаем режим выделения только одного объекта
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //Включаем построчное выделение
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //Устанавливаем заголовки колонок
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //Растягиваем последнюю колонку на всё окно
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //Скрываем нулевую колонку
    ui->tableWidget->hideColumn(0);
}

///
/// \brief Получает данные из таблицы и передает на график
/// \return
///
QList<TMonitor> MainWindow::SetDiagramData()
{
    QList<TMonitor> _diagramList;

    ReadTable(&_diagramList);
    return _diagramList;
}

///
/// \brief Считывает данные из файла и записывает в список
/// \param tempList
/// \param filePath
///
void MainWindow::ReadFile(QList<TMonitor> *tempList, const QString filePath)
{
    TMonitor rMonitor;//временный объект
    TMonitor::TResolution _resolution{0, 0};

    QFile file(filePath);//(QCoreApplication::applicationDirPath() + "/monitorcopy.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while(!file.atEnd()){
            //читаем строку
            QString str = file.readLine();
            //делим на слова
            QStringList lst = str.split(";");
            //вывод для теста

            rMonitor.setManufacturer(lst.at(0));
            rMonitor.setModel(lst.at(1)); //erase кроме 1-го
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
/// \brief Заполняет файл данными из списка
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
        QTextStream writeStream(&file); // Создаем объект класса QTextStream
        // и передаем ему адрес объекта fileOut
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
/// \brief Сохраняет настройки главного окна в реестре
///
void MainWindow::saveSettings()
{
    settings->setValue("title", windowTitle());
    settings->setValue("geometry", geometry());
}

///
/// \brief Загружает настройки главного окна из реестра
///
void MainWindow::loadSettings()
{
    setWindowTitle(settings->value("title", "Monitor's base").toString());
    setGeometry(settings->value("geometry", QRect(100, 100, 800, 600)).toRect());
}

///
/// \brief Слот: Открытие файла и вывод заполненой таблицы
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
/// \brief Слот: Удаление записи из таблицы
///
void MainWindow::on_actionDelete_triggered()
{
    if (isSelectItem == false){
    //Создаем список индексов выделенных строк
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();
    if (selectedRows.count() != 0){
        //Удаляем первый элемент в этом списке (т.к. выделяется только одна строка)
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
/// \brief Слот: Добавление пустой строки в таблицу
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
    //Создаем список индексов выделенных строк TODO защита от пустого файла
    QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();

    if (!selectedRows.isEmpty()){
        if (isAddUp == true){
            //Добавляем строку перед выделенной строкой
            ui->tableWidget->insertRow(selectedRows[0].row());
            ui->statusbar->showMessage(tr("Add: row"));
        }
        else {
            //Добавляем строку после выделенной строки
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
/// \brief Слот: Включение русского языка
///
void MainWindow::on_actionRussian_triggered()
{
    /// Загружаем тот перевод который выбрал пользователь
    appTranslator.load("Monitors_QT_ru_RU", qmPath);
    /**
     * Для переводов стандартных сообщений Qt можно запросить у системы
     * их местонахождение через вызов "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_ru_RU.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * Взываем "retranslateUi()" для обновления интерфейса приложения
     * используя загруженный из соответствующего файла "qm" язык
     */
    ui->retranslateUi(this);
}

///
/// \brief Слот: Включение английского языка
///
void MainWindow::on_actionEnglish_triggered()
{
    /// Загружаем тот перевод который выбрал пользователь
    appTranslator.load("Monitors_QT_en_US", qmPath);
    /**
     * Для переводов стандартных сообщений Qt можно запросить у системы
     * их местонахождение через вызов "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_en_US.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * Взываем "retranslateUi()" для обновления интерфейса приложения
     * используя загруженный из соответствующего файла "qm" язык
     */
    ui->retranslateUi(this);
}

///
/// \brief Слот: Включение финского языка
///
void MainWindow::on_actionFinland_triggered()
{
    /// Загружаем тот перевод который выбрал пользователь
    appTranslator.load("Monitors_QT_fi_FN", qmPath);
    /**
     * Для переводов стандартных сообщений Qt можно запросить у системы
     * их местонахождение через вызов "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("Monitors_QT_fi_FN.qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));

    /**
     * Взываем "retranslateUi()" для обновления интерфейса приложения
     * используя загруженный из соответствующего файла "qm" язык
     */
    ui->retranslateUi(this);
}

///
/// \brief Слот: Вывод данных о программе
///
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("Author: Maxim Morozov        \n             2021"));
}

///
/// \brief Слот: Поиск по производителю
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
/// \brief Слот: Отрисовка динамического графика соотношений
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
/// \brief Слот: Отрисовка кругового графика производителей
///
void MainWindow::on_actionChart1_triggered()
{
    struct chartElement{ //структура для списка элементов диаграммы
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
    QList<chartElement> _chartList; //список элементов для диаграммы
    bool exist = false;

    //Заполнение списка производителей и их количество
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

    //Заполнение данных графика
        QPieSeries *series = new QPieSeries();
        for (int i = 0; i < _chartList.count(); i++){
            series->append(_chartList[i].brand, _chartList[i].countOf);
        }

    //Настройка разделения графика на части (pie)
        for (int i = 0; i < _chartList.count(); i++){
        QPieSlice *slice = series->slices().at(i);
        slice->setExploded();
        slice->setLabelVisible();
        //slice->setPen(QPen(Qt::darkGreen, 2));
        //slice->setBrush(Qt::green);
        }

    //Передача данных графику и установка названия
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle(tr("Manufacturer popularity graph"));
        //chart->legend()->hide();

    //Отрисовка графика на виджете
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

    //Вывод окна на отображение
        QMainWindow *window = new QMainWindow;
        window->setCentralWidget(chartView);
        window->resize(800, 600);
        window->show();
    }
    else {
        ui->statusbar->showMessage("Error: Delete empty strings");
    }
}
