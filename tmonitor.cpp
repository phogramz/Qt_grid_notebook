#include "tmonitor.h"
#include <iostream>
#include <cstring>

using namespace std;

///
/// \brief ����������� ������ �������� ������
///
TMonitor::TMonitor():
    //Id(0)
    manufacturer("")
  , model("")
  , matrixType("")
  , resolution({0, 0})
  , diagonal(0)
  , frameRate(0)
  , weight(0)
  , loudspeakers(false)
  , price(0)
  , str(new char)
{

}

///
/// \brief ����������� ������ �������� ������ (������� ������)
/// \param lmanufacturer
/// \param lmodel
/// \param lmatrixType
/// \param lresolution
/// \param ldiagonal
/// \param lframeRate
/// \param lweight
/// \param lloudspeakers
/// \param lprice
/// \param lstr
///
TMonitor::TMonitor(const QString lmanufacturer,
                   const QString lmodel,
                   const QString lmatrixType,
                   const TMonitor::TResolution lresolution,
                   const int ldiagonal,
                   const int lframeRate,
                   const float lweight,
                   const bool lloudspeakers,
                   const int lprice,
                   const char *lstr):
    manufacturer(lmanufacturer)
  , model(lmodel)
  , matrixType(lmatrixType)
  , resolution(lresolution)
  , diagonal(ldiagonal)
  , frameRate(lframeRate)
  , weight(lweight)
  , loudspeakers(lloudspeakers)
  , price(lprice)
  , str(new char[strlen(lstr) + 1])
{
    strcpy(str, lstr);
}

///
/// \brief ���������� ������ �������� ������
///
TMonitor::~TMonitor()
{
    //delete [] str;
}

///
/// \brief ����� ��������� ����� �������������
/// \param lmanufacturer
///
void TMonitor::setManufacturer(const QString lmanufacturer)
{
    manufacturer = lmanufacturer;
}

///
/// \brief ����� ��������� ����� �������������
/// \return
///
QString TMonitor::getManufacturer() const
{
    return manufacturer;
}

///
/// \brief ����� ��������� �������� ������
/// \param lmodel
///
void TMonitor::setModel(const QString lmodel)
{
    model = lmodel;
}

///
/// \brief ����� ��������� �������� ������
/// \return
///
QString TMonitor::getModel() const
{
    return model;
}

///
/// \brief ����� ��������� �������� ���� �������
/// \param lmatrixType
///
void TMonitor::setMatrixType(const QString lmatrixType)
{
    matrixType = lmatrixType;
}

///
/// \brief ����� ��������� �������� ���� �������
/// \return
///
QString TMonitor::getMatrixType() const
{
    return matrixType;
}

///
/// \brief ����� ��������� �������� ���������� ��������
/// \param lresolution
///
void TMonitor::setResolution(const TMonitor::TResolution lresolution)
{
    resolution = lresolution;
}

///
/// \brief ����� ��������� �������� ���������� ��������
/// \return
///
TMonitor::TResolution TMonitor::getResolution() const
{
    return resolution;
}

///
/// \brief ����� ��������� �������� ���������
/// \param ldiagonal
///
void TMonitor::setDiagonal(const int ldiagonal)
{
    diagonal = ldiagonal;
}

///
/// \brief ����� ��������� �������� ���������
/// \return
///
int TMonitor::getDiagonal() const
{
    return diagonal;
}

///
/// \brief ����� ��������� �������� ������� ������
/// \param lframeRate
///
void TMonitor::setFrameRate(const int lframeRate)
{
    frameRate = lframeRate;
}

///
/// \brief ����� ��������� �������� ������� ������
/// \return
///
int TMonitor::getFrameRate() const
{
    return frameRate;
}

///
/// \brief ����� ��������� �������� ����
/// \param lweight
///
void TMonitor::setWeight(const float lweight)
{
    weight = lweight;
}

///
/// \brief ����� ��������� �������� ����
/// \return
///
float TMonitor::getWeight() const
{
    return weight;
}

///
/// \brief ����� ��������� �������� �������
/// \param lloudspeakers
///
void TMonitor::setLoudspeakers(const bool lloudspeakers)
{
    loudspeakers = lloudspeakers;
}

///
/// \brief ����� ��������� �������� �������
/// \return
///
bool TMonitor::getLoudspeakers() const
{
    return loudspeakers;
}

///
/// \brief ����� ��������� �������� ���������
/// \param lprice
///
void TMonitor::setPrice(const int lprice)
{
    price = lprice;
}

///
/// \brief ����� ��������� �������� ���������
/// \return
///
int TMonitor::getPrice() const
{
    return price;
}

///
/// \brief ����� ���������� ��������� ��������� ����������
/// \param lstr
///
void TMonitor::setStr(const char *lstr)
{
    delete [] str;
    str = nullptr;

    str = new char[strlen(lstr) + 1];
    strcpy(str, lstr);
}

///
/// \brief ����� ��������� ��������� ����������
/// \return
///
char *TMonitor::getStr() const
{
    return str;
}
