#include "dataloaderlocalsohu.h"
#include "appconfig.h"
#include "stocklistitem.h"
#include <QFile>
#include "stockminutechart.h"
namespace data{
extern const char* data_root;
}
namespace datasohu{
DataLoaderLocal::DataLoaderLocal() {}


bool DataLoaderLocal::loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const StockListItem* pItem,void* pCreate)
{
    QDate startDate=start;
    bool bOK=false;
    for(;startDate<=end;)
    {
        minutechart::StockDayMinute* pDay=NULL;//new minutechart::StockDayMinute();

        if(loadCode(&pDay,startDate,pItem,pCreate))
        {
            pDay->date=startDate;
            days->push_back(pDay);
            bOK|=true;
        }
        //else
        //    delete pDay;

        startDate=startDate.addDays(1);
    }
    return bOK;
}

bool DataLoaderLocal::loadCode(minutechart::StockDayMinute** day,const QDate& date,const StockListItem* pItem,void* pCreate)
{
    QString dir,prefix;
    if(pItem->getFlag()&StockItemFlag_ZS)
    {
        dir="\\index";
        prefix="zs";
    }
    else
    {
        dir="";
        prefix="cn";
    }
    QString minutePath=QString("%1%4\\sohuday\\%2\\%5_%3.json")
                             .arg(data::data_root,
                                  date.toString("yyyyMMdd"),
                                  pItem->getCode(),
                                  dir,prefix);
    return loadFile(day,minutePath,pCreate);
}

bool DataLoaderLocal::loadFile(minutechart::StockDayMinute** day,const QString& path,void* pCreate)
{
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        QByteArray data=f.readAll();
        f.close();
        *day=new minutechart::StockDayMinute(pCreate);
        if(!this->parseMinuteData(data,*day,true))
        {
            delete *day;
            *day=NULL;
        }
        return true;

    }
    return false;
}
}
