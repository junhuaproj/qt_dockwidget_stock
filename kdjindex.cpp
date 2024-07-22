#include "kdjindex.h"
#include <ta_func.h>
#include <math.h>

namespace chart{
KdjIndex::KdjIndex(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title)
{
    memset(&kdj,0,sizeof(StockKdj));
    cfg=new KdjIndexCfg(this);
}

KdjIndex::~KdjIndex()
{
    delete cfg;
    deInit();
}

void KdjIndex::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
    kdj.k=(double*)malloc(sizeof(double)*stock->count);
    kdj.d=(double*)malloc(sizeof(double)*stock->count);
    kdj.j=(double*)malloc(sizeof(double)*stock->count);

    memset(kdj.k,0,sizeof(double)*stock->count);
    memset(kdj.d,0,sizeof(double)*stock->count);
    memset(kdj.j,0,sizeof(double)*stock->count);
}
void KdjIndex::deInit()
{
    if(kdj.d)
        free(kdj.d);
    if(kdj.k)
        free(kdj.k);
    if(kdj.j)
        free(kdj.j);
    memset(&kdj,0,sizeof(StockKdj));
}


void KdjIndex::reCalcIndex()
{
    uint32_t count=stock->count;

    int begIdx=0,nbeElemet=0;

    TA_STOCH(0,count,stock->highs,stock->lows,stock->closes,
             cfg->getFaskKday(),
             cfg->getSLowKday()*2-1,TA_MAType_EMA,
             cfg->getSlowDday()*2-1,TA_MAType_EMA,
             &begIdx,&nbeElemet,
             kdj.k,kdj.d);
    int moveCount=std::min<int>(nbeElemet,count-begIdx);
    memmove(kdj.k+begIdx,kdj.k,sizeof(StockFloat)*moveCount);
    memmove(kdj.d+begIdx,kdj.d,sizeof(StockFloat)*moveCount);

    memset(kdj.k,0,begIdx*sizeof(double));
    memset(kdj.d,0,begIdx*sizeof(double));
    for(int i=begIdx;i<nbeElemet+begIdx&&i<count;i++)
    {
        kdj.j[i]=3*kdj.k[i]-2*kdj.d[i];
    }
}

void KdjIndex::reCalcRange(int start,int count)
{
    StockFloatRange range1,range2,range3;
    calcFloatRange(kdj.k,&range1,start,count);
    calcFloatRange(kdj.d,&range2,start,count);
    calcFloatRange(kdj.j,&range3,start,count);
    range.low=fmin(range1.low,range2.low);
    range.high=fmax(range1.high,range2.high);

    range.low=fmin(range3.low,range.low);
    range.high=fmax(range3.high,range.high);
}
const StockFloatRange* KdjIndex::getRange()
{
    return &range;
}
void KdjIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("k=%1").arg(kdj.k[index],0,'f',2);
    strings.push_back(new ColorString(t,cfg->getKColor()));

    t=QString("d=%1").arg(kdj.d[index],0,'f',2);
    strings.push_back(new ColorString(t,cfg->getDColor()));

    t=QString("j=%1").arg(kdj.j[index],0,'f',2);
    strings.push_back(new ColorString(t,cfg->getJColor()));
}
ChartDraw* KdjIndex::getChartDraw()
{
    return this;
}
StatisticIndexCfg* KdjIndex::getCfg()
{
    return cfg;
}
////
int KdjIndex::chartCount()
{
    return 3;
}
DataChartType KdjIndex::chartType(int index)
{
    return Chart_Line;
}
double KdjIndex::getZeroY()//得到Y轴起点坐标
{
    return range.low;
}

bool KdjIndex::lineValue(int index,PChartLine pLine)//折线图
{
    pLine->width=cfg->getLineWidth();
    switch(index)
    {
    case 0:
        pLine->color=cfg->getKColor();
        pLine->v=kdj.k+pLine->start;
        break;
    case 1:
        pLine->color=cfg->getDColor();
        pLine->v=kdj.d+pLine->start;
        break;
    case 2:
        pLine->color=cfg->getJColor();
        pLine->v=kdj.j+pLine->start;
        break;
    }
    return true;
}
bool KdjIndex::isPillarSysColor(int index)//柱状图使用涨跌色
{
    return true;
}
bool KdjIndex::pillarColor(int index,PChartPillar pillar)//得到柱状图颜色
{
    return false;
}
void KdjIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{

}
}
