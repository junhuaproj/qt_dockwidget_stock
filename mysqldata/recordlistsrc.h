#ifndef RECORDLISTSRC_H
#define RECORDLISTSRC_H
#include <QString>
#include <QList>
#include "data/stockprop.h"
/*class StockProp
{
public:
    StockProp(){}
    virtual ~StockProp(){}
    int id;
    QString name;
};
*/
class StockListItem;

class RecordListSrc
{
public:
    RecordListSrc();
    virtual ~RecordListSrc();
    QString code;
    const StockListItem* pStock;
    float downPercent;
    int day;
    QString indexes;
    QList<const StockProp*> business;
    QList<const StockProp*> notions;
};

#endif // RECORDLISTSRC_H
