#include "dialogsettings.h"
#include "ui_dialogsettings.h"

///
/// @page
/// \brief ����������� ���� ��������
/// \param parent
///
DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

///
/// \brief ���������� ���� ��������
///
DialogSettings::~DialogSettings()
{
    //AddMode();
    delete ui;
}

///
/// \brief ����: ��������� �������� ������ ���������� ������
///
void DialogSettings::on_radioButtonAddUp_clicked()//bool &isAddUpModeOn)
{
    DialogSettings::isAddUpModeOn = true;
}

///
/// \brief ����: ��������� �������� ������ ���������� ������
///
void DialogSettings::on_radioButtonAddDown_clicked()//bool &isAddUpModeOn)
{
    DialogSettings::isAddUpModeOn = false;
}

///
/// \brief ����: ��������� �������� �������� ����
///
void DialogSettings::on_radioButtonThemeWhite_clicked()
{
    DialogSettings::isDarkModeOn = false;
}

///
/// \brief ����: ��������� �������� �������� ����
///
void DialogSettings::on_radioButtonThemeDark_clicked()
{
    DialogSettings::isDarkModeOn = true;
}

///
/// \brief ����: ��������� �������� ������ ������ ���������
///
void DialogSettings::on_radioButtonSelectString_clicked()
{
    DialogSettings::isItemSelectMode = false;
}

///
/// \brief ����: ��������� �������� ������ ������ ���������
///
void DialogSettings::on_radioButtonSelectItem_clicked()
{
    DialogSettings::isItemSelectMode = true;
}
