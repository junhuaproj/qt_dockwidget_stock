/**
 * 价格下载线程
*/
#ifndef TODAYPRICERUNNABLE_H
#define TODAYPRICERUNNABLE_H

//#include <QObject>
#include <QRunnable>
#include "bgrunnable.h"

#include <QList>

class StockListItem;
class StockLastStatus;
class TodayPriceRunnable : public BgRunnable
{
    //Q_OBJECT
public:
    TodayPriceRunnable(NetThreadPool* pool,data::DataLoader* dataLoader);
    virtual void run();
    StockLastStatus* getLastStatus(const StockListItem* stock);

protected:
    QList<StockLastStatus*> newStatus;
};

#endif // TODAYPRICERUNNABLE_H
