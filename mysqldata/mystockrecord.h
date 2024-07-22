/**
 * 记录的操作
 */
#ifndef MYSTOCKRECORD_H
#define MYSTOCKRECORD_H

#include "mydata.h"


namespace data{
class StockRecordItem;
class DataLoader;
}
class MyStockRecord : public MyData
{
public:
    MyStockRecord(MyDB* db);
    virtual ~MyStockRecord();

    // virtual void initBind();
    void setDataLoader(data::DataLoader* pDataLoader);
    bool bindValue(const data::StockRecordItem* record);
    bool gets(QList<data::StockRecordItem*>& items);
    bool update(const data::StockRecordItem* item);
    bool add(data::StockRecordItem*);
    void del(const data::StockRecordItem*);
    bool adds(QList<data::StockRecordItem*>& items);
    bool gets(const char* sql,QList<data::StockRecordItem*>& items);
    bool getsByParent(int parent,QList<data::StockRecordItem*>& items);
    bool gets(QList<data::StockRecordItem*>& items,const QDateTime* begin=NULL,const QDateTime* end=NULL);

    bool getsByWhere(const QString& where,QList<data::StockRecordItem*>& items);
protected:
    data::DataLoader* pDataLoader;
};

#endif // MYSTOCKRECORD_H
