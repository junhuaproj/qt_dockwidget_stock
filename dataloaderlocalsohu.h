/*
 * 从本地加载SOHU数据
*/

#ifndef DATALOADERLOCALSOHU_H
#define DATALOADERLOCALSOHU_H

#include <QDate>
#include "sohudataparser.h"
#include "idataloaderminute.h"
//class StockChartView;
class StockListItem;

namespace datasohu{
class DataLoaderLocal:public DataParser,public IDataLoaderMinute
{
public:
    DataLoaderLocal();
    virtual bool loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const StockListItem* pItem,void* pCreate);
    bool loadCode(minutechart::StockDayMinute** day,const QDate& date,const StockListItem* pItem,void* pCreate);

protected:
    bool loadFile(minutechart::StockDayMinute** day,const QString& path,void* pCreate);
};
}
#endif // DATALOADERLOCALSOHU_H
