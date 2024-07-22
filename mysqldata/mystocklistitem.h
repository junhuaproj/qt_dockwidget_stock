/**
 * 获取股票清单和备注
*/
#ifndef MYSTOCKLISTITEM_H
#define MYSTOCKLISTITEM_H

#include "mydata.h"
namespace data
{
class StockNote;
}
class StockListItem;

class MyStockListItem:public MyData
{
public:
    MyStockListItem(MyDB* db);
    virtual ~MyStockListItem();

    //virtual void initBind();

    // bool addNote(const data::StockNote* item);
    // bool isExist(const data::StockNote* item);
    // bool isExist(int stockid);
    bool updateNote(int id,const QString& note,int level);
    void get(int id,StockListItem& item);
    void gets(QList<StockListItem*>& items);

    int getFlags(const char* market);
// protected:
//     int stockid;
//     int level;
//     MYSQL_TIME lastDateTime;
//     char* szNote;
//     char* szName;
//     char* szCode;
//     char* szMarket;
};



#endif // MYSTOCKLISTITEM_H
