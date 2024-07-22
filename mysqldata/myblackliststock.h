/*
 * 增加到黑名单MYSQL操作类
*/
#ifndef MYBLACKLISTSTOCK_H
#define MYBLACKLISTSTOCK_H

#include "mydata.h"

class MyBlackListStock : public MyData
{
public:
    MyBlackListStock(MyDB* db);
    virtual ~MyBlackListStock();
    bool add(int stockid,const QString& code,const QString& market,const QDate& outDate);
};

#endif // MYBLACKLISTSTOCK_H
