/*
 * 记录列表
*/
#ifndef MYSTOCKRECORDLIST_H
#define MYSTOCKRECORDLIST_H

#include "mydata.h"
namespace data{

class StockRecordList;
}
class MyStockRecordList : public MyData
{
public:
    MyStockRecordList(MyDB* db);
    virtual ~MyStockRecordList();
    // virtual void initBind();

    bool bindValue(const data::StockRecordList* record);
    void del(const data::StockRecordList* record);
    bool add(data::StockRecordList* record);
    bool update(const data::StockRecordList* record);
    bool getLists(const QDateTime& begin,const QDateTime& end,QList<data::StockRecordList*>& list,int level=-1);
    bool getLists(const char* sql,QList<data::StockRecordList*>& list);
    bool getListsByWhere(const QString& where,QList<data::StockRecordList*>& list);
protected:
    int id;
    MYSQL_TIME crdate;
    int level;
    char* szTitle;
    char* szContent;
};

#endif // MYSTOCKRECORDLIST_H
