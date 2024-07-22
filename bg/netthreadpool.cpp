#include "netthreadpool.h"
#include "todayminuterunnable.h"
#include "todaypricerunnable.h"
#include <QDebug>
//NetThreadPool net;

#include <functional>
#include <memory>

using namespace std::placeholders;

NetThreadPool::NetThreadPool(data::DataLoader* dataLoader):
    pDataLoader(dataLoader),bExit(false)
{
    minuteRun=new TodayMinuteRunnable(this,pDataLoader);
    manager=new ManagerRunnable(this);
    priceRun=new TodayPriceRunnable(this,pDataLoader);
    priceRun->setAutoDelete(false);
    minuteRun->setAutoDelete(false);
    manager->setAutoDelete(false);
    this->start(minuteRun);
    start(manager);
    start(priceRun);
}

NetThreadPool::~NetThreadPool()
{
    lock();
    bExit=true;
    unlock();
    this->waitForDone();
    delete minuteRun;
    delete manager;
    delete priceRun;
    qDebug()<<tr("thread pool exit");
    ::clearLastStatus(lastStatus);
    //delete priceRun;
}

void NetThreadPool::onLastStatusUpdated(const QList<StockLastStatus*>& status)
{
    emit lastStatusChanged(status);
}
void NetThreadPool::combineStockItem()
{
    lock();
    if(!minuteremoving.isEmpty())
    {
        QList<UpdateItem*> realRemove;
        getRemoveListFromList(minute,minuteremoving,realRemove);
        removeDayMinute(realRemove);
        releaseRemoveUpdateItem(minute,realRemove);
        /*QList<const StockListItem*>::iterator it= minuteremoving.begin(),
            end=minuteremoving.end();
        for(;it!=end;it++)
            minute.removeAll(*it);*/
    }
    if(!priceremoving.isEmpty())
    {
        QList<UpdateItem*> realRemove;

        getRemoveListFromList(prices,priceremoving,realRemove);
        removeLastStatus(realRemove);
        releaseRemoveUpdateItem(prices,realRemove);
        /*QList<const StockListItem*>::iterator it= priceremoving.begin(),
            end=priceremoving.end();
        for(;it!=end;it++)
            prices.removeAll(*it);*/
    }
    if(!minuteAdding.isEmpty())
    {
        addUpdateItem(minute,minuteAdding);
    }
    //minute.append(minuteAdding.constBegin(),minuteAdding.constEnd());
    //minuteAdding.clear();
    clearUpdateList(minuteAdding);

    if(!pricesAdding.isEmpty())
    {
        addUpdateItem(prices,pricesAdding);
    }
    //prices.append(pricesAdding.constBegin(),pricesAdding.constEnd());
    clearUpdateList(pricesAdding);
    //pricesAdding.clear();
    unlock();
}

void NetThreadPool::removeDayMinute(const QList<UpdateItem*>& realRemove)
{
    QList<UpdateItem*>::const_iterator it= realRemove.begin(),
        end=realRemove.end();
    minutechart::StockDayMinute* pDay;
    for(;it!=end;it++)
    {
        if(stockDays.contains((*it)->stock))
        {
            pDay=stockDays.value((*it)->stock);
            if(pDay->isCreate(this))
            {
                delete pDay;
            }
            stockDays.remove((*it)->stock);
        }
    }
}

void NetThreadPool::removeLastStatus(const QList<UpdateItem*>& realRemove)
{
    QList<UpdateItem*>::const_iterator it= realRemove.begin(),
        end=realRemove.end();
    QList<StockLastStatus*>::const_iterator itFind;
    for(;it!=end;it++)
    {
        itFind=findLastStatus(lastStatus,(*it)->stock);
        if(itFind!=lastStatus.end())
        {
            delete (*itFind);
            lastStatus.erase(itFind);
        }
    }
}

const StockListItem* NetThreadPool::nextMinute()
{
    UpdateItem* stock=NULL;
    lock();
    if(!minute.isEmpty())
    {
        stock=minute.at(0);
        minute.pop_front();
        minute.push_back(stock);
    }
    unlock();
    if(stock)
        return stock->stock;
    return NULL;
}
void NetThreadPool::addStock(const StockListItem* stock,uint32_t flag,uint32_t wnd)
{
    lock();
    if(flag&UPDATE_MINUTE)
        minuteAdding.push_back(newUpdateItem(stock,wnd));
    if(flag&UPDATE_PRICE)
        pricesAdding.push_back(newUpdateItem(stock,wnd));
    unlock();
}

void NetThreadPool::addStock(const QList<StockListItem*>& stocks,uint32_t flag,uint32_t wnd)
{
    if(flag&UPDATE_MINUTE)
    {
        //addToStockList(minuteAdding,stocks);
        addToStockList(minuteAdding,stocks,wnd);
        /*QList<StockListItem*>::const_iterator it=stocks.begin(),end=stocks.end();
        for(;it!=end;it++)
            minuteAdding.append(*it);*/
    }
    if(flag&UPDATE_PRICE)
    {
        //addToStockList(pricesAdding,stocks);
        addToStockList(pricesAdding,stocks,wnd);
        ///pricesAdding.append(stocks.begin(),stocks.end());
        /*QList<StockListItem*>::const_iterator it=stocks.begin(),end=stocks.end();
        for(;it!=end;it++)
            pricesAdding.append(*it);
        */
    }
}

void NetThreadPool::addStock(const QList<const StockListItem*>& stocks,uint32_t flag,uint32_t wnd)
{
    if(flag&UPDATE_MINUTE)
    {
        //minuteAdding.append(stocks.begin(),stocks.end());
        addToStockList(minuteAdding,stocks,wnd);
    }
    if(flag&UPDATE_PRICE)
    {
        //pricesAdding.append(stocks.begin(),stocks.end());
        addToStockList(pricesAdding,stocks,wnd);
    }
}

void NetThreadPool::removeStock(const StockListItem* stock,uint32_t flag,uint32_t wnd)
{
    if(flag&UPDATE_MINUTE)
    {
        //minuteremoving.push_back(stock);
        minuteremoving.push_back(newUpdateItem(stock,wnd));
    }
    if(flag&UPDATE_PRICE)
    {
        //priceremoving.push_back(stock);
        priceremoving.push_back(newUpdateItem(stock,wnd));
    }
}
void NetThreadPool::removeStock(const QList<const StockListItem*>& stocks,uint32_t flag,uint32_t wnd)
{
    if(flag&UPDATE_MINUTE)
    {
        //minuteremoving.append(stocks.begin(),stocks.end());
        addToStockList(minuteremoving,stocks,wnd);
    }
    if(flag&UPDATE_PRICE)
    {
        //priceremoving.append(stocks.begin(),stocks.end());
        addToStockList(priceremoving,stocks,wnd);
    }
}

void NetThreadPool::removeStock(const QList<StockListItem*>& stocks,uint32_t flag,uint32_t wnd)
{
    if(flag&UPDATE_MINUTE)
    {
        addToStockList(minuteremoving,stocks,wnd);
        //minuteremoving.append(stocks.begin(),stocks.end());
    }
    if(flag&UPDATE_PRICE)
    {
        addToStockList(priceremoving,stocks,wnd);
        //priceremoving.append(stocks.begin(),stocks.end());
    }
}
UpdateItem* newUpdateItem(const StockListItem* stock,uint32_t wnd)
{
    UpdateItem* item=(UpdateItem*)malloc(sizeof(UpdateItem));
    item->stock=stock;
    //item->flag=flag;
    item->wnd=wnd;
    return item;
}
void addToStockList(QList<UpdateItem*>& dest,const QList<StockListItem*>& adds,uint32_t wnd)
{
    QList<StockListItem*>::const_iterator it=adds.begin(),end=adds.end();
    //UpdateItem* item;
    for(;it!=end;it++)
    {
        //item=(UpdateItem*)malloc(sizeof(UpdateItem));
        //item->stock=*it;
        //item->flag=flag;
        //item->wnd=wnd;
        dest.append(newUpdateItem(*it,wnd));
    }
}
void getRemoveListFromList(QList<UpdateItem*>& src,QList<UpdateItem*>& remove,QList<UpdateItem*>& realRemove)
//void removeFromList(QList<UpdateItem*>& src,QList<const StockListItem*>& remove)
{
    QList<UpdateItem*>::iterator it= remove.begin(),
        end=remove.end();
    QList<UpdateItem*>::iterator itfind;
    //QList<UpdateItem*> tmp;
    for(;it!=end;it++)
    {
        itfind=findStockFromUpdateItems(src,(*it)->stock);
        if(itfind!=src.end())
        {
            (*itfind)->wnd=(*itfind)->wnd&(~(*it)->wnd);
            if((*itfind)->wnd==0)//没有另外使用时，添加到删除链表
                realRemove.append(*itfind);
        }
        //src.removeAll(*it);
    }
    clearUpdateList(remove);

    /*UpdateItem* item;
    while(!tmp.isEmpty())
    {
        item=tmp.back();
        src.removeAll(item);
        tmp.pop_back();
        free(item);
    }*/
}
void releaseRemoveUpdateItem(QList<UpdateItem*>& src,QList<UpdateItem*>& realRemove)
{
    UpdateItem* item;
    while(!realRemove.isEmpty())
    {
        item=realRemove.back();
        src.removeAll(item);
        realRemove.pop_back();
        free(item);
    }
}
void addToStockList(QList<UpdateItem*>& dest,const QList<const StockListItem*>& adds,uint32_t wnd)
{
    QList<const StockListItem*>::const_iterator it=adds.begin(),end=adds.end();
    //UpdateItem* item;
    for(;it!=end;it++)
    {
        //item=(UpdateItem*)malloc(sizeof(UpdateItem));
        //item->stock=*it;
        //item->flag=flag;
        //item->wnd=wnd;
        dest.append(newUpdateItem(*it,wnd));
    }
    /*QList<StockListItem*>::const_iterator it=adds.begin(),end=adds.end();
    for(;it!=end;it++)
    {
        dest.append(newUpdateItem(*it,wnd));
    }*/
}
void NetThreadPool::getPricePage(QList<const StockListItem*>& stock,int maxSize)
{
    lock();
    QList<UpdateItem*> tmp;
    UpdateItem* item;
    while(!prices.isEmpty()&&stock.size()<maxSize)
    {
        item=prices.front();
        stock.push_back(item->stock);
        tmp.push_back(item);
        prices.pop_front();
    }
    //重新添加到后面
    while(!tmp.isEmpty())
    {
        prices.push_back(tmp.front());
        tmp.pop_front();
    }
    unlock();
}

StockLastStatus* NetThreadPool::findStockListStatus(const StockListItem* stock)
{
    QList<StockLastStatus*>::const_iterator it=findLastStatus(lastStatus,stock);
    if(it!=lastStatus.end())
    {
        return *it;
    }
    return NULL;
}

minutechart::StockDayMinute* NetThreadPool::getStockDayMinute(const StockListItem* stock)
{
    minutechart::StockDayMinute* pDay=stockDays.value(stock);
    if(pDay==NULL)
    {
        pDay=new minutechart::StockDayMinute(this,SOHU_FILL_COUNT);
        stockDays.insert(stock,pDay);
    }
    return pDay;
}

StockLastStatus* NetThreadPool::newStockLastStatus(const StockListItem* stock)
{
    StockLastStatus* status=new StockLastStatus();

    status->setPStock(stock);
    lastStatus.push_back(status);
    return status;
}

int NetThreadPool::getWaitTimer(uint32_t taskType)const
{
    switch(taskType)
    {
    case UPDATE_MINUTE:
        if(minute.size()>50)
            return 10;
        else if(minute.size()>10)
            return 20;
        else
            return 30;
        break;
    case UPDATE_PRICE:
        if(prices.size()>500)
            return 10;
        else if(minute.size()>100)
            return 20;
        else
            return 30;
        break;
    }
}

void NetThreadPool::stockDayMinuteUpdate(const StockListItem* stock,minutechart::StockDayMinute* pDay)
{
    lock();
    //delete day;
    emit dayMinuteUpdated(stock,pDay);
    unlock();
}

void NetThreadPool::clearStockDay()
{
    QMap<const StockListItem*,minutechart::StockDayMinute*>::iterator it= stockDays.begin(),
        end=stockDays.end();
    minutechart::StockDayMinute* pDay;
    for(;it!=end;it++)
    {
        pDay=it.value();
        if(pDay->isCreate(this))
            delete pDay;
    }
    stockDays.clear();
}

void clearUpdateList(QList<UpdateItem*>& items)
{
    while(!items.isEmpty())
    {
        free(items.back());
        items.pop_back();
    }
}

void addUpdateItem(QList<UpdateItem*>& dest,QList<UpdateItem*>& src)
{
    QList<UpdateItem*>::const_iterator it= src.begin(),end=src.end();
    QList<UpdateItem*>::iterator itfind;
    QList<UpdateItem*> tmp;
    for(;it!=end;it++)
    {
        itfind=findStockFromUpdateItems(dest,(*it)->stock);
        if(itfind!=dest.end())
        {//已经存在就不在添加
            (*itfind)->wnd|=(*it)->wnd;
            tmp.push_back(*it);
        }
        else//不存在就添加
            dest.push_back(*it);
    }
    clearUpdateList(tmp);
    //直接清空，原数据分两部分，一部分在上一步清除，另一部重新使用
    src.clear();
}

QList<UpdateItem*>::iterator findStockFromUpdateItems(QList<UpdateItem*>& items,const StockListItem* stock)
{
    return std::find_if(items.begin(),items.end(),std::bind(find_update_by_stock,_1,stock));
}

bool find_update_by_stock(UpdateItem* p,const StockListItem* stock)
{
    return p->stock==stock;
}
