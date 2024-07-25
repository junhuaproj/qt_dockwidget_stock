#include "stockbasedata.h"
#include "ta_func.h"
namespace chart {
StockBaseData::StockBaseData(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title),maCfgLen(0),
    maCfg(NULL),ma(NULL),maLen(0),maColor(NULL),lineWidth(0.9)
//,stock(NULL)
{
    //int maCfgLen;
    //int* maCfg;
    //double** ma;
}

StockBaseData::~StockBaseData()
{
    deInit();
    freeCfg();
}
void StockBaseData::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
    maLen=maCfgLen;
    int pointerLen=sizeof(double*);
    void* p=malloc(pointerLen*maLen);
    ma=(double**)p;
    for(int i=0;i<maLen;i++)
    {
        ma[i]=(double*)malloc(sizeof(double)*stock->count);
        memset(ma[i],0,sizeof(double)*stock->count);
    }
}

void StockBaseData::freeCfg()
{
    if(maCfg)
    {
        free(maCfg);
        maCfg=NULL;
        maCfgLen=0;
        free(maColor);
        maColor=NULL;
    }
}
void StockBaseData::freeMa()
{
    if(maLen)
    {
        for(int i=0;i<maLen;i++)
        {
            free(ma[i]);
        }
        free(ma);
        maLen=0;
        ma=NULL;
    }
}
void StockBaseData::deInit()
{
    freeMa();

}


void StockBaseData::reCalcIndex()
{
    //计算移动均线
    int begIdx=0,nbeElemet=0;
    for(int i=0;i<maCfgLen;i++)
    {
        TA_MA(0,stock->count,stock->closes,maCfg[i],TA_MAType_SMA,&begIdx,&nbeElemet,ma[i]);
        int moveCount=std::min<int>(nbeElemet,stock->count-begIdx);
        memmove(ma[i]+begIdx,ma[i],sizeof(StockFloat)*moveCount);
        memset(ma[i],0,begIdx*sizeof(double));
    }
}

void StockBaseData::reCalcRange(int start,int count)
{
    range.high=FLT_MIN;
    range.low=FLT_MAX;;
    int32_t end=start+count;
    for(;start<end&&start<stock->count;start++)
    {
        range.high=fmax(stock->highs[start],range.high);
        range.low=fmin(stock->lows[start],range.low);
    }

}
const StockFloatRange* StockBaseData::getRange()
{
    return &range;
}

void StockBaseData::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("%5,open:%1,close:%2,high:%3,low:%4")
            .arg(stock->opens[index],0,'f',2)
            .arg(stock->closes[index],0,'f',2)
            .arg(stock->highs[index],0,'f',2)
                    .arg(stock->lows[index],0,'f',2).arg(stock->dates[index].date);
    strings.push_back(new ColorString(t,0xffffff));
    for(int i=0;i<maCfgLen;i++)
    {
        t=QString("MA%1:%2").arg(maCfg[i]).arg(ma[i][index]);
        strings.push_back(new ColorString(t,maColor[i]));
    }
}
bool StockBaseData::pillarColor(int index,PChartPillar pillar)
{
    return false;
}
bool StockBaseData::isPillarSysColor(int index)
{
    return false;
}

uint8_t StockBaseData::paddingBottom()const
{
    return 15;
}

ChartDraw* StockBaseData::getChartDraw()
{
    return this;
}

double StockBaseData::getZeroY()
{
    return range.low;
}
/**
 * 间隔线
 * K线
 * 移动均线
*/
int StockBaseData::chartCount()
{
    return 2+maLen;
}

DataChartType StockBaseData::chartType(int index)
{
    if(index<2)
    {
        return Chart_User;
    }
    else
    {
        return Chart_Line;
    }

}
bool StockBaseData::lineValue(int index,PChartLine pLine)//折线图
{
    int maIndex=index-2;
    pLine->width=lineWidth;
    pLine->color=maColor[maIndex];
    pLine->v=ma[maIndex]+pLine->start;
    return true;
}
void StockBaseData::drawKLineItem(int dataIndex,QRectF& rc,const PaintParam* p)
{
    float open =p->getValueHeight(stock->opens[dataIndex]);
    float close = p->getValueHeight(stock->closes[dataIndex]);
    float high = p->getValueHeight(stock->highs[dataIndex]);
    float low = p->getValueHeight(stock->lows[dataIndex]);

    QPen pen;
    pen.setWidth(1);
    if(p->isIncrease(dataIndex))
    {
        QPoint pt1, pt2;
        pen.setColor(p->clrUp);
        p->p->setPen(pen);

        rc.setY(p->zero_y - close);
        rc.setHeight(close-open-1);//画矩形时高度会加1
        //pDC->Rectangle(&rcItem);
        p->p->drawRect(rc);

        pt1.setX((rc.right() + rc.x()) / 2);
        pt2.setX(pt1.x());
        if (high > close)
        {
            pt1.setY(rc.y());
            pt2.setY(p->zero_y - high);
            p->p->drawLine(pt1, pt2);
        }
        if (low < open)
        {
            pt1.setY(rc.bottom());
            pt2.setY(p->zero_y - low);
            p->p->drawLine(pt1, pt2);
        }
    }
    else
    {
        pen.setColor(p->clrDown);
        p->p->setPen(pen);
        //p->p->setPen(p->clrDown);
        rc.setY(p->zero_y - open);
        rc.setHeight(open- close);

        if(open==close)
        {
            p->p->drawLine(rc.x(),p->zero_y-open,rc.right(),p->zero_y-open);
        }
        else
            p->p->fillRect(rc,p->clrDown);


        if (high > open || low < close)
        {
            QPoint pt1, pt2;
            pt1.setX((rc.right() + rc.x()) / 2);
            pt2.setX(pt1.x());

            pt1.setY(p->zero_y - low);
            pt2.setY (p->zero_y - high);
            p->p->drawLine(pt1, pt2);
        }
    }
}
void StockBaseData::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{
    if(chartIndex==1)
    {
        drawKLineItem(dataIndex,rc,p);
    }
    else if(chartIndex==0)
    {
        if((dataIndex-p->start)%20==0)
        {
            p->p->setPen(p->pDrawParam->grid);
            p->p->drawLine(rc.center().rx(),p->rc.top(),rc.center().rx(),p->rc.bottom());
            QRectF rcText(rc);
            rcText.setY(p->rc.bottom());
            rcText.setHeight(paddingBottom());
            QFontMetrics fm(p->p->font());
            p->p->setPen(p->pChartCfg->fore);
            rcText.setWidth(fm.horizontalAdvance(stock->dates[dataIndex].date));
            p->p->drawText(rcText,stock->dates[dataIndex].date);
        }
    }
}
StatisticIndexCfg* StockBaseData::getCfg()
{
    return this;
}
bool StockBaseData::loadCfg(QJsonObject* obj)
{
    //freeCfg();
    return false;
}
bool StockBaseData::saveCfg(QJsonObject* obj)
{
    return false;
}
const char* StockBaseData::cfgName()const
{
    return StatisticIndex::getTitle();
}
const char* StockBaseData::getIndexId()const
{
    return StatisticIndex::getIndexId();
}
bool StockBaseData::loadDefault()
{
    freeCfg();
    maCfgLen=4;
    maCfg=(int*)malloc(sizeof(int)*maCfgLen);
    maCfg[0]=5;
    maCfg[1]=10;
    maCfg[2]=20;
    maCfg[3]=30;
    maColor=(uint32_t*)malloc(sizeof(uint32_t)*maCfgLen);
    maColor[0]=0xff00ff;
    maColor[1]=0x007fff;
    maColor[2]=0xff7f00;
    maColor[3]=0x7f1f7f;
    return true;
}
int StockBaseData::getCfgCount()
{
    return 0;
}
bool StockBaseData::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    return false;
}
bool StockBaseData::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    return false;
}
}
