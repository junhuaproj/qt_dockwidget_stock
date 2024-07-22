/**
 * 股票属性
 * 是某指数成分股
 * 概念
 * 行业等
*/

#ifndef STOCKPROP_H
#define STOCKPROP_H
#include <QString>
#include <QList>
class StockProp
{
public:
    StockProp();
    StockProp(int id,const QString& name);
    virtual ~StockProp();
    int id;
    QString name;
};
bool find_propBy_id_func(StockProp* p,int id);
bool find_propByC_id_func(const StockProp* p,int id);
void clearPropList(QList<StockProp*>& props);

void propListToString(QList<const StockProp*>& props,QString& out);
void propListToString(const QList<StockProp*>& props,QString& out);
#endif // STOCKPROP_H
