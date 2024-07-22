#include "biasstatisticindex.h"
#include "ta_func.h"
#include <math.h>
#include "biasstatisticindexcfg.h"
//using namespace std;
namespace chart {
BiasStatisticIndex::BiasStatisticIndex(const char* indexId,const char* title,QObject *parent)
    :QObject(parent), StatisticIndex(indexId,title)//,stock(NULL)
{
    memset(&bias,0,sizeof(StockBias));
    cfg=new BiasStatisticIndexCfg(this);

}
BiasStatisticIndex::~BiasStatisticIndex()
{
    deInit();
    //free(indexId);
    delete cfg;
}

void BiasStatisticIndex::init(const StockDayData* stock)
{
    deInit();
    //this->stock=stock;
    StatisticIndex::init(stock);
    bias.count=cfg->getCfgLen();
    bias.days=(int*)malloc(sizeof(int)*bias.count);
    memcpy(bias.days,cfg->getDays(),sizeof(int)*bias.count);
    bias.values=(StockFloat**)malloc(sizeof(double*)*bias.count);
    for(int i=0;i<bias.count;i++)
    {
        void *p=malloc(sizeof(double)*stock->count);
        bias.values[i]=(StockFloat*)p;
        memset(bias.values[i],0,sizeof(StockFloat)*stock->count);
    }
    setFinished(true);
}

void BiasStatisticIndex::deInit()
{
    if(bias.count)
    {
        free(bias.days);
        for(int i=0;i<bias.count;i++)
        {
            free(bias.values[i]);
        }
        memset(&bias,0,sizeof(StockBias));
    }
    setFinished(false);
}


void BiasStatisticIndex::reCalcIndex()
{
    int begIdx=0,nbeElemet=0;
    TA_RetCode ret;
    int moveCount;
    for(int i=0;i<bias.count;i++)
    {
        ret=TA_MA(0,stock->count,stock->closes,bias.days[i],
                    TA_MAType_SMA,&begIdx,&nbeElemet,bias.values[i]);
        if(TA_SUCCESS==ret)
        {
            if(begIdx>0)
            {
                moveCount=std::min<int>(nbeElemet,stock->count-begIdx);
                memmove(bias.values[i]+begIdx,bias.values[i],sizeof(StockFloat)*moveCount);
                memset(bias.values[i],0,sizeof(StockFloat)*begIdx);
            }
        }
    }
    setFinished(true);
}
const StockFloatRange* BiasStatisticIndex::getRange()
{
    return &range;
}

void BiasStatisticIndex::reCalcRange(int start,int count)
{
    if(start<0)
        return ;
    if(start+count>stock->count)return;
    range.high=FLT_MIN;
    range.low=FLT_MAX;
    StockFloatRange range1;
    for(int i=0;i<bias.count;i++)
    {
        calcFloatRange(bias.values[i],&range1,start,count);
        range.high=fmax(range.high,range1.high);
        range.low=fmin(range.low,range1.low);
    }
}

void BiasStatisticIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t;
    for(int i=0;i<bias.count;i++)
    {
        t=QString("bias%1=%2").arg(bias.days[i]).arg(bias.values[i][index],0,'f',2);
        strings.push_back(new ColorString(t,0xffffff));
    }
}
StatisticIndexCfg* BiasStatisticIndex::getCfg()
{
    return cfg;
}
ChartDraw* BiasStatisticIndex::getChartDraw()
{
    return this;
}
int BiasStatisticIndex::chartCount()
{
    return bias.count;
}
DataChartType BiasStatisticIndex::chartType(int index)
{
    Q_UNUSED(index);
    return Chart_Line;
}
bool BiasStatisticIndex::lineValue(int index,PChartLine pLine)//折线图
{
    pLine->color=cfg->getColors()[index];
    pLine->width=cfg->getLineWidth()[index];
    pLine->v=bias.values[index]+pLine->start;
    //return bias.values[index];
    return true;
}
double BiasStatisticIndex::getZeroY()
{
    return range.low;
}
bool BiasStatisticIndex::isPillarSysColor(int index)
{
    Q_UNUSED(index);
    return false;
}
bool BiasStatisticIndex::pillarColor(int index,PChartPillar pillar)
{
    Q_UNUSED(index);
    Q_UNUSED(pillar);
    return false;
}
void BiasStatisticIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{
    Q_UNUSED(chartIndex)
    Q_UNUSED(dataIndex)
    Q_UNUSED(rc)
    Q_UNUSED(p)
}

}
