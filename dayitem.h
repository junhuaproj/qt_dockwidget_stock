#ifndef  __DAYITEM_H__
#define __DAYITEM_H__

#define MA_LEN  4
struct StockDayItem
{
    char date[12];
    float open, close, chg, pchg, low, high;
    int voturnover;
    double vaturnover, turnover;
    float ema1;
    float ema2;
    float diff;
    float dea;
    float macd;

    float kdj_k;
    float kdj_d;
    float kdj_j;

    //float obv;

    float ma[MA_LEN];
};

struct StockDayMacd
{
    float ema1;
    float ema2;
    float diff;
    float dea;
    float macd;
};

typedef struct StockDayMacd* PStockDayMacd;
typedef struct StockDayItem *PStockDayItem;


#endif // ! __DAYITEM_H__
