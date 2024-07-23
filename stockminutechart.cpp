#include "stockminutechart.h"
#include "stocklistitem.h"
#include <QMimeData>
#include <QDragEnterEvent>
#include "appconfig.h"

namespace minutechart
{
#define MINUTE_VIEW_COUNT   2
const char* mimetablestsock="application/x-qabstractitemmodeldatalist";
StockMinuteChart::StockMinuteChart(QWidget *parent)
    : ChartWidget(MINUTE_VIEW_COUNT,parent),pStock(NULL)
    ,priceMin(FLT_MAX),priceMax(0)
    ,volumeMin(FLT_MAX),volumeMax(0)

    ,preClose(0),
    showCurrStatus(0),pricePercent(80),bTitleVisible(true)
{
    chartCfg.itemSep=0;
    chartCfg.marginLeft=50;
    chartCfg.marginTop=30;
    chartCfg.marginRight=40;
    chartCfg.marginBottom=30;
    resetChartPos(&curPos);
}

StockMinuteChart::~StockMinuteChart()
{
    reset();
}
void StockMinuteChart::reset()
{
    StockDayMinute* pDay;
    prices.clear();
    while(!days.isEmpty())
    {
        pDay=days.back();
        if(pDay->isCreate(this))
            delete pDay;
        days.pop_back();
    }
}

void StockMinuteChart::addDay(StockDayMinute* day)
{
    days.push_back(day);
    QList<StockPriceItem*>::const_iterator it=day->minute.constBegin(),
        end=day->minute.constEnd();
    for(;it!=end;it++)
        prices.append(*it);
}

void StockMinuteChart::addDays(QList<StockDayMinute*>& days)
{
    QList<StockDayMinute*>::iterator it=days.begin(),end=days.end();
    for(;it!=end;it++)
    {
        addDay(*it);
    }
}
void StockMinuteChart::reCalcRate()
{
    priceRate=rcViews[0].height()/(priceMax-priceMin);
    volumeRate=rcViews[1].height()/(volumeMax-volumeMin);
    chartCfg.itemWidth=rcViews[0].width()/prices.size();
}
void StockMinuteChart::reCalcRange()
{
    reCalcPriceRange(priceMin,priceMax);
    reCalcVolumeRange(volumeMin,volumeMax);
    reCalcRate();
    resetChartPos(&startPos);
    resetChartPos(&stopPos);

    totalVolume=0;
    totalFund=0;
    QList<StockDayMinute*>::iterator it=days.begin(),end=days.end();
    for(;it!=end;it++)
    {
        (*it)->reCalc();
        totalVolume+=(*it)->getTotalVolume();
        totalFund+=(*it)->getTotalFund();
    }
}

void StockMinuteChart::reCalcPriceRange(float& min,float& max)const
{
    float curMin,curMax;
    min=FLT_MAX;
    max=0;
    QList<StockDayMinute*>::const_pointer it=days.constBegin(),end=days.constEnd();
    for(;it!=end;it++)
    {
        (*it)->getPriceRange(curMin,curMax);
        min=fmin(min,curMin);
        max=fmax(max,curMax);
    }
}
void StockMinuteChart::reCalcVolumeRange(int32_t& min,int32_t& max)const
{
    int32_t curMin,curMax;
    min=INT_MAX;
    max=0;
    QList<StockDayMinute*>::const_pointer it=days.constBegin(),end=days.constEnd();
    for(;it!=end;it++)
    {
        (*it)->getVolumeRange(curMin,curMax);
        min=std::min(min,curMin);
        max=std::max(max,curMax);
    }

}
bool StockMinuteChart::isDataValid()const
{
    return days.size()!=0;
}
bool StockMinuteChart::isIndexValid(int index)const
{
    return index>=0&&index<prices.size();
}

float StockMinuteChart::getYValue(int view,float yPos)
{
    QRectF rc=rcViews[view];
    double rate;
    double zero_v;
    if(view==0)
    {
        rate = priceRate;
        zero_v=priceMin;
    }
    else
    {
        rate=volumeRate;
        zero_v=volumeMin;
    }
    double zero_y=rc.bottom()-(rate*(zero_v-priceMin));
    return zero_v+(zero_y-yPos)/rate;
}


void StockMinuteChart::paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc)
{
    //if(bTitleVisible)
    painter->setPen(chartCfg.fore);
    painter->drawText(rcChart.left(),rc.top(),rcChart.width(),
                      rcChart.top()-rc.top(),Qt::AlignCenter,title);
}

void StockMinuteChart::paintCharts(QPainter* painter,const QRectF& rcChart)
{
    painter->save();

    paintPrice(painter);

    paintVolume(painter);
#if 1
    //X轴标签


    paintVReferLine(painter,rcChart);
    //painter.setPen(QPen(Qt::white,0.3));
    painter->setPen(QPen(QColor(0xff,0xff,0xff,0xaf),0.5));

    if(curPos.xIndex>=0&&curPos.xIndex<prices.size())
    {
        qreal x=rcChart.left()+chartCfg.itemWidth+curPos.xIndex*chartCfg.itemWidth;
        painter->drawLine(x,rcChart.top(),x,rcChart.bottom());

        painter->drawLine(rect().left(),curPos.y,rect().right(),curPos.y);

        if(showCurrStatus)
        {
            //const StockPriceItem* priceItem=prices.at(curPos.xIndex);
            QRectF rc(rcChart.left(),chartCfg.marginTop/2,rcChart.width(),chartCfg.marginTop);
            if(bTitleVisible){
                rc.setY(chartCfg.marginTop/2);
                rc.setHeight(chartCfg.marginTop/2);
            }

            QString subtitle;
            getItemDescribe(&curPos,subtitle);
            painter->drawText(rc,subtitle);
        }
    }
#endif
    painter->restore();
}

void StockMinuteChart::paintVReferLine(QPainter* painter,const QRectF& chartRect)
{
    painter->setPen(QPen(QColor(0xff,0xff,0xff),0.2,Qt::PenStyle::SolidLine));

    qreal xleft=(chartRect.left()+chartCfg.itemWidth/2);

    QList<StockDayMinute*>::const_iterator it=days.constBegin(),end=days.constEnd();
    QRectF rcText;
    rcText.setTop(chartRect.bottom());
    rcText.setHeight(chartCfg.marginBottom);
    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    for(;it!=end;it++)
    {
        int center=(*it)->count()/2;
        rcText.setLeft(xleft);
        xleft+=chartCfg.itemWidth*center;
        painter->drawLine(xleft,chartRect.top(),
                         xleft,chartRect.bottom());

        xleft+=((*it)->count()-center)*chartCfg.itemWidth;
        painter->drawLine(xleft,chartRect.top(),
                         xleft,chartRect.bottom());
        rcText.setRight(xleft);
        painter->drawText(rcText,(*it)->date.toString(appConfig.getDateFormat()),option);
    }

}
StockDayMinute* StockMinuteChart::lastDay()
{
    if(!days.isEmpty())
        return days.last();
    return NULL;
}

void StockMinuteChart::resizeEvent(QResizeEvent *event)
{
    QRectF rc;//=rect();

    getChartRect(rc);

    int itemHeight = rc.height();
    rcViews[0]=rc;
    rcViews[0].setHeight(itemHeight*pricePercent/100);
    rcViews[1]=rc;
    rcViews[1].setTop(rcViews[0].bottom());
    rcViews[1].setHeight(itemHeight-rcViews[0].height());

    if (!isDataValid())return ;
    chartCfg.itemWidth=rc.width()/prices.count();
    //大小改变后重新计算比例
    reCalcRate();
}

void StockMinuteChart::paintVolume(QPainter* painter)
{

    //int32_t volumeDiff=volumeMax-volumeMin;

    QPointF start;
    start.setX(rcViews[1].left()+chartCfg.itemWidth/2);
    start.setY(rcViews[1].bottom());
    //float rate=chartRect.height()/volumeDiff;

    painter->setPen(QPen(Qt::white,1,Qt::PenStyle::SolidLine));

    QList<StockDayMinute*>::const_iterator it= days.constBegin(),end=days.constEnd();

    for(;it!=end;it++)
    {
        (*it)->paintVolume(painter,chartCfg.itemWidth,1,volumeRate,start,volumeMin);
        start.setX(start.x()+(*it)->count()*chartCfg.itemWidth);
    }

#if 1
    QFontMetrics fontMetrics=painter->fontMetrics();
    if(curPos.viewIdx==1)// curPos.y>chartRect.top()&&curPos.y<chartRect.bottom())
    {
        //qreal curDiff=chartRect.bottom()-curPos.y;
        //float curValue=volumeDiff*curDiff/chartRect.height()+volumeMin;
        QRectF rcTip(0,curPos.y-fontMetrics.height()/2,chartCfg.marginLeft,fontMetrics.height());
        painter->setPen(QPen(Qt::white));
        //painter->fillRect(rcTip,QColor(171,214,248));
        painter->fillRect(rcTip,QColor(0x66,0x99,0x66));
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curPos.yValue,0,'f',0));
    }
#endif
}

void StockMinuteChart::paintPrice(QPainter* painter)
{
    QPointF start;
    start.setX(rcViews[0].left()+chartCfg.itemWidth/2);
    start.setY(rcViews[0].bottom());

    QPointF* points=new QPointF[prices.size()],*pts;

    //pen.setColor();
    painter->setPen(QPen(QColor(0x9f,0x9f,0x9f),0.4,Qt::PenStyle::SolidLine));
    //painter->drawRect(chartRect);
    painter->setPen(QPen(Qt::white,1,Qt::PenStyle::SolidLine));

    QList<StockDayMinute*>::const_iterator it= days.constBegin(),end=days.constEnd();

    pts=points;
    int ptcount=0;
    for(;it!=end;it++)
    {
        int curCount=(*it)->getPricePoints(pts,chartCfg.itemWidth,1,priceRate,start,priceMin);
        pts+=curCount;
        ptcount+=curCount;
        //=(*it)->count();
        start.setX(start.x()+chartCfg.itemWidth*curCount);
    }

    painter->drawPolyline(points,ptcount);

    delete []points;
#if 1
    QFontMetrics fontMetrics=painter->fontMetrics();

    qreal itemHeight=rcViews[0].height()/4;
    painter->setPen(QPen(QColor(0xbf,0xbf,0xbf),0.2,Qt::PenStyle::SolidLine));
    for(int i=1;i<4;i++)
    {
        qreal y=rcViews[0].bottom()-i*itemHeight;
        painter->drawLine(rcViews[1].left(),y,rcViews[0].right(),y);
    }

    int8_t clr=-1;
    //昨天收盘价
    float basePrice=preClose;
    if(basePrice==0&&!days.isEmpty())
    {
        basePrice=(*days.cbegin())->getPreClose();
    }
    painter->setPen(QPen(Qt::white,0.2,Qt::PenStyle::SolidLine));
    for(int i=1;i<4;i++)
    {
        qreal y=rcViews[0].bottom()-i*itemHeight;
        float curValue=(priceMax-priceMin)*((i*itemHeight))/rcViews[0].height()+priceMin;
        if(curValue>basePrice&&clr<=0)
            painter->setPen(QPen(QColor(0xff,0x0,0x00),0.2,Qt::PenStyle::SolidLine));
        QRectF rcTip(0,y-fontMetrics.height()/2,chartCfg.marginLeft,fontMetrics.height());
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curValue,0,'f',2));
        rcTip.moveLeft(rcViews[0].right());
        painter->drawText(rcTip,Qt::AlignLeft,QString("%1%").arg((100*(curValue-basePrice)/basePrice),0,'f',2));
    }

    if(curPos.viewIdx==0&&curPos.xIndex!=-1)//curPos.y>chartRect.top()&&curPos.y<chartRect.bottom())
    {
        //qreal curDiff=chartRect.bottom()-curPos.y;
        //float curValue=priceDiff*(curDiff)/chartRect.height()+priceMin;
        QRectF rcTip(0,curPos.y-fontMetrics.height()/2,chartCfg.marginLeft,fontMetrics.height());
        painter->setPen(QPen(Qt::white));
        painter->fillRect(rcTip,Qt::black);
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curPos.yValue,0,'f',2));
        if(basePrice!=0)
        {
            rcTip.moveLeft(rcViews[0].right());
            painter->fillRect(rcTip,Qt::black);
            painter->drawText(rcTip,Qt::AlignLeft,QString("%1%").arg((100*(curPos.yValue-basePrice)/basePrice),0,'f',2));
        }
    }
#endif
}

bool StockMinuteChart::getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const
{
    int startIdx,endIdx;
    if(!getValidIndex(start,end,startIdx,endIdx))
        return false;
    double vol=0,fund=0;
    for(;startIdx<=endIdx;startIdx++)
    {
        vol+=prices.at(startIdx)->getVolume();
        fund+=prices.at(startIdx)->getFund();
    }
    out=QString(tr("vol:%1;fund:%2,avg price=%3,%4%"))
              .arg(vol,0,'f',3)
              .arg(fund,0,'f',0)
              .arg(fund/vol/100,0,'f',3)
              .arg(fund*100/totalFund,0,'f',2);
    return true;
}

bool StockMinuteChart::getItemDescribeEx(const ChartPos* start,QString& out)const
{
    double downFund=0,downVolume=0;
    double upFund=0,upVolume=0;
    QList<const StockPriceItem*>::const_iterator it= prices.constBegin(),end=prices.constEnd();
    for(;it!=end;it++)
    {
        if((*it)->getPrice()==0)continue;
        if((*it)->getPrice()>start->yValue)
        {
            upFund+=(*it)->getFund();
            upVolume+=(*it)->getVolume();
        }
        else
        {
            downFund+=(*it)->getFund();
            downVolume+=(*it)->getVolume();
        }
    }
    out=QString(tr("up:v:%1,f:%2,%5%|down:v:%3,f:%4,%6%")).arg(upFund,0,'f',0).arg(upVolume,0,'f',0)
              .arg(downFund,0,'f',0).arg(downVolume,0,'f',0)
              .arg(upFund*100/totalFund,0,'f',0).arg(downFund*100/totalFund,0,'f',0);
    return true;

}

bool StockMinuteChart::getItemDescribe(const ChartPos *pos,QString& out)const
{
    const StockPriceItem* price=prices.at(pos->xIndex);
    out=QString("%1,price:%2,avg price:%3,vol:%4").arg(price->getHhmm())
              .arg(price->getPrice(),0,'f',2)
              .arg(price->getAvgPrice(),0,'f',2)
              .arg(price->getVolume());
    return true;
}

void StockMinuteChart::setCurrStatus(bool bShow)
{
    showCurrStatus=bShow;
    if(showCurrStatus)
        chartCfg.marginTop=50;
    else
        chartCfg.marginTop=22;
}
minutechart::StockDayMinute* StockMinuteChart::newStockDayMinute(int fillCount)
{
    return new minutechart::StockDayMinute(this,fillCount);
}
void StockMinuteChart::setTitleVisible(bool bVisible)
{
    if(bTitleVisible==bVisible)return ;
    bTitleVisible=bVisible;
    if(bTitleVisible)
    {
        chartCfg.marginTop=50;
    }
    else
    {
        chartCfg.marginTop=30;
    }
}

void StockMinuteChart::setPrices(float preClosePrice,float open,float high,float low)
{
    preClose=preClosePrice;
}
void StockMinuteChart::setStockItem(const StockListItem* pStockItem)
{
    pStock=pStockItem;
    if(pStock)
    {
        QString title;
        pStock->toString(title);
        setTitle(title);
    }
    this->reset();
}
bool StockMinuteChart::StockMinuteChart::isStock()const
{
    return pStock!=NULL;
}
const StockListItem* StockMinuteChart::getStockItem()const
{
    return pStock;
}

void StockMinuteChart::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* pData= event->mimeData();
    if(pData->hasFormat(mimetablestsock))
    {
        event->acceptProposedAction();
    }
}
void StockMinuteChart::dropEvent(QDropEvent *event)
{
    const QMimeData* pData= event->mimeData();
    if(pData->hasFormat(mimetablestsock))
    {
        QByteArray bytes=pData->data(mimetablestsock);
        QDataStream stream(&bytes,QIODevice::ReadOnly);
        int row,col;
        QMap<int,QVariant> dataMap;
        while(!stream.atEnd())
        {
            stream>>row>>col>>dataMap;
            if(col==0)
            {
                StockListItem* p=(StockListItem*)qvariant_cast<uint64_t>(dataMap[TABLE_DATA_STOCK]);
                reset();
                setStockItem(p);
                repaint();
            }
            qDebug()<<"row"<<row<<"col"<<col<<"map"<<dataMap;
        }
        event->acceptProposedAction();
    }

}

}
