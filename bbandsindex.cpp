#include "bbandsindex.h"
#include "ta_func.h"
#include <math.h>
//using namespace std;

namespace chart {

enum BbandCfgVal
{
    BbandCfgVal_Period,
    BbandCfgVal_DevUp,
    BbandCfgVal_DevDown,
    BbandCfgVal_UpperColor,
    BbandCfgVal_LowColor,
    BbandCfgVal_MiddleColor,
    BbandCfgVal_LineWidth,
    BbandCfgVal_Count,
};



BbandsIndex::BbandsIndex(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title)
{
    memset(&bbands,0,sizeof(StockBbands));
}

BbandsIndex::~BbandsIndex()
{
    deInit();
}
void BbandsIndex::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
    bbands.lower=(double*)malloc(sizeof(double)*stock->count);
    bbands.middle =(double*)malloc(sizeof(double)*stock->count);
    bbands.upper=(double*)malloc(sizeof(double)*stock->count);
    memset(bbands.lower,0,sizeof(double)*stock->count);
    memset(bbands.middle,0,sizeof(double)*stock->count);
    memset(bbands.upper,0,sizeof(double)*stock->count);
}
void BbandsIndex::deInit()
{
    if(bbands.lower)
        free(bbands.lower);
    if(bbands.middle)
        free(bbands.middle);
    if(bbands.upper)
        free(bbands.upper);
    memset(&bbands,0,sizeof(StockBbands));
}
///
/// \brief BbandsIndex::reCalcIndex
/// middle ok,upper,lower有误差
void BbandsIndex::reCalcIndex()
{
    int begIdx,nbeElemet;
    TA_BBANDS(0,stock->count,stock->closes,period,devUp,devDown,
              TA_MAType_SMA,&begIdx,&nbeElemet,
              bbands.upper,bbands.middle,bbands.lower);

    int moveCount=std::min<int>(nbeElemet,stock->count-begIdx);
    memmove(bbands.upper+begIdx,bbands.upper,sizeof(StockFloat)*moveCount);
    memmove(bbands.middle+begIdx,bbands.middle,sizeof(StockFloat)*moveCount);
    memmove(bbands.lower+begIdx,bbands.lower,sizeof(StockFloat)*moveCount);

    memset(bbands.upper,0,begIdx*sizeof(double));
    memset(bbands.middle,0,begIdx*sizeof(double));
    memset(bbands.lower,0,begIdx*sizeof(double));
}
void BbandsIndex::reCalcRange(int start,int count)
{
    StockFloatRange range1,range2,range3;
    calcFloatRange(bbands.lower,&range1,start,count);
    calcFloatRange(bbands.middle,&range2,start,count);
    calcFloatRange(bbands.upper,&range3,start,count);
    range.low=fmin(range1.low,range2.low);
    range.high=fmax(range1.high,range2.high);

    range.low=fmin(range3.low,range.low);
    range.high=fmax(range3.high,range.high);
}
const StockFloatRange* BbandsIndex::getRange()
{
    return &range;
}
void BbandsIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("low=%1").arg(bbands.lower[index],0,'f',2);
    strings.push_back(new ColorString(t,lowColor));

    t=QString("middle=%1").arg(bbands.middle[index],0,'f',2);
    strings.push_back(new ColorString(t,middleColor));

    t=QString("upper=%1").arg(bbands.upper[index],0,'f',2);
    strings.push_back(new ColorString(t,upperColor));
}
ChartDraw* BbandsIndex::getChartDraw()
{
    return this;
}
StatisticIndexCfg* BbandsIndex::getCfg()
{
    return this;
}

int BbandsIndex::chartCount()
{
    return 3;
}
DataChartType BbandsIndex::chartType(int index)
{
    return Chart_Line;
}
double BbandsIndex::getZeroY()//得到Y轴起点坐标
{
    return range.low;
}

bool BbandsIndex::lineValue(int index,PChartLine pLine)//折线图
{
    pLine->width=lineWidth;
    switch(index)
    {
    case 0:
        pLine->color=upperColor;
        pLine->v=bbands.upper+pLine->start;
        break;
    case 1:
        pLine->color=middleColor;
        pLine->v=bbands.middle+pLine->start;
        break;
    case 2:
        pLine->color=lowColor;
        pLine->v=bbands.lower+pLine->start;
        break;
    }
    return true;
}
bool BbandsIndex::isPillarSysColor(int index)//柱状图使用涨跌色
{
    return true;
}
bool BbandsIndex::pillarColor(int index,PChartPillar pillar)//得到柱状图颜色
{
    return false;
}

bool BbandsIndex::loadCfg(QJsonObject* obj)
{
    upperColor=JsonValueToUInt(obj->value("upperColor"));
    middleColor=JsonValueToUInt(obj->value("middleColor"));
    lowColor=JsonValueToUInt(obj->value("lowerColor"));
    devUp=JsonValueToFloat(obj->value("devUp"));
    devDown=JsonValueToFloat(obj->value("devDown"));
    lineWidth=JsonValueToFloat(obj->value("lineWidth"));
    period=JsonValueToInt(obj->value("period"));
    return true;
}
bool BbandsIndex::saveCfg(QJsonObject* obj)
{
    InsertColorToJson(obj,"upperColor",upperColor);
    InsertColorToJson(obj,"middleColor",middleColor);
    InsertColorToJson(obj,"lowerColor",lowColor);

    InsertFloatToJson(obj,"devUp",devUp);
    InsertFloatToJson(obj,"devDown",devDown);
    InsertFloatToJson(obj,"lineWidth",lineWidth);
    InsertIntToJson(obj,"period",period);
    return true;
}
const char* BbandsIndex::cfgName()const
{
    return this->getTitle();
}
const char* BbandsIndex::getIndexId()const
{
    return StatisticIndex::getIndexId();
}
bool BbandsIndex::loadDefault()
{
    upperColor=0xffff00;
    middleColor=0x7f7f7f;
    lowColor=0x7f007f;
    lineWidth=1;
    period=20;
    devUp=2;
    devDown=2;
    return true;
}
int BbandsIndex::getCfgCount()
{
    return BbandCfgVal_Count;
}
bool BbandsIndex::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    switch(index)
    {
    case BbandCfgVal_Period:
        cfgValue->setInt(tr("period"),period);break;
    case BbandCfgVal_DevUp:
        cfgValue->setFloat(tr("dev up"),devUp);break;
    case BbandCfgVal_DevDown:
        cfgValue->setFloat(tr("dev down"),devDown);break;
    case BbandCfgVal_UpperColor:
        cfgValue->setColor(tr("upper color"),upperColor);break;
    case BbandCfgVal_LowColor:
        cfgValue->setColor(tr("lower color"),lowColor);break;
    case BbandCfgVal_MiddleColor:
        cfgValue->setColor(tr("middle color"),middleColor);break;
    case BbandCfgVal_LineWidth:
        cfgValue->setFloat(tr("line width"),lineWidth);
        break;
    default:
        return false;
    }
    return true;
}
bool BbandsIndex::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    switch(index)
    {
    case BbandCfgVal_Period:
        period=cfgValue->vInt;break;
    case BbandCfgVal_DevUp:
        devUp=cfgValue->vFlt;break;
    case BbandCfgVal_DevDown:
        devDown=cfgValue->vFlt;break;
    case BbandCfgVal_UpperColor:
        upperColor=cfgValue->vClr;break;
    case BbandCfgVal_LowColor:
        lowColor=cfgValue->vClr;break;
    case BbandCfgVal_MiddleColor:
        middleColor=cfgValue->vClr;
    case BbandCfgVal_LineWidth:
        lineWidth=cfgValue->vFlt;
        break;
    default:
        return false;
    }
    return true;
}

void BbandsIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{

}
}
