#include "todaypricerunnable.h"
#include "stocklistitem.h"
#include "netthreadpool.h"
#include "sohudataparser.h"
#include "netthreadrequest.h"

TodayPriceRunnable::TodayPriceRunnable(NetThreadPool* pool,data::DataLoader* dataLoader)
:BgRunnable(pool,dataLoader)
{

}

StockLastStatus* get_Last_Status_by_Stock(const StockListItem* stock,void* p)
{
    TodayPriceRunnable* runnable=(TodayPriceRunnable*)p;
    return runnable->getLastStatus(stock);
}

StockLastStatus* TodayPriceRunnable::getLastStatus(const StockListItem* stock)
{
    StockLastStatus* status=pool->findStockListStatus(stock);
    if(status==NULL)
        status=pool->newStockLastStatus(stock);
    newStatus.push_back(status);
    return status;
}

void TodayPriceRunnable::run()
{
    NetThreadRequest request;
    datasohu::DataParser parser;
    QDateTime curDate;
    uint32_t wait;
    QList<const StockListItem*> updates;
    QString codes;
    parser.setDataLoader(pDataLoader);
    //QList<StockLastStatus*> stockLastStatus;
    while(!pool->isExit())
    {
        updates.clear();
        pool->getPricePage(updates);
        if(updates.isEmpty())
        {
            QThread::msleep(100);
            continue;
        }
        wait=pool->getWaitTimer(UPDATE_MINUTE);
        newStatus.clear();
        curDate=QDateTime::currentDateTime();

        QList<const StockListItem*>::const_iterator it=updates.constBegin(),
            end=updates.constEnd();
        codes.clear();
        for(;it!=end;it++)
        {
            codes.append("cn_"+(*it)->getCode()+",");
        }
        QDateTime curDate=QDateTime::currentDateTime();
        codes=codes.left(codes.length()-1);
        //setRequestValid(true);
        QString url=QString("https://hqm.stock.sohu.com/getqjson?code=%1&cb=fortune_hq_diwei&_=%2").arg(codes).arg(curDate.toMSecsSinceEpoch());

        request.request(url);

        pool->lock();
        parser.parsePriceData(request.data(),pool->getLastStatus(),get_Last_Status_by_Stock,this);
        pool->unlock();
        pool->onLastStatusUpdated(newStatus);
        //parser.parsePriceData(request.data(),stockLastStatus);
        //pool->addStockMinute(pStock,stockLastStatus);
        //stockLastStatus.clear();
        //request.
        /*while(!net.isExit())
        {
            QThread::msleep(100);
            if(request.tryLock())
                break;
        }*/
        // request.lock();
        // request.unlock();
        //wait=50;
        while((wait--)>0&&!pool->isExit())
            QThread::msleep(100);
        //QThread::msleep(5000);
    }
}
