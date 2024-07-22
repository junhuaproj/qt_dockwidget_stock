/*
 * 获取股票属性，所属指数，概念，行业
*/
#ifndef MYSTOCKPROP_H
#define MYSTOCKPROP_H

#include "mydata.h"
#include "data/stockprop.h"
class MyStockProp : public MyData
{
public:
    MyStockProp(MyDB* db);
    virtual ~MyStockProp();
    bool getProp(int stockid,QList<StockProp*>& indexes,QList<StockProp*>& notions,QList<StockProp*>& bussiness);

    bool getIndexes(int stockid,QList<StockProp*>& indexes);
    bool getNotions(int stockid,QList<StockProp*>& notions);
    bool getBussiness(int stockid,QList<StockProp*>& bussiness);
    bool getProp(const char* sql,QList<StockProp*>& indexes);
protected:
    BindValue* id;
    BindValue* name;
};

#endif // MYSTOCKPROP_H
