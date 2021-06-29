#include "timer.h"

#include <windows.h>

Timer::Timer(QObject *parent) : QObject(parent)
{

}

void Timer::start(int count, int delay)
{
    int i = 0;
    while (i <= count){
#ifdef _WIN64
        Sleep(delay);
#else
        usleep(delay);
#endif
        emit step(i); //испустить (динамический вызов сигнала)
        i++;
    }
}
