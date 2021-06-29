#ifndef TMONITOR_H
#define TMONITOR_H

#include <string>
#include <QAbstractTableModel>

///
/// \brief Класс организации исходных данных
///
class TMonitor
{
 public:
    struct TResolution //разрешение экрана
    {
        int height;
        int widht;
    };
 private: //закрытые основные переменные класса
    //int Id;
    QString manufacturer;
    QString model;
    QString matrixType;
    TResolution resolution;
    int diagonal;
    int frameRate;
    float weight;
    bool loudspeakers;
    int price;
    char *str;

public:
    TMonitor(); //конструктор
    TMonitor(const QString lmanufacturer,
             const QString lmodel,
             const QString lmatrixType,
             const TResolution lresolution,
             const int ldiagonal,
             const int lframeRate,
             const float lweight,
             const bool lloudspeakers,
             const int lprice,
             const char *lstr);
   ~TMonitor(); //деструктор

    //void setID(const int lId);
    //int getID() const;
    void setManufacturer(const QString lmanufacturer);
    QString getManufacturer() const;
    void setModel(const QString lmodel);
    QString getModel() const;
    void setMatrixType(const QString lmatrixType);
    QString getMatrixType() const;
    void setResolution (const TResolution lresolution);
    TResolution getResolution() const;
    void setDiagonal(const int ldiagonal);
    int getDiagonal() const;
    void setFrameRate(const int lframeRate);
    int getFrameRate () const;
    void setWeight (const float lweight);
    float getWeight () const;
    void setLoudspeakers(const bool lloudspeakers);
    bool getLoudspeakers() const;
    void setPrice(const int lprice);
    int getPrice() const;

    void setStr(const char *lstr);
    char* getStr() const;

};

#endif // TMONITOR_H
