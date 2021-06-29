#ifndef TIMER_H
#define TIMER_H

#include <QObject>

///
/// \brief Класс расчета времени для progressBar
///
class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr);
    void start(int count, int delay);
signals:
    void step(int value);
};

#endif // TIMER_H
