/*
 * 日线数据
*/
#ifndef STOCKDAYDATA_H
#define STOCKDAYDATA_H

#define STOCK_DATE_LEN    12
#define StockFloat  double
#define StockInt    int


enum StockDoubleType{
    Stock_Open,
    Stock_Close,
    Stock_High,
    Stock_Low,

    Stock_Chg,
    Stock_Pchg,
    Stock_Vaturnovers,

    Stock_Turnovers,
    Stock_Dbl,
};

typedef struct StockDate
{
    char date[STOCK_DATE_LEN];
}*PStockDate;


struct StockFloatRange
{
    StockFloat low,high;
};

struct StockDayData{
    int count;
    StockDate* dates;
    StockFloat* closes;
    StockFloat* opens;
    StockFloat* highs;
    StockFloat* lows;
    StockFloat* vol;
};

void calcFloatRange(const StockFloat* values,StockFloatRange* range,int start,int32_t count);
#endif // STOCKDAYDATA_H
