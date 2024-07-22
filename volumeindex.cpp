#include "volumeindex.h"
namespace chart {
VolumeIndex::VolumeIndex(const char* indexId,const char* title,QObject *parent)
    : QObject(parent),StatisticIndex(indexId,title),pillarWidth(0)
{}

void VolumeIndex::init(const StockDayData* stock)
{
    deInit();
    StatisticIndex::init(stock);
}
const char* VolumeIndex::cfgName()const
{
    return getTitle();
}

const char* VolumeIndex::getIndexId()const
{
    return StatisticIndex::getIndexId();
}
void VolumeIndex::deInit()
{

}

void VolumeIndex::reCalcIndex()
{

}
void VolumeIndex::reCalcRange(int start,int count)
{
    ::calcFloatRange(stock->vol,&range,start,count);
}
const StockFloatRange* VolumeIndex::getRange()
{
    return &range;
}
void VolumeIndex::getItemDescription(int index,QList<ColorString*>& strings)
{
    QString t=QString("%1").arg(stock->vol[index],0,'f',2);
    strings.push_back(new ColorString(t,0xffffff));
}
ChartDraw* VolumeIndex::getChartDraw()
{
    return this;
}

////
int VolumeIndex::chartCount()
{
    return 1;
}
DataChartType VolumeIndex::chartType(int index)
{
    Q_UNUSED(index);
    return Chart_Pillar;
}
double VolumeIndex::getZeroY()
{
    return range.low;
}
bool VolumeIndex::lineValue(int index,PChartLine pLine)//折线图
{
    Q_UNUSED(index);
    return false;
}
bool VolumeIndex::isPillarSysColor(int index)//柱状图使用涨跌色
{
    Q_UNUSED(index);
    return true;
}
bool VolumeIndex::pillarColor(int index,PChartPillar pillar)//得到柱状图颜色
{
    Q_UNUSED(index);
    pillar->colors=NULL;
    pillar->v=stock->vol+pillar->start;
    pillar->width=pillarWidth;
    return true;
}
void VolumeIndex::chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)
{

}
StatisticIndexCfg* VolumeIndex::getCfg()
{
    return this;
}

bool VolumeIndex::loadCfg(QJsonObject* obj)
{
    pillarWidth=JsonValueToFloat(obj->value("width"));
    return true;
}
bool VolumeIndex::saveCfg(QJsonObject* obj)
{
    InsertFloatToJson(obj,"width",pillarWidth);
    return true;
}
bool VolumeIndex::loadDefault()
{
    pillarWidth=7;
    return true;
}
int VolumeIndex::getCfgCount()
{
    return 1;
}

bool VolumeIndex::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    Q_UNUSED(index);
    cfgValue->setFloat(tr("width"),pillarWidth);
    return true;
}

bool VolumeIndex::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    Q_UNUSED(index);
    pillarWidth=cfgValue->vFlt;
    return true;
}

}

