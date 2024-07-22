/*
 * 获取指数
*/
#ifndef MYSTOCKINDEX_H
#define MYSTOCKINDEX_H

#include "mydata.h"

class StockListItem;
class MyStockIndex : public MyData
{
public:
    MyStockIndex(MyDB* db);
    virtual ~MyStockIndex();

    bool gets(QList<StockListItem*>& items);
};

#endif // MYSTOCKINDEX_H
