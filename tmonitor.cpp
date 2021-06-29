#include "tmonitor.h"
#include <iostream>
#include <cstring>

using namespace std;

///
/// \brief Конструктор класса исходных данных
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
/// \brief Конструктор класса исходных данных (включая методы)
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
/// \brief Деструктор класса исходных данных
///
TMonitor::~TMonitor()
{
    //delete [] str;
}

///
/// \brief Метод установки имени производителя
/// \param lmanufacturer
///
void TMonitor::setManufacturer(const QString lmanufacturer)
{
    manufacturer = lmanufacturer;
}

///
/// \brief Метод получения имени производителя
/// \return
///
QString TMonitor::getManufacturer() const
{
    return manufacturer;
}

///
/// \brief Метод установки названия модели
/// \param lmodel
///
void TMonitor::setModel(const QString lmodel)
{
    model = lmodel;
}

///
/// \brief Метод получения названия модели
/// \return
///
QString TMonitor::getModel() const
{
    return model;
}

///
/// \brief Метод установки значения типа матрицы
/// \param lmatrixType
///
void TMonitor::setMatrixType(const QString lmatrixType)
{
    matrixType = lmatrixType;
}

///
/// \brief Метод получения значения типа матрицы
/// \return
///
QString TMonitor::getMatrixType() const
{
    return matrixType;
}

///
/// \brief Метод установки значения разрешения монитора
/// \param lresolution
///
void TMonitor::setResolution(const TMonitor::TResolution lresolution)
{
    resolution = lresolution;
}

///
/// \brief Метод получения значения разрешения монитора
/// \return
///
TMonitor::TResolution TMonitor::getResolution() const
{
    return resolution;
}

///
/// \brief Метод установки значения диагонали
/// \param ldiagonal
///
void TMonitor::setDiagonal(const int ldiagonal)
{
    diagonal = ldiagonal;
}

///
/// \brief Метод получения значения диагонали
/// \return
///
int TMonitor::getDiagonal() const
{
    return diagonal;
}

///
/// \brief Метод установки значения частоты кадров
/// \param lframeRate
///
void TMonitor::setFrameRate(const int lframeRate)
{
    frameRate = lframeRate;
}

///
/// \brief Метод получения значения частоты кадров
/// \return
///
int TMonitor::getFrameRate() const
{
    return frameRate;
}

///
/// \brief Метод установки значения веса
/// \param lweight
///
void TMonitor::setWeight(const float lweight)
{
    weight = lweight;
}

///
/// \brief Метод получения значения веса
/// \return
///
float TMonitor::getWeight() const
{
    return weight;
}

///
/// \brief Метод установки значения колонок
/// \param lloudspeakers
///
void TMonitor::setLoudspeakers(const bool lloudspeakers)
{
    loudspeakers = lloudspeakers;
}

///
/// \brief Метод получения значения колонок
/// \return
///
bool TMonitor::getLoudspeakers() const
{
    return loudspeakers;
}

///
/// \brief Метод установки значения стоимости
/// \param lprice
///
void TMonitor::setPrice(const int lprice)
{
    price = lprice;
}

///
/// \brief Метод получения значения стоимости
/// \return
///
int TMonitor::getPrice() const
{
    return price;
}

///
/// \brief Метод заполнения параметра служебной информации
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
/// \brief Метод получения служебной информации
/// \return
///
char *TMonitor::getStr() const
{
    return str;
}
