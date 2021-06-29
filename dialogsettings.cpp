#include "dialogsettings.h"
#include "ui_dialogsettings.h"

///
/// @page
/// \brief Конструктор окна настроек
/// \param parent
///
DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

///
/// \brief Деструктор окна настроек
///
DialogSettings::~DialogSettings()
{
    //AddMode();
    delete ui;
}

///
/// \brief Слот: Обработка настроек режима добавления записи
///
void DialogSettings::on_radioButtonAddUp_clicked()//bool &isAddUpModeOn)
{
    DialogSettings::isAddUpModeOn = true;
}

///
/// \brief Слот: Обработка настроек режима добавления записи
///
void DialogSettings::on_radioButtonAddDown_clicked()//bool &isAddUpModeOn)
{
    DialogSettings::isAddUpModeOn = false;
}

///
/// \brief Слот: Обработка настроек выбраной темы
///
void DialogSettings::on_radioButtonThemeWhite_clicked()
{
    DialogSettings::isDarkModeOn = false;
}

///
/// \brief Слот: Обработка настроек выбраной темы
///
void DialogSettings::on_radioButtonThemeDark_clicked()
{
    DialogSettings::isDarkModeOn = true;
}

///
/// \brief Слот: Обработка настроек режима выбора элементов
///
void DialogSettings::on_radioButtonSelectString_clicked()
{
    DialogSettings::isItemSelectMode = false;
}

///
/// \brief Слот: Обработка настроек режима выбора элементов
///
void DialogSettings::on_radioButtonSelectItem_clicked()
{
    DialogSettings::isItemSelectMode = true;
}
