/**
 * 后台线程池，管理所有后台任务，主要是从HTTP下载数据，包括报价，分时价格
*/
#ifndef NETTHREADPOOL_H
#define NETTHREADPOOL_H

#include <QThreadPool>
#include <QMutex>
#include <QMap>
#include "stocklaststatus.h"
//#include "stockminutechart.h"
#include "managerrunnable.h"
#include "data/stockdayminute.h"
//需要下载数据的类型
#define UPDATE_MINUTE   0x01    //分时价格数据
#define UPDATE_PRICE    0x02    //价格
//窗口类型
#define WND_FALG_TODAY  0x01
#define WND_FALG_MAIN   0x02
class StockListItem;

class TodayMinuteRunnable;
class TodayPriceRunnable;
class StockLastStatus;

namespace data{
class DataLoader;
}
namespace minutechart
{
class StockDayMinute;
}

struct UpdateItem
{
    const StockListItem* stock;
    //uint32_t flag;
    uint32_t wnd;
};

UpdateItem* newUpdateItem(const StockListItem* stock,uint32_t wnd);
class NetThreadPool : public QThreadPool
{
    Q_OBJECT
public:
    NetThreadPool(data::DataLoader* dataLoader);
    virtual ~NetThreadPool();

    //增加股票到任务
    void addStock(const StockListItem*,uint32_t flag,uint32_t wnd);
    void addStock(const QList<const StockListItem*>&,uint32_t flag,uint32_t wnd);
    void addStock(const QList<StockListItem*>&,uint32_t flag,uint32_t wnd);

    //从任务中移除
    void removeStock(const StockListItem*,uint32_t flag,uint32_t wnd);
    void removeStock(const QList<const StockListItem*>&,uint32_t flag,uint32_t wnd);
    void removeStock(const QList<StockListItem*>&,uint32_t flag,uint32_t wnd);

    void stockDayMinuteUpdate(const StockListItem*,minutechart::StockDayMinute* day);
    int getWaitTimer(uint32_t taskType)const;
    const StockListItem* nextMinute();
    void getPricePage(QList<const StockListItem*>& stock,int maxSize=10);
    inline bool isExit(){return bExit;}

    void combineStockItem();
    void lock()
    {
        stockMutex.lock();
    }
    void unlock()
    {
        stockMutex.unlock();
    }
    inline QList<StockLastStatus*>& getLastStatus()
    {
        return lastStatus;
    }
    StockLastStatus* findStockListStatus(const StockListItem* stock);
    StockLastStatus* newStockLastStatus(const StockListItem* stock);
    void onLastStatusUpdated(const QList<StockLastStatus*>& status);

    void removeLastStatus(const QList<UpdateItem*>& realRemove);

    void removeDayMinute(const QList<UpdateItem*>& realRemove);
    minutechart::StockDayMinute* getStockDayMinute(const StockListItem*);
    void clearStockDay();

protected:
    bool bExit;
    QMutex stockMutex;
    ManagerRunnable* manager;
    TodayMinuteRunnable* minuteRun;
    TodayPriceRunnable* priceRun;
    data::DataLoader* pDataLoader;

    QMap<const StockListItem*,minutechart::StockDayMinute*> stockDays;

    QList<StockLastStatus*> lastStatus;
    //正在更新的
    //QList<const StockListItem*> minuteUpdating;
    //QList<const StockListItem*> priceUpdating;

    //待删除
    QList<UpdateItem*> minuteremoving;
    QList<UpdateItem*> priceremoving;

    //任务
    QList<UpdateItem*> minute;
    QList<UpdateItem*> prices;

    //待增加
    QList<UpdateItem*> minuteAdding;
    QList<UpdateItem*> pricesAdding;

signals:
    //void updated(QList<const StockListItem*>*,uint32_t flag);
    void lastStatusChanged(const QList<StockLastStatus*>&);
    void dayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*);
};

void clearUpdateList(QList<UpdateItem*>&);
QList<UpdateItem*>::iterator findStockFromUpdateItems(QList<UpdateItem*>& items,const StockListItem* stock);
void addToStockList(QList<UpdateItem*>& dest,const QList<StockListItem*>& adds,uint32_t wnd);
//得到确实需要删除的，有些是重复的，可能不需要删除
void getRemoveListFromList(QList<UpdateItem*>& src,QList<UpdateItem*>& remove,QList<UpdateItem*>& realRemove);
void releaseRemoveUpdateItem(QList<UpdateItem*>& src,QList<UpdateItem*>& realRemove);

void addToStockList(QList<UpdateItem*>& dest,const QList<const StockListItem*>& adds,uint32_t wnd);


bool find_update_by_stock(UpdateItem* p,const StockListItem* stock);

void addUpdateItem(QList<UpdateItem*>& dest,QList<UpdateItem*>& src);
//extern NetThreadPool net;
#endif // NETTHREADPOOL_H
