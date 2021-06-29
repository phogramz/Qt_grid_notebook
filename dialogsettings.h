#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogSettings;
}

///
/// \brief ����� ����������� ���� ��������
/// \details ����� ��� ��������� ��������. ��������� ��������� ������� �� ������� ����,
/// � ����������� � �������� ���������� ��� ��������������.
///
class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = nullptr);
    ~DialogSettings();

    bool isAddUpModeOn = false;/// ����� ���������� ����� ������
    bool isDarkModeOn = false;///������ ����
    bool isItemSelectMode = false;///����� ������ ���������

//signals:
//    void sendData(QString str);
//private slots:
//    void onButtonSend();

public:
    Ui::DialogSettings *ui;
private slots:
    void on_radioButtonAddUp_clicked();//(bool &isAddUpModeOn);
    void on_radioButtonAddDown_clicked();//(bool &isAddUpModeOn);
    void on_radioButtonThemeWhite_clicked();
    void on_radioButtonThemeDark_clicked();
    void on_radioButtonSelectString_clicked();
    void on_radioButtonSelectItem_clicked();
};

#endif // DIALOGSETTINGS_H
