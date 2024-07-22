#include "dataloadernetsohu.h"

#include "stocklistitem.h"
#include <QList>

namespace datasohu{
DataLoaderNetMinute::DataLoaderNetMinute(INetResponse* response,QObject *parent)
    :NetRequest(response,parent)
{

}

DataLoaderNetMinute::~DataLoaderNetMinute()
{

}

bool DataLoaderNetMinute::requestStockLastStatus(const QList<StockListItem*>& stocks)
{
    QString codes="";
    QList<StockListItem*>::const_iterator it=stocks.constBegin(),end=stocks.constEnd();
    for(;it!=end;it++)
    {
        codes.append("cn_"+(*it)->getCode()+",");
    }
    QDateTime curDate=QDateTime::currentDateTime();
    codes=codes.left(codes.length()-1);
    setRequestValid(true);
    QString url=QString("https://hqm.stock.sohu.com/getqjson?code=%1&cb=fortune_hq_diwei&_=%2").arg(codes).arg(curDate.toMSecsSinceEpoch());
    return NetRequest::requestUrl(url);
}

bool DataLoaderNetMinute::requestStock(const StockListItem* pStock)
{
    QDateTime curDate=QDateTime::currentDateTime();
    QString codeHead=pStock->getCode().mid(3);
    QString url=QString("https://hq.stock.sohu.com/cn/%1/cn_%2-4.html?openinwebview_finance=false&t=%3&_=%3")
                      .arg(codeHead,pStock->getCode())
                      .arg(curDate.toMSecsSinceEpoch());
    return NetRequest::requestUrl(url,(uint64_t)pStock);
}

bool DataLoaderNetMinute::getRequestValid() const
{
    return requestValid;
}

void DataLoaderNetMinute::setRequestValid(bool newRequestValid)
{
    requestValid = newRequestValid;
}

}
