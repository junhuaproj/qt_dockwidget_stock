#include "stockdayminute.h"
#include <QPointF>

#include <QPainter>

namespace minutechart
{
StockDayMinute::StockDayMinute(void* pCreater,int fillCount):preClose(0)
    ,pCreate(pCreater)
{
    if(fillCount)
    {
        fillEmpty(fillCount);
        bFilled=true;
        date=QDate::currentDate();
    }
    else
        bFilled=false;
}
StockDayMinute::~StockDayMinute()
{
    reset();
}
void StockDayMinute::reCalc()
{
    totalFund=0;
    totalVolume=0;
    QList<StockPriceItem*>::const_iterator it= minute.constBegin(),end=minute.constEnd();
    for(;it!=end;it++)
    {
        if((*it)->getPrice()==0)continue;
        totalFund+=(*it)->getFund();
        totalVolume+=(*it)->getVolume();
    }
}
float StockDayMinute::getPreClose()const
{
    return preClose;
}
double StockDayMinute::getTotalVolume() const
{
    return totalVolume;
}

double StockDayMinute::getTotalFund() const
{
    return totalFund;
}
void StockDayMinute::setPriceItem(StockPriceItem* p,const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice)
{
    p->setHhmm(hhmm);
    p->setPrice(price);
    p->setVolume(volume);
    p->setFund(fund);
    p->setAvgPrice(avgPrice);
}

bool StockDayMinute::setMin(int index,const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice)
{
    if(index>=0&&index<minute.size())
    {
        setPriceItem(minute.at(index),hhmm,price,volume,fund,avgPrice);
        return true;
    }
    return false;
}

void StockDayMinute::addMin(const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice)
{
    StockPriceItem* p=new StockPriceItem();
    setPriceItem(p,hhmm,price,volume,fund,avgPrice);
    minute.push_back(p);
}
void StockDayMinute::addMin(uint16_t hour,uint16_t minute,float price,int32_t volume,int32_t fund,float avgPrice)
{
    char hhmm[8];
    sprintf(hhmm,"%02d:%02d",hour,minute);
    //QString strhour=hhmm;
    addMin(hhmm,//QString("%1:%2").arg(hour,2,10,'0').arg(minute,2,10,'0'),
           price,volume,fund,avgPrice);
}
void StockDayMinute::reset()
{
    while(!minute.isEmpty())
    {
        delete minute.back();
        minute.pop_back();
    }
}
int StockDayMinute::getPricePoints(QPointF* pts,qreal itemWidth,float lineWidth,float rate,const QPointF& start,float startValue)const
{
    qreal x=start.x()+itemWidth/2;
    QList<StockPriceItem*>::const_iterator it= minute.constBegin(),end=minute.constEnd();
    int count=0;
    for(;it!=end;it++)
    {
        if((*it)->getPrice()==0)break;
        pts->setX(x);
        pts->setY(start.y()-(((*it)->getPrice()-startValue)*rate));
        x+=itemWidth;
        pts++;
        count++;
    }
    return count;
}

void StockDayMinute::getPriceRange(float& min,float& max)const
{
    min=FLT_MAX;
    max=0;
    for(QList<StockPriceItem*>::const_iterator it=minute.constBegin();
         it!=minute.constEnd();
         it++)
    {
        if((*it)->getPrice()==0)continue;
        if((*it)->getPrice()>max)
            max=(*it)->getPrice();
        if((*it)->getPrice()<min)
            min=(*it)->getPrice();
    }
}
void StockDayMinute::getVolumeRange(int32_t& min,int32_t& max)const
{
    min=INT_MAX;
    max=0;

    for(QList<StockPriceItem*>::const_iterator it=minute.constBegin();
         it!=minute.constEnd();
         it++)
    {
        if((*it)->getPrice()==0)continue;
        if((*it)->getVolume()>max)
            max=(*it)->getVolume();
        if((*it)->getVolume()<min)
            min=(*it)->getVolume();
    }
}

void StockDayMinute::fillEmpty(int count)
{
    for(int i=0;i<count;i++)
    {
        StockPriceItem* item=new StockPriceItem();
        minute.push_back(item);
    }
}


int StockDayMinute::paintVolume(QPainter* painter,qreal itemWidth,float lineWidth,float rate,
                                const QPointF& start,int32_t startValue)const
{
    QPointF zero=start,valuePos;
    zero.setX(zero.x()+itemWidth/2);
    QList<StockPriceItem*>::const_iterator it= minute.begin(),end=minute.end();
    int count=0;
    for(;it!=end;it++)
    {
        if((*it)->getPrice()==0)break;
        valuePos.setX(zero.x());
        valuePos.setY(start.y()-(((*it)->getVolume()-startValue)*rate));
        painter->drawLine(zero,valuePos);
        zero.setX(zero.x()+itemWidth);
        count++;
    }
    return count;
}

}
