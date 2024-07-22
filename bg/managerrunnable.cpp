/**
 * 调度
*/
#include "managerrunnable.h"
#include "netthreadpool.h"
#include <QThread>

ManagerRunnable::ManagerRunnable(NetThreadPool* newNet)
    :netPool(newNet)
{

}

void ManagerRunnable::run()
{
    while(!netPool->isExit())
    {
        netPool->combineStockItem();
        QThread::msleep(100);
    }
}
