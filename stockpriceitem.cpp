#include "stockpriceitem.h"

StockPriceItem::StockPriceItem():price(0) {

}

const QString& StockPriceItem::getHhmm() const
{
    return hhmm;
}

void StockPriceItem::setHhmm(const QString &newHhmm)
{
    hhmm = newHhmm;
}

float StockPriceItem::getPrice() const
{
    return price;
}

void StockPriceItem::setPrice(float newPrice)
{
    price = newPrice;
}
bool StockPriceItem::isHour()const
{
    return hhmm.contains("00");
}

bool StockPriceItem::is0900Or1300()const
{
    return hhmm.contains("13:00")||hhmm.contains("09:00");
}
float StockPriceItem::getAvgPrice() const
{
    return avgPrice;
}

void StockPriceItem::setAvgPrice(float newAvgPrice)
{
    avgPrice = newAvgPrice;
}

int32_t StockPriceItem::getVolume() const
{
    return volume;
}

void StockPriceItem::setVolume(int32_t newVolume)
{
    volume = newVolume;
}

int32_t StockPriceItem::getFund() const
{
    return fund;
}

void StockPriceItem::setFund(int32_t newFund)
{
    fund = newFund;
}
