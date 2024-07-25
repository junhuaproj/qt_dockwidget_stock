/*
 * 通达信数据加载
*/
#ifndef __TDXMINUTEDATA_H__
#define __TDXMINUTEDATA_H__

#include <QDate>
//#include <stockchartview.h>
#include <vector>
#include "stocklistitem.h"
//#include "stockdayitems.h"
#include "dayitem.h"
#include "idataloaderminute.h"

//using namespace  std;
namespace tdx {
class TDXMinuteData:public IDataLoaderMinute
{
public:
    TDXMinuteData();

    virtual bool loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const StockListItem* pItem,void* pCreate);
    bool loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const QString& code,const char* dir,void* pCreate);

    int loadDayData(const StockListItem* pItem, std::vector<PStockDayItem>& stockDays);
    int loadDayData(const QString& code,const char* dir, std::vector<PStockDayItem>& stockDays);
};
}
#endif // TDXMINUTEDATA_H
