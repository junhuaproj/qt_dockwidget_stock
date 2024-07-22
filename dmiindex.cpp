#include "dmiindex.h"
#include <ta_func.h>
namespace chart {
DmiIndex::DmiIndex(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title)
{
    memset(&dmi,0,sizeof(StockDmi));
}
DmiIndex::~DmiIndex()
{
    deInit();
}
void DmiIndex::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
    dmi.pdi=(double*)malloc(sizeof(double)*stock->count);
    dmi.mdi =(double*)malloc(sizeof(double)*stock->count);
    dmi.adx=(double*)malloc(sizeof(double)*stock->count);
    dmi.adxr=(double*)malloc(sizeof(double)*stock->count);
    memset(dmi.pdi,0,sizeof(double)*stock->count);
    memset(dmi.mdi,0,sizeof(double)*stock->count);
    memset(dmi.adx,0,sizeof(double)*stock->count);
    memset(dmi.adxr,0,sizeof(double)*stock->count);
}
void DmiIndex::deInit()
{
    if(dmi.pdi)
        free(dmi.pdi);
    if(dmi.mdi)
        free(dmi.mdi);
    if(dmi.adx)
        free(dmi.adx);
    if(dmi.adxr)
        free(dmi.adxr);
    memset(&dmi,0,sizeof(StockDmi));
}
#include <math.h>
//using namespace std;
void DmiIndex::reCalcIndex()
{
    double* pdm,*mdm,*tr,*tr7;
    double *pdm7,*mdm7;
    pdm=(double*)malloc(sizeof(double)*stock->count);
    mdm=(double*)malloc(sizeof(double)*stock->count);
    tr=(double*)malloc(sizeof(double)*stock->count);
    tr7=(double*)malloc(sizeof(double)*stock->count);

    pdm7=(double*)malloc(sizeof(double)*stock->count);
    mdm7=(double*)malloc(sizeof(double)*stock->count);

    tr[0]=0;
    pdm[0]=0;
    mdm[0]=0;
    for(int i=1;i<stock->count;i++)
    {
        tr[i]=stock->closes[i]-stock->lows[i];
        tr[i]=fmax(tr[i],stock->lows[i]-stock->closes[i-1]);
        tr[i]=fmax(tr[i],stock->highs[i]-stock->closes[i-1]);
    }
    for(int i=1;i<stock->count;i++)
    {
        pdm[i]=stock->highs[i]-stock->highs[i-1];
        //if(pdm[i]<=fabs(stock->lows[i]-stock->lows[i-1]))
        //    pdm[i]=0;
        if(pdm[i]<=0)
            pdm[i]=0;

        mdm[i]=stock->lows[i-1]-stock->lows[i];
        //if(mdm[i]<=fabs(stock->highs[i]-stock->lows[i-1]))
        //    mdm[i]=0;
        if(mdm[i]<=0)
            mdm[i]=0;
        if(pdm[i]<mdm[i])
            pdm[i]=0;
        else
            mdm[i]=0;
    }

    for(int i=5;i<stock->count;i++)
    {
        tr7[i]=tr[i];
        pdm7[i]=pdm[i];
        mdm7[i]=mdm[i];
        for(int t=i-5;t<i;t++)
        {
            tr7[i]+=tr[t];
            pdm7[i]+=pdm[t];
            mdm7[i]+=mdm[t];
        }
        dmi.pdi[i]=pdm7[i]/tr7[i];
        dmi.mdi[i]=mdm7[i]/tr7[i];
    }
    dmi.adx[13]=0;
    for(int i=7;i<14;i++)
    {
        dmi.adx[13]=(dmi.pdi[i]-fabs(dmi.mdi[i]))-(dmi.pdi[i]+fabs(dmi.mdi[i]));
    }
    dmi.adx[13]/=7;
    for(int i=14;i<stock->count;i++)
    {
        dmi.adx[i]=dmi.adx[i-1]*6-((dmi.pdi[i]-fabs(dmi.mdi[i]))-(dmi.pdi[i]+fabs(dmi.mdi[i])));
        dmi.adx[i]/=7;
    }

    for(int i=20;i<stock->count;i++)
    {
        dmi.adxr[i]=(dmi.adx[i]+dmi.adx[i-6])/2;
    }
    //TA_PLUS_DI()
    free(tr);
    free(tr7);
    free(pdm);
    free(mdm);
    free(pdm7);
    free(mdm7);

}
void DmiIndex::reCalcRange(int start,int count)
{

}
const StockFloatRange* DmiIndex::getRange()
{
    return &range;
}
void DmiIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("mdi=%1").arg(dmi.mdi[index],0,'f',2);
    strings.push_back(new ColorString(t,0xff0000));

    t=QString("pdi=%1").arg(dmi.pdi[index],0,'f',2);
    strings.push_back(new ColorString(t,0xff0000));

    t=QString("adx=%1").arg(dmi.adx[index],0,'f',2);
    strings.push_back(new ColorString(t,0xff0000));

    t=QString("adxr=%1").arg(dmi.adxr[index],0,'f',2);
    strings.push_back(new ColorString(t,0xff0000));
}
ChartDraw* DmiIndex::getChartDraw()
{
    return this;
}
StatisticIndexCfg* DmiIndex::getCfg()
{
    return this;
}

int DmiIndex::chartCount()
{
    return 3;
}
DataChartType DmiIndex::chartType(int index)
{
    return Chart_Line;
}
double DmiIndex::getZeroY()//得到Y轴起点坐标
{
    return range.low;
}

bool DmiIndex::lineValue(int index,PChartLine pLine)//折线图
{
    pLine->width=1;
    pLine->color=0xff0000;
    switch(index)
    {
    case 0:
        //pLine->color=upperColor;
        pLine->v=dmi.mdi+pLine->start;
        break;
    case 1:
        //pLine->color=middleColor;
        pLine->v=dmi.pdi+pLine->start;
        break;
    case 2:
        //pLine->color=lowColor;
        pLine->v=dmi.adx+pLine->start;
        break;
    case 3:
        pLine->v=dmi.adxr+pLine->start;
        break;
    }
    return true;
}
bool DmiIndex::isPillarSysColor(int index)//柱状图使用涨跌色
{
    return true;
}
bool DmiIndex::pillarColor(int index,PChartPillar pillar)//得到柱状图颜色
{
    return false;
}

bool DmiIndex::loadCfg(QJsonObject* obj)
{
    /*upperColor=JsonValueToUInt(obj->value("upperColor"));
    middleColor=JsonValueToUInt(obj->value("middleColor"));
    lowColor=JsonValueToUInt(obj->value("lowerColor"));
    devUp=JsonValueToFloat(obj->value("devUp"));
    devDown=JsonValueToFloat(obj->value("devDown"));
    lineWidth=JsonValueToFloat(obj->value("lineWidth"));
    period=JsonValueToInt(obj->value("period"));*/
    return true;
}
bool DmiIndex::saveCfg(QJsonObject* obj)
{
    /*InsertColorToJson(obj,"upperColor",upperColor);
    InsertColorToJson(obj,"middleColor",middleColor);
    InsertColorToJson(obj,"lowerColor",lowColor);

    InsertFloatToJson(obj,"devUp",devUp);
    InsertFloatToJson(obj,"devDown",devDown);
    InsertFloatToJson(obj,"lineWidth",lineWidth);
    InsertIntToJson(obj,"period",period);*/
    return true;
}
const char* DmiIndex::cfgName()const
{
    return this->getTitle();
}
const char* DmiIndex::getIndexId()const
{
    return StatisticIndex::getIndexId();
}
bool DmiIndex::loadDefault()
{
    /*upperColor=0xffff00;
    middleColor=0x7f7f7f;
    lowColor=0x7f007f;
    lineWidth=1;
    period=20;
    devUp=2;
    devDown=2;*/
    return true;
}
int DmiIndex::getCfgCount()
{
    return 0;//BbandCfgVal_Count;
}
bool DmiIndex::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    /*switch(index)
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
    }*/
    return true;
}
bool DmiIndex::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    /*switch(index)
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
    }*/
    return true;
}

void DmiIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{

}
}
