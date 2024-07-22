#include "stockchartview.h"
#include <QPainter>
#include <limits.h>
#include <float.h>
#include <QFontMetrics>
#include <qdebug.h>
#include <QBrush>
#include <QMimeData>
#include <QTableWidgetItem>
#include "appconfig.h"
const char* mimetableitemlist="application/x-qabstractitemmodeldatalist";

StockChartView::StockChartView(QWidget *parent)
    : QWidget(parent),chartLeft(45),chartRight(30),
    chartBottom(15),chartTop(50),priceMin(INT_MAX),priceMax(0),
    valueCount(241),volumeMin(INT_MAX),volumeMax(0),curIndex(-1),
    curMouseY(-1),
    bTitleVisible(true),showCurrStatus(true)
{
}

StockChartView::~StockChartView()
{
    clearPrices();
    clearXChartLabels();
}


void StockChartView::setCurrStatus(bool bShow)
{
    if(bShow)
        chartTop=50;
    else
        chartTop=22;
}

void StockChartView::setTitleVisible(bool bVisible)
{
    if(bTitleVisible==bVisible)return ;
    bTitleVisible=bVisible;
    if(bTitleVisible)
    {
        chartTop=50;
    }
    else
    {
        chartTop=30;
    }
}
void StockChartView::setTitle(const QString& title)
{
    this->title=title;
}

void StockChartView::setStockItem(StockListItem* pStockItem)
{
    stockItem.set(pStockItem);
}

const StockListItem* StockChartView::getStockItem()const
{
    return &stockItem;
}
bool StockChartView::hasStock()const
{
    return stockItem.getCode().length()!=0;
}
void StockChartView::clearPrices()
{
    while(!timePrices.isEmpty()){
        delete timePrices.back();
        timePrices.pop_back();
    }
}
void StockChartView::clearXChartLabels()
{
    while(!xChartLabels.isEmpty())
    {
        delete xChartLabels.back();
        xChartLabels.pop_back();
    }
}

void StockChartView::addXChartLabel(int value,const QString& label)
{
    StockChartLabel* xLabel=new StockChartLabel();
    xLabel->setValue(value);
    xLabel->setLabel(label);
    xChartLabels.push_back(xLabel);
}


void StockChartView::setPrices(float preClosePrice,float open,float high,float low)
{
    this->preClosePrice = preClosePrice;
    this->openPrice=open;
    lowPrice=low;
    highPrice= high;
}

int32_t StockChartView::getValueCount()const
{
    return valueCount;
}
void StockChartView::setValueCount(int32_t count)
{
    valueCount=count;
}
void StockChartView::addStockPrice(StockPriceItem* item)
{
    timePrices.append(item);
}

void StockChartView::addStockPrice(QString hhmm,float price,float avgPrice,int32_t volume,int32_t fund)
{
    StockPriceItem* p=new StockPriceItem();
    p->setAvgPrice(avgPrice);
    p->setHhmm(hhmm);
    p->setPrice(price);
    p->setFund(fund);
    p->setVolume(volume);
    addStockPrice(p);
}
#include <QVector>
#include <QPoint>

void StockChartView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //painter.setRenderHint(QPainter::TextAntialiasing);
    painter.fillRect(rect(),QColor(0x33,0x33,0x33));
    painter.save();
    QRect rcClient=rect();
    QRectF chartRect(chartLeft,chartTop,
                    rcClient.width()-chartLeft-chartRight,
                    rcClient.height()-chartTop-chartBottom);
    //painter.drawRect(chartRect);

    int32_t count=valueCount;
    if(count<timePrices.size())
        count=timePrices.size();

    qreal itemWidth=chartRect.width()/count;
    QRectF priceRect=chartRect;
    priceRect.setHeight(chartRect.height()*2/3);
    paintPrice(&painter,priceRect,itemWidth);
    QRectF volumeRect=chartRect;
    volumeRect.moveTop(priceRect.bottom());
    volumeRect.setHeight(chartRect.height()/3);
    this->paintVolume(&painter,volumeRect,itemWidth);

    //X轴标签
    QFontMetrics fontMetrics=painter.fontMetrics();
    for(QList<StockChartLabel*>::const_iterator it=xChartLabels.constBegin();
         it!=xChartLabels.constEnd();it++)
    {
        qreal textHeight=fontMetrics.height();
        qreal textWidth=fontMetrics.horizontalAdvance((*it)->getLabel());
        painter.drawText(QPoint(chartRect.left()+itemWidth+itemWidth*(*it)->getValue()-textWidth/2,
                                 chartRect.bottom()+textHeight),(*it)->getLabel());
    }
    //画竖线，
    painter.setPen(QPen(QColor(0xff,0xff,0xff),0.2,Qt::PenStyle::SolidLine));
    for(QList<StockChartLabel*>::const_iterator it=xChartLabels.constBegin();
         it!=xChartLabels.constEnd();it++)
    {
        painter.drawLine(chartRect.left()+itemWidth+itemWidth*(*it)->getValue(),chartRect.top(),
                          chartRect.left()+itemWidth+itemWidth*(*it)->getValue(),chartRect.bottom());
    }

    //painter.setPen(QPen(Qt::white,0.3));
    painter.setPen(QPen(QColor(0xff,0xff,0xff,0xaf),0.5));
    if(bTitleVisible)
        painter.drawText(chartLeft,0,rcClient.width()-chartLeft-chartRight,chartTop/2,Qt::AlignCenter,title);
    if(curIndex>=0&&curIndex<timePrices.size())
    {
        qreal x=chartRect.left()+itemWidth+curIndex*itemWidth;
        painter.drawLine(x,chartRect.top(),x,chartRect.bottom());

        painter.drawLine(rect().left(),curMouseY,rect().right(),curMouseY);

        if(showCurrStatus)
        {
            StockPriceItem* priceItem=timePrices.at(curIndex);
            QRectF rc(chartRect.left(),chartTop/2,chartRect.width(),chartTop);
            if(bTitleVisible){
                rc.setY(chartTop/2);
                rc.setHeight(chartTop/2);
            }
            //QString subtitle=QString("%1,price:%2,avg price:%3,vol:%4").arg(priceItem->getHhmm())
            //                       .arg(priceItem->getPrice(),0,'f',2)
            //                       .arg(priceItem->getAvgPrice(),0,'f',2)
            //                       .arg(priceItem->getVolume());
            QString subtitle;
            getCurPriceItemStatus(priceItem,subtitle);
            painter.drawText(rc,subtitle);
        }
        /*qreal itemWidth=chartRect.width()/4;
        QRectF rc(chartRect.left(),(chartTop-fontMetrics.height())/2,itemWidth,fontMetrics.height());
        painter.drawText(rc,priceItem->getHhmm());
        rc.moveLeft(rc.right());
        painter.drawText(rc,QString("price:%1").arg(priceItem->getPrice(),0,'f',2));
        rc.moveLeft(rc.right());
        painter.drawText(rc,QString("avg price:%1").arg(priceItem->getAvgPrice(),0,'f',2));
        rc.moveLeft(rc.right());
        painter.drawText(rc,QString("vol:%1").arg(priceItem->getVolume()));*/
    }

    painter.restore();
    /*float curPriceMin;
    float curPriceMax;

    getPriceRange(curPriceMin,curPriceMax);
    curPriceMin=(int)((curPriceMin*100-5))/100.0;
    curPriceMax=(int)((curPriceMax*100+5))/100.0;
    if(curPriceMin>priceMin)
        curPriceMin=priceMin;
    if(curPriceMax<priceMax)
        curPriceMax=priceMax;

    float priceDiff=curPriceMax-curPriceMin;

    int32_t x=chartRect.left();
    //QVector<QPoint> lines;
    QPoint* points=new QPoint[timePrices.size()];
    ;
    //pen.setColor();
    painter.setPen(QPen(QColor(0x9f,0x9f,0x9f),0.4,Qt::PenStyle::SolidLine));
    painter.drawRect(chartRect);
    painter.setPen(QPen(QColor(0,0,0),1,Qt::PenStyle::SolidLine));
    int index=0;
    for(QList<StockPriceItem*>::const_iterator it=timePrices.constBegin();
         it!=timePrices.constEnd();it++,index++)
    {
        int32_t y=chartRect.bottom()-(((*it)->getPrice()-curPriceMin)*chartRect.height()/priceDiff);
        //lines.push_back(QPoint(x,y));
        points[index].setX(x);
        points[index].setY(y);
        x+=itemWidth;

    }
    painter.drawPolyline(points,timePrices.size());

    delete []points;
    QFontMetrics fontMetrics=painter.fontMetrics();

    for(QList<StockChartLabel*>::const_iterator it=xChartLabels.constBegin();
         it!=xChartLabels.constEnd();it++)
    {
        int textHeight=fontMetrics.height();
        int textWidth=fontMetrics.horizontalAdvance((*it)->getLabel());
        painter.drawText(QPoint(chartRect.left()+itemWidth*(*it)->getValue()-textWidth/2,
                                chartRect.bottom()+textHeight),(*it)->getLabel());
    }
    painter.setPen(QPen(QColor(0xbf,0xbf,0xbf),0.2,Qt::PenStyle::SolidLine));
    for(QList<StockChartLabel*>::const_iterator it=xChartLabels.constBegin();
         it!=xChartLabels.constEnd();it++)
    {
        painter.drawLine(chartRect.left()+itemWidth*(*it)->getValue(),chartRect.top(),
                         chartRect.left()+itemWidth*(*it)->getValue(),chartRect.bottom());
    }

    painter.restore();*/
}

void StockChartView::getCurPriceItemStatus(const StockPriceItem *pPrice,QString& out)
{
    out=QString("%1,price:%2,avg price:%3,vol:%4").arg(pPrice->getHhmm())
                           .arg(pPrice->getPrice(),0,'f',2)
                           .arg(pPrice->getAvgPrice(),0,'f',2)
                           .arg(pPrice->getVolume());
}
void StockChartView::paintVolume(QPainter* painter,const QRectF& chartRect,qreal itemWidth)
{
    int32_t curVolumeMin;
    int32_t curVolumeMax;

    getVolumeRange(curVolumeMin,curVolumeMax);

    if(curVolumeMin>volumeMin)
        curVolumeMin=volumeMin;
    if(curVolumeMax<volumeMax)
        curVolumeMax=volumeMax;

    int32_t volumeDiff=curVolumeMax-curVolumeMin;

    qreal x=chartRect.left()+itemWidth;
    int index=0;

    painter->setPen(QPen(Qt::white,1,Qt::PenStyle::SolidLine));
    for(QList<StockPriceItem*>::const_iterator it=timePrices.constBegin();
         it!=timePrices.constEnd();it++,index++)
    {
        qreal y=chartRect.bottom()-(((*it)->getVolume()-curVolumeMin)*chartRect.height()/volumeDiff);
        //lines.push_back(QPoint(x,y));
        painter->drawLine(x,chartRect.bottom(),x,y);
        x+=itemWidth;
    }

    QFontMetrics fontMetrics=painter->fontMetrics();
    if(curMouseY>chartRect.top()&&curMouseY<chartRect.bottom())
    {
        qreal curDiff=chartRect.bottom()-curMouseY;
        float curValue=volumeDiff*curDiff/chartRect.height()+curVolumeMin;
        QRectF rcTip(0,curMouseY-fontMetrics.height()/2,chartLeft,fontMetrics.height());
        painter->setPen(QPen(Qt::white));
        //painter->fillRect(rcTip,QColor(171,214,248));
        painter->fillRect(rcTip,QColor(0x66,0x99,0x66));
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curValue,0,'f',0));
    }
}

void StockChartView::paintPrice(QPainter* painter,const QRectF& chartRect,qreal itemWidth)
{
    float curPriceMin;
    float curPriceMax;

    getPriceRange(curPriceMin,curPriceMax);
    curPriceMin=(int)((curPriceMin*100-5))/100.0;
    curPriceMax=(int)((curPriceMax*100+5))/100.0;
    if(curPriceMin>priceMin)
        curPriceMin=priceMin;
    if(curPriceMax<priceMax)
        curPriceMax=priceMax;

    float priceDiff=curPriceMax-curPriceMin;

    qreal x=chartRect.left()+itemWidth;
    //QVector<QPoint> lines;
    QPointF* points=new QPointF[timePrices.size()];

    //pen.setColor();
    painter->setPen(QPen(QColor(0x9f,0x9f,0x9f),0.4,Qt::PenStyle::SolidLine));
    painter->drawRect(chartRect);
    painter->setPen(QPen(Qt::white,1,Qt::PenStyle::SolidLine));
    int index=0;
    for(QList<StockPriceItem*>::const_iterator it=timePrices.constBegin();
         it!=timePrices.constEnd();it++,index++)
    {
        qreal y=chartRect.bottom()-(((*it)->getPrice()-curPriceMin)*chartRect.height()/priceDiff);
        //lines.push_back(QPoint(x,y));
        points[index].setX(x);
        points[index].setY(y);
        x+=itemWidth;

    }
    painter->drawPolyline(points,timePrices.size());

    delete []points;
    QFontMetrics fontMetrics=painter->fontMetrics();

    qreal itemHeight=chartRect.height()/8;
    painter->setPen(QPen(QColor(0xbf,0xbf,0xbf),0.2,Qt::PenStyle::SolidLine));
    for(int i=1;i<8;i++)
    {
        qreal y=chartRect.bottom()-i*itemHeight;
        painter->drawLine(chartRect.left(),y,chartRect.right(),y);
    }

    int8_t clr=-1;
    painter->setPen(QPen(Qt::white,0.2,Qt::PenStyle::SolidLine));
    for(int i=1;i<8;i++)
    {
        qreal y=chartRect.bottom()-i*itemHeight;
        float curValue=priceDiff*((i*itemHeight))/chartRect.height()+curPriceMin;
        if(curValue>preClosePrice&&clr<=0)
            painter->setPen(QPen(QColor(0xff,0x0,0x00),0.2,Qt::PenStyle::SolidLine));
        QRectF rcTip(0,y-fontMetrics.height()/2,chartLeft,fontMetrics.height());
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curValue,0,'f',2));
        rcTip.moveLeft(chartRect.right());
        //painter->drawText(rcTip,Qt::AlignLeft,QString("%1%").arg((100*(curValue-preClosePrice)/preClosePrice),0,'f',2));
    }
    //昨天收盘价
    if(preClosePrice>curPriceMin&&preClosePrice<curPriceMax)
    {
        painter->setPen(QPen(Qt::white,0.2,Qt::PenStyle::SolidLine));
        qreal y=chartRect.bottom()-((preClosePrice-curPriceMin)*chartRect.height()/priceDiff);
        painter->drawLine(chartRect.left(),y,chartRect.right(),y);
        QRectF rcTip(0,y-fontMetrics.height()/2,chartLeft,fontMetrics.height());
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(preClosePrice,0,'f',2));
    }

    if(curMouseY>chartRect.top()&&curMouseY<chartRect.bottom())
    {
        qreal curDiff=chartRect.bottom()-curMouseY;
        float curValue=priceDiff*(curDiff)/chartRect.height()+curPriceMin;
        QRectF rcTip(0,curMouseY-fontMetrics.height()/2,chartLeft,fontMetrics.height());
        painter->setPen(QPen(Qt::white));
        painter->fillRect(rcTip,Qt::black);
        painter->drawText(rcTip,Qt::AlignRight,QString("%1").arg(curValue,0,'f',2));
        rcTip.moveLeft(chartRect.right());
        painter->fillRect(rcTip,Qt::black);
        painter->drawText(rcTip,Qt::AlignLeft,QString("%1%").arg((100*(curValue-preClosePrice)/preClosePrice),0,'f',2));
    }
}

void StockChartView::mouseMoveEvent(QMouseEvent* event)
{
    QRectF chartRect(chartLeft,chartTop,
                     rect().width()-chartLeft-chartRight,
                     rect().height()-chartTop-chartBottom);
    //painter.drawRect(chartRect);
    if(chartRect.contains(event->pos()))
    {
        int32_t count=valueCount;
        if(count<timePrices.size())
            count=timePrices.size();

        qreal itemWidth=chartRect.width()/count;
        curIndex=(event->pos().x()-chartRect.left()-itemWidth)/itemWidth;
        //qDebug()<<"index:"<<index;
        curMouseY=event->pos().y();
        if(curIndex<timePrices.size()&&curIndex>-1)
            emit onCurrTime(timePrices.at(curIndex));
        this->repaint();
    }
    else
    {
        if(curIndex==-1&&curMouseY==-1)
        {}
        else
        {
            curIndex=-1;
            curMouseY=-1;
            repaint();
        }
    }
}

void StockChartView::getPriceRange(float& min,float& max)const
{
    min=FLT_MAX,max=0;
    for(QList<StockPriceItem*>::const_iterator it=timePrices.constBegin();
         it!=timePrices.constEnd();
         it++)
    {
        if((*it)->getPrice()>max)
            max=(*it)->getPrice();
        if((*it)->getPrice()<min)
            min=(*it)->getPrice();
    }
}
void StockChartView::getVolumeRange(int32_t& min,int32_t& max)const
{
    min=INT_MAX;
    max=0;

    for(QList<StockPriceItem*>::const_iterator it=timePrices.constBegin();
         it!=timePrices.constEnd();
         it++)
    {
        if((*it)->getVolume()>max)
            max=(*it)->getVolume();
        if((*it)->getVolume()<min)
            min=(*it)->getVolume();
    }
}

void StockChartView::mousePressEvent(QMouseEvent* event)
{
    Qt::MouseButton btn=event->button();
    if(btn!=Qt::MouseButton::LeftButton)
        return;
    QRectF chartRect(chartLeft,chartTop,
                     rect().width()-chartLeft-chartRight,
                     rect().height()-chartTop-chartBottom);
    //painter.drawRect(chartRect);
    if(chartRect.contains(event->pos()))
    {
        int32_t count=valueCount;
        if(count<timePrices.size())
            count=timePrices.size();

        qreal itemWidth=chartRect.width()/count;
        curIndex=(event->pos().x()-chartRect.left()-itemWidth)/itemWidth;
        if(curIndex<timePrices.size())
        {
            StockPriceItem* p=timePrices.at(curIndex);
            emit onCurrTime(p);
        }
        else
        {
            curIndex=-1;
        }
    }
}
void StockChartView::addOneDayXLabel(StockChartView* p,int day)
{
    const int DAY_COUNT=242;
    if(day==0)
        p->addXChartLabel(0+DAY_COUNT*day,"09:30");
    p->addXChartLabel(30+DAY_COUNT*day,"10:00");
    p->addXChartLabel(60+DAY_COUNT*day,"10:30");
    p->addXChartLabel(90+DAY_COUNT*day,"11:00");
    p->addXChartLabel(120+DAY_COUNT*day,"11:30");
    //axisX->append("13:00",121);
    p->addXChartLabel(151+DAY_COUNT*day,"13:30");
    p->addXChartLabel(181+DAY_COUNT*day,"14:00");
    p->addXChartLabel(211+DAY_COUNT*day,"14:30");
    p->addXChartLabel(241+DAY_COUNT*day,"15:00");
}
void StockChartView::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* pData= event->mimeData();
    if(pData->hasFormat(mimetableitemlist))
    {
        event->acceptProposedAction();
    }
    /*QStringList formats=pData->formats();
    QStringList::iterator it=formats.begin(),end=formats.end();
    qDebug()<<"formats";
    for(;it!=end;it++)
    {
        qDebug()<<(*it);
    }
    QList<QTableWidgetItem> itemList=pData->
    */
}
void StockChartView::dropEvent(QDropEvent *event)
{
    const QMimeData* pData= event->mimeData();
    if(pData->hasFormat(mimetableitemlist))
    {
        QByteArray bytes=pData->data(mimetableitemlist);
        QDataStream stream(&bytes,QIODevice::ReadOnly);
        int row,col;
        QMap<int,QVariant> dataMap;
        while(!stream.atEnd())
        {
            stream>>row>>col>>dataMap;
            if(col==0)
            {
                StockListItem* p=(StockListItem*)qvariant_cast<uint64_t>(dataMap[TABLE_DATA_STOCK]);
                clearPrices();
                setStockItem(p);
                repaint();
            }
            qDebug()<<"row"<<row<<"col"<<col<<"map"<<dataMap;
        }
        event->acceptProposedAction();
    }

}
