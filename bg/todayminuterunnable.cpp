#include "todayminuterunnable.h"
#include <QThread>
#include "netthreadrequest.h"
#include "stocklistitem.h"
#include "netthreadpool.h"
#include "sohudataparser.h"


//#include "stockminutechart.h"
TodayMinuteRunnable::TodayMinuteRunnable(NetThreadPool* pool,data::DataLoader* dataLoader)
    :BgRunnable(pool,dataLoader)
{

}

// void TodayMinuteRunnable::setStock(const StockListItem* newStock)
// {
//     pStock=newStock;
// }

void TodayMinuteRunnable::run()
{
    //reply=networkManager->get(QNetworkRequest(url))
    //QThread::sleep(1);
    NetThreadRequest request;
    datasohu::DataParser parser;
    QDateTime curDate;
    uint32_t wait;
    minutechart::StockDayMinute* day;
    parser.setDataLoader(pDataLoader);
    while(!pool->isExit())
    {
        const StockListItem* pStock=pool->nextMinute();
        if(pStock==NULL)
        {
            QThread::msleep(100);
            continue;
        }
        wait=pool->getWaitTimer(UPDATE_MINUTE);
        curDate=QDateTime::currentDateTime();
        QString codeHead=pStock->getCode().mid(3);
        QString url=QString("https://hq.stock.sohu.com/cn/%1/cn_%2-4.html?openinwebview_finance=false&t=%3&_=%3")
                          .arg(codeHead,pStock->getCode())
                          .arg(curDate.toMSecsSinceEpoch());

        request.setData1((uint64_t)pStock);
        request.request(url);
        day=pool->getStockDayMinute(pStock);
        parser.parseMinuteData(request.data(),day);
        pool->stockDayMinuteUpdate(pStock,day);

        while((wait--)>0&&!pool->isExit())
            QThread::msleep(100);
        //QThread::msleep(5000);
    }
}
