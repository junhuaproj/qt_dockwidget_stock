/*
 * 加载分时线数据接口，以支持多种来源的数据，例如通达信，其他网络抓取的数据
*/
#ifndef IDATALOADERMINUTE_H
#define IDATALOADERMINUTE_H

#include <QList>

namespace minutechart
{
class StockDayMinute;
}

struct IDataLoaderMinute{
    virtual bool loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const StockListItem* pItem,void* pCreate)=0;
};

#endif // IDATALOADERMINUTE_H
