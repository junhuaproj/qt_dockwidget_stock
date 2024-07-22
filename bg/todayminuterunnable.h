/**
 * 分钟线数据下载线程
*/
#ifndef TODAYMINUTERUNNABLE_H
#define TODAYMINUTERUNNABLE_H

//#include <QObject>
//#include <QRunnable>
#include "bgrunnable.h"


class StockListItem;

class TodayMinuteRunnable : public BgRunnable
{
    //Q_OBJECT
public:
    TodayMinuteRunnable(NetThreadPool* pool,data::DataLoader* dataLoader);
    virtual void run();

};

#endif // TODAYMINUTERUNNABLE_H
