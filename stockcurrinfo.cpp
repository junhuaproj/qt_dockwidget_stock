#include "stockcurrinfo.h"

StockCurrInfo::StockCurrInfo() {}

QString StockCurrInfo::getCode() const
{
    return code;
}

void StockCurrInfo::setCode(const QString &newCode)
{
    code = newCode;
}

QString StockCurrInfo::getName() const
{
    return name;
}

void StockCurrInfo::setName(const QString &newName)
{
    name = newName;
}

float StockCurrInfo::getPrice() const
{
    return price;
}

void StockCurrInfo::setPrice(float newPrice)
{
    price = newPrice;
}

const QString& StockCurrInfo::getChangedPercent() const
{
    return changedPercent;
}

void StockCurrInfo::setChangedPercent(const QString& newChangedPercent)
{
    changedPercent = newChangedPercent;
}

const QString& StockCurrInfo::getChangedValue() const
{
    return changedValue;
}

void StockCurrInfo::setChangedValue(const QString& newChangedValue)
{
    changedValue = newChangedValue;
}

int StockCurrInfo::getVolume() const
{
    return volume;
}

void StockCurrInfo::setVolume(int newVolume)
{
    volume = newVolume;
}

int StockCurrInfo::getCurVol() const
{
    return curVol;
}

void StockCurrInfo::setCurVol(int newCurVol)
{
    curVol = newCurVol;
}

int StockCurrInfo::getFund() const
{
    return fund;
}

void StockCurrInfo::setFund(int newFund)
{
    fund = newFund;
}

const QString& StockCurrInfo::getChange() const
{
    return change;
}

void StockCurrInfo::setChange(const QString& newChange)
{
    change = newChange;
}

const QString& StockCurrInfo::getVolRate() const
{
    return volRate;
}

void StockCurrInfo::setVolRate(const QString& newVolRate)
{
    volRate = newVolRate;
}

float StockCurrInfo::getHigh() const
{
    return high;
}

void StockCurrInfo::setHigh(float newHigh)
{
    high = newHigh;
}

float StockCurrInfo::getLow() const
{
    return low;
}

void StockCurrInfo::setLow(float newLow)
{
    low = newLow;
}

float StockCurrInfo::getPriceRate() const
{
    return priceRate;
}

void StockCurrInfo::setPriceRate(float newPriceRate)
{
    priceRate = newPriceRate;
}

float StockCurrInfo::getPreClose() const
{
    return preClose;
}

void StockCurrInfo::setPreClose(float newPreClose)
{
    preClose = newPreClose;
}

float StockCurrInfo::getOpen() const
{
    return open;
}

void StockCurrInfo::setOpen(float newOpen)
{
    open = newOpen;
}
int StockCurrInfo::getPriceStatus()const
{
    return (int)((price*100)-(preClose*100));
}
