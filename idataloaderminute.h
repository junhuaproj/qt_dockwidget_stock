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
