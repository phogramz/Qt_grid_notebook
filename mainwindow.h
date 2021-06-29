#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tmonitor.h"
#include "dialogsettings.h"
#include "dialogdiagram.h"
#include "comboboxdelegate.h"

#include <QMainWindow>
#include <QSettings>
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QActionGroup;
class Timer;

///
/// @mainclass
/// \brief Класс основного окна
/// \details Класс главного окна формы, отображает получаемые данные из файлов,
/// а также обрабатывает все сигналы с окна, вызывая слоты описанные в файле mainwindow.cpp
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString filename;
    bool isAddUp;
    bool isDarkMode;
    bool isSelectItem;

    void ReadFile(QList<TMonitor> *tempList, const QString filePath);
    void WriteFile(QList<TMonitor> *tempList, const QString filePath);
    void ReadTable(QList<TMonitor> *saveList);

    void saveSettings();
    void loadSettings();

//public slots:
//    void receiveData(QString str);

private slots:

    void slotCustomMenuRequested(QPoint pos);

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSettings_triggered();

    void on_actionDelete_triggered();

    void on_actionAdd_triggered();

    void on_actionSaveAs_triggered();

    //void on_actionAbout_triggered();

    void on_actionDiagram1_triggered();

    //void switchLanguage(QAction *action);

    void on_actionRussian_triggered();

    void on_actionEnglish_triggered();

    void on_actionFinland_triggered();

    void on_actionAbout_triggered();

    void on_pushButton_Search_clicked();

    void on_actionChart1_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    QTranslator translator;
/////////////////////////////
    QTranslator appTranslator;
    QTranslator qtTranslator;

    /// Путь к файлам перевода "*.qm"
    QString qmPath;

    /// Окно настроек
    DialogSettings *myDialogSettings;

    /// Таймер для progressBar
    Timer *myTimer;

private:

    QList<TMonitor> SetData(const QString filePath);
    void SetTableData(QList<TMonitor> &_mainList);
    void SetHeaders(const QStringList &headers);
    QList<TMonitor> SetDiagramData();

    void SettingsPreset(DialogSettings &_dialogSettingWindow);
};
#endif // MAINWINDOW_H
