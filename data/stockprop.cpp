#include "stockprop.h"

StockProp::StockProp()
    :id(0)
{}

StockProp::StockProp(int nid,const QString& nname)
    :id(nid),name(nname)
{

}
StockProp::~StockProp()
{

}

void clearPropList(QList<StockProp*>& props)
{
    while(!props.isEmpty())
    {
        delete props.back();
        props.pop_back();
    }
}
bool find_propBy_id_func(StockProp* p,int id)
{
    return p->id==id;
}

bool find_propByC_id_func(const StockProp* p,int id)
{
    return p->id==id;
}
void propListToString(QList<const StockProp*>& props,QString& out)
{
    QList<const StockProp*>::iterator it=props.begin(),end=props.end();
    for(;it!=end;it++)
    {
        out.append((*it)->name);
    }
}
void propListToString(const QList<StockProp*>& props,QString& out)
{
    QList<StockProp*>::const_iterator it=props.constBegin(),end=props.constEnd();
    for(;it!=end;it++)
    {
        out.append((*it)->name);
        out.append(",");
    }
}
