/*
 * 从网络加载SOHU数据
*/
#ifndef DATALOADERNETSOHU_H
#define DATALOADERNETSOHU_H

#include "netrequest.h"
#include <QJsonArray>
#include "sohudataparser.h"

//class StockChartView;
class StockListItem;
class StockLastStatus;

namespace datasohu{
class DataLoaderNetMinute:public NetRequest,public DataParser
{
    Q_OBJECT
public:
    DataLoaderNetMinute(INetResponse* response,QObject *parent = nullptr);
    virtual ~DataLoaderNetMinute();

    bool requestStockLastStatus(const QList<StockListItem*>& stocks);
    bool requestStock(const StockListItem* pStock);


    bool getRequestValid() const;
    void setRequestValid(bool newRequestValid);

protected:
    bool requestValid;
};
}
#endif // DATALOADERNETSOHU_H
