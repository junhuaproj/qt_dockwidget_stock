#ifndef SOHUDATAPARSER_H
#define SOHUDATAPARSER_H

#include <QJsonArray>

class StockListItem;
//class StockChartView;
class StockLastStatus;

namespace minutechart
{
class StockDayMinute;
}
namespace data {
class DataLoader;
}

typedef StockLastStatus* (*Get_Last_Status_by_Stock_Callbback)(const StockListItem* stock,void*);

namespace datasohu{


class DataParser
{
public:
    DataParser();

    void setDataLoader(data::DataLoader* dataLoader);
    bool parseMinuteData(const QByteArray& data,minutechart::StockDayMinute* day,bool bInited=false);
    bool parsePriceData(const QByteArray& data,QList<StockLastStatus*>& stocks,Get_Last_Status_by_Stock_Callbback callback=NULL,void* p=NULL);
protected:
    bool parsePriceJsonData(const QJsonObject& obj,QList<StockLastStatus*>& stocks,Get_Last_Status_by_Stock_Callbback callback,void* p);
    bool parseMinuteJsonData(const QJsonArray& data,minutechart::StockDayMinute* day);

protected:
    data::DataLoader* dataLoader;
};

}
#endif // SOHUDATAPARSER_H
