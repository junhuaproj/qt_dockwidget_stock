#ifndef IDATALOADER_H
#define IDATALOADER_H

#include <stockchartview.h>
#include "stockcurrinfo.h"

struct TodayStockMinuteUpdate
{
    virtual void MinuteUpdated(StockChartView* chartView)=0;
};

struct TodayStocksPriceUpdate
{
    virtual void PriceUpdated(const StockCurrInfo* stockInfo)=0;
    virtual void PriceUpdateFinished()=0;
};

#endif // IDATALOADER_H
