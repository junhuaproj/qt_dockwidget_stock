#include "stocklaststatus.h"
#include <functional>
#include <memory>
using namespace std::placeholders;

StockLastStatus::StockLastStatus() {}

const StockListItem *StockLastStatus::getPStock() const
{
    return pStock;
}

void StockLastStatus::setPStock(const StockListItem *newPStock)
{
    pStock = newPStock;
}

double StockLastStatus::getPrice() const
{
    return price;
}

void StockLastStatus::setPrice(double newPrice)
{
    price = newPrice;
}

const QString& StockLastStatus::getChangedPer() const
{
    return changedPer;
}

void StockLastStatus::setChangedPer(const QString& newChangedPer)
{
    changedPer = newChangedPer;
}

const QString& StockLastStatus::getChangedValue() const
{
    return changedValue;
}

void StockLastStatus::setChangedValue(const QString& newChangedValue)
{
    changedValue = newChangedValue;
}

double StockLastStatus::getVolume() const
{
    return volume;
}

void StockLastStatus::setVolume(double newVolume)
{
    volume = newVolume;
}

double StockLastStatus::getFund() const
{
    return fund;
}

void StockLastStatus::setFund(double newFund)
{
    fund = newFund;
}

const QString& StockLastStatus::getChange() const
{
    return change;
}

void StockLastStatus::setChange(const QString& newChange)
{
    change = newChange;
}

const QString& StockLastStatus::getVolRate() const
{
    return volRate;
}

void StockLastStatus::setVolRate(const QString& newVolRate)
{
    volRate = newVolRate;
}

double StockLastStatus::getHigh() const
{
    return high;
}

void StockLastStatus::setHigh(double newHigh)
{
    high = newHigh;
}

double StockLastStatus::getLow() const
{
    return low;
}

void StockLastStatus::setLow(double newLow)
{
    low = newLow;
}

double StockLastStatus::getPriceRate() const
{
    return priceRate;
}

void StockLastStatus::setPriceRate(double newPriceRate)
{
    priceRate = newPriceRate;
}

double StockLastStatus::getPreClose() const
{
    return preClose;
}

void StockLastStatus::setPreClose(double newPreClose)
{
    preClose = newPreClose;
}

double StockLastStatus::getOpen() const
{
    return open;
}

void StockLastStatus::setOpen(double newOpen)
{
    open = newOpen;
}

QString StockLastStatus::getCurVol() const
{
    return curVol;
}

void StockLastStatus::setCurVol(const QString &newCurVol)
{
    curVol = newCurVol;
}

void StockLastStatus::toString(QString& out)const
{
    out=QString(tr("price:%1,chg:%2,high:%3,low:%4,amplitude:%5,rate:%6"))
              .arg(price).arg(changedValue).arg(high).arg(low)
              .arg(high-low,0,'f',2)
              .arg(volRate);
}

bool find_status_by_stock_func(StockLastStatus* p,const StockListItem* stock)
{
    return p->getPStock()==stock;
}
QList<StockLastStatus*>::const_pointer findLastStatus(const QList<StockLastStatus*>& status,const StockLastStatus* stock)
{
    return std::find(status.constBegin(),status.constEnd(),stock);
}

QList<StockLastStatus*>::const_iterator findLastStatus(const QList<StockLastStatus*>& sastStatus,const StockListItem* stock)
{
    return std::find_if(sastStatus.constBegin(),sastStatus.constEnd(),std::bind(find_status_by_stock_func,_1,stock));
}

void clearLastStatus(QList<StockLastStatus*>& lastStatus)
{
    while(!lastStatus.isEmpty())
    {
        delete lastStatus.back();
        lastStatus.pop_back();
    }
}
