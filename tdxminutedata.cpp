#include "tdxminutedata.h"
#include <QFile>
#include "stockminutechart.h"
#include "appconfig.h"
namespace tdx {
//数据结构,总计长度32字节
#define ITEM_LEN    32
typedef struct MinuteItem
{
    uint16_t date;//日期
    uint16_t time;//时间
    float price[4];//价格open,high,low,close
    float amount;//成交金额
    uint32_t volume;//成交量
}*PMinuteItem;

typedef struct DayItem
{
    uint32_t date;
    uint32_t open;
    uint32_t high;
    uint32_t low;
    uint32_t close;
    float fund;//成交额
    int32_t volume;
    uint32_t reserve;
}*PDayItem;

//const char* tdx_doc_root="E:\\zd_pazq_hy\\vipdoc";
TDXMinuteData::TDXMinuteData() {}

bool TDXMinuteData::loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const StockListItem* pItem,void* pCreate)
{
    if(pItem->isShanghai()||pItem->isShanghaiZS())
        return loadCode(days,start,end,pItem->getCode(),"sh",pCreate);
    else if(pItem->isShenzhen()||pItem->isShenzhenZS())
        return loadCode(days,start,end,pItem->getCode(),"sz",pCreate);
    return false;
}

bool TDXMinuteData::loadCode(QList<minutechart::StockDayMinute*>* days,const QDate& start,const QDate& end,const QString& code,const char* dir,void* pCreate)
{
    //const char* root = "E:\\zd_pazq_hy\\vipdoc";

    QString path=QString("%1\\%2\\minline\\%3%4.lc1").arg(appConfig.getTdxPath(),dir,dir, code);
    QFile f(path);
    if(!f.open(QFile::ReadOnly))
    {
        return false;
    }
    bool ret=false;
    MinuteItem src;
    uint16_t year;
    uint8_t month,day;
    uint8_t hour,min;
    uint16_t startDate=start.day();
    startDate+=start.month()*100;
    startDate+=(start.year()-2004)*2048;

    uint16_t endDate=end.day();
    endDate+=end.month()*100;
    endDate+=(end.year()-2004)*2048;
    //qDebug()<<"find:"<<startDate;
    int nday=0;
    uint16_t prevDate=0;
    minutechart::StockDayMinute* dayMinute=NULL;
    double amount=0;
    while(true)
    {
        if(f.read((char*)&src,ITEM_LEN)!=ITEM_LEN)break;
        //qDebug()<<"date:"<<src.date;
        if(endDate<src.date)break;//如果终止日期小于保存的开始日期，数据不存在
        if(startDate>src.date)//如果开始日期大于当前存储日期，跳过整天数据；
        {
            //hour = (src.time) / 60;
            //min = src.time % 60;
            nday++;
            f.seek(ITEM_LEN*240*nday);
            continue;
        }
        if(endDate<src.date)//如果终止日期比当前日期大，本次数据读完了
            break;
        year = (src.date / 2048) + 2004;
        month = (src.date % 2048) / 100;
        day = (src.date) % 2048 % 100;

        hour = (src.time) / 60;
        min = src.time % 60;

        if(prevDate==0||prevDate!=src.date)
        {
            dayMinute=new minutechart::StockDayMinute(pCreate);
            dayMinute->date.setDate(year,month,day);
            days->push_back(dayMinute);
            prevDate=src.date;
        }
        dayMinute->addMin(hour,min,
                          src.price[3],src.volume/100,src.amount,
                          src.volume==0?0:src.amount/(src.volume));
        amount+=src.amount;

        ret=true;
    }
    qDebug()<<"amount:"<<amount;
    f.close();
    return ret;
}
int TDXMinuteData::loadDayData(const StockListItem* pItem, std::vector<PStockDayItem>& stockDays)
{
    if(pItem->isShanghai())
        return loadDayData(pItem->getCode(),"sh",stockDays);
    else if(pItem->isShenzhen())
        return loadDayData(pItem->getCode(),"sz",stockDays);
    return 0;

}

int TDXMinuteData::loadDayData(const QString& code,const char* dir, std::vector<PStockDayItem>& stockDays)
{
    QString path=QString("%1\\%2\\lday\\%3%4.day").arg(tdx_doc_root,dir,dir, code);
    QFile f(path);
    if(!f.open(QFile::ReadOnly))
    {
        return 0;
    }
    DayItem src;
    uint16_t year;
    uint8_t month,day;
    //uint8_t hour,min;

    PStockDayItem pDayItem;
    while(true)
    {
        if(f.read((char*)&src,ITEM_LEN)!=ITEM_LEN)break;

        year = (src.date / 10000);
        month = (src.date % 10000) / 100;
        day = (src.date%10000) %100;


        //QString dateTime=QString("%1-%2-%3").arg(year,4,10).arg(month,2,10).arg(day,2,10);
        pDayItem=(PStockDayItem)malloc(sizeof(StockDayItem));
        memset(pDayItem,0,sizeof(StockDayItem));
        sprintf(pDayItem->date,"%04d-%02d-%02d",year,month,day);
        pDayItem->close=(float)src.close/100;
        pDayItem->open=(float)src.open/100;
        pDayItem->high=(float)src.high/100;
        pDayItem->low=(float)src.low/100;
        pDayItem->voturnover=src.volume;
        pDayItem->vaturnover=src.fund;
        stockDays.push_back(pDayItem);


    }
    f.close();
    return stockDays.size();
}

}
