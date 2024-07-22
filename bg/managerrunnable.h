/*
 * 负责把前台数据提交到后台，目的是防止前台卡顿
*/

#ifndef MANAGERRUNNABLE_H
#define MANAGERRUNNABLE_H

#include <QRunnable>
class NetThreadPool;
class ManagerRunnable : public QRunnable
{
public:
    ManagerRunnable(NetThreadPool* net);
    virtual void run();

protected:
    NetThreadPool* netPool;
};

#endif // MANAGERRUNNABLE_H
