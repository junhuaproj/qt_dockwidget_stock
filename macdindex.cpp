#include "macdindex.h"

#include "Algorithm.h"
namespace chart {
MacdIndex::MacdIndex(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title)
{
    cfg=new StockMacdCfg(this);
    memset(&macd,0,sizeof(StockMacd));
}

MacdIndex::~MacdIndex()
{
    deInit();
    delete cfg;
}

void MacdIndex::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
    macd.dea=(StockFloat*)malloc(sizeof(StockFloat)*stock->count);
    memset(macd.dea,0,sizeof(StockFloat)*stock->count);
    macd.diff=(StockFloat*)malloc(sizeof(StockFloat)*stock->count);
    memset(macd.diff,0,sizeof(StockFloat)*stock->count);
    macd.macd=(StockFloat*)malloc(sizeof(StockFloat)*stock->count);
    memset(macd.macd,0,sizeof(StockFloat)*stock->count);
}

void MacdIndex::deInit()
{
    if(macd.dea)
        free(macd.dea);
    if(macd.diff)
        free(macd.diff);
    if(macd.macd)
        free(macd.macd);
    memset(&macd,0,sizeof(StockMacd));
}


void MacdIndex::reCalcIndex()
{
    if(stock->count>0)
    {
        double* ema1=(double*)malloc(sizeof(double)*stock->count);
        double* ema2=(double*)malloc(sizeof(double)*stock->count);
        Algorithm::ema(stock->closes,ema1,stock->count,cfg->getFast());
        Algorithm::ema(stock->closes,ema2,stock->count,cfg->getSlow());
        for(int i=0;i<stock->count;i++)
        {
            macd.diff[i]=ema1[i]-ema2[i];
        }
        free(ema2);
        Algorithm::ema(macd.diff,macd.dea,stock->count,cfg->getDea());
        for(int i=0;i<stock->count;i++)
        {
            macd.macd[i]=2*(macd.diff[i]-macd.dea[i]);
        }
        free(ema1);
    }
}

void MacdIndex::reCalcRange(int start,int count)
{
    if(start<0)
        return ;
    if(start+count>stock->count)return;
    range.high=FLT_MIN;
    range.low=FLT_MAX;
    StockFloatRange range1,range2,range3;

    calcFloatRange(macd.dea,&range1,start,count);
    calcFloatRange(macd.diff,&range2,start,count);
    calcFloatRange(macd.macd,&range3,start,count);

    range.high=fmax(range1.high,range2.high);
    range.high=fmax(range.high,range3.high);

    range.low=fmin(range2.low,range1.low);
    range.low=fmin(range.low,range3.low);
}

const StockFloatRange* MacdIndex::getRange()
{
    return &range;
}

void MacdIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("diff=%1").arg(macd.diff[index],0,'f',2);
    strings.push_back(new ColorString(t,cfg->getDiffColor()));

    t=QString("dea=%1").arg(macd.dea[index],0,'f',2);
    strings.push_back(new ColorString(t,cfg->getDeaColor()));

    t=QString("macd=%1").arg(macd.macd[index],0,'f',2);
    strings.push_back(new ColorString(t,0xffffff));
}

ChartDraw* MacdIndex::getChartDraw()
{
    return this;
}

////

int MacdIndex::chartCount()
{
    return 3;
}

DataChartType MacdIndex::chartType(int index)
{
    if(index==2)
        return Chart_Pillar2;
    return Chart_Line;
}

bool MacdIndex::lineValue(int index,PChartLine pLine)//折线图
{
    switch(index)
    {
    case 0:
        pLine->color=cfg->getDiffColor();
        pLine->width=cfg->getDiffWidth();
        pLine->v=macd.diff+pLine->start;
        break;
    case 1:
        pLine->color=cfg->getDeaColor();
        pLine->width=cfg->getDeaWidth();
        pLine->v=macd.dea+pLine->start;
        break;
    default:
        return false;
    }
    return true;
}

bool MacdIndex::isPillarSysColor(int index)
{
    return false;
}

bool MacdIndex::pillarColor(int index,PChartPillar pillar)
{
    if(index==2)
    {
        //pillar->colors=(uint32_t*)malloc(sizeof(uint32_t)*pillar->count);
        pillar->v=macd.macd+pillar->start;
        for(int i=0;i<pillar->count;i++)
        {
            pillar->colors[i]=pillar->v[i]>0?0xff0000:0x00ff00;
        }

        pillar->width=0;
        pillar->zero_v=0;
        return true;
    }
    return false;
}
double MacdIndex::getZeroY()
{
    return 0;
}
void MacdIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{

}

StatisticIndexCfg* MacdIndex::getCfg()
{
    return cfg;
}
}
