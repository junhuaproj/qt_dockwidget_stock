#include "stockmacdcfg.h"
#include "macdindex.h"
namespace chart {
enum MacdCfgCfgVal
{
    MacdCfgCfgVal_Slow,
    MacdCfgCfgVal_Fast,
    MacdCfgCfgVal_Dea,
    MacdCfgCfgVal_DiffWidth,
    MacdCfgCfgVal_DeaWidth,
    MacdCfgCfgVal_MacdWidth,
    MacdCfgCfgVal_DiffColor,
    MacdCfgCfgVal_DeaColor,
    MacdCfgCfgVal_Count
};

StockMacdCfg::StockMacdCfg(MacdIndex* p,QObject *parent)
    : QObject(parent),slow(0),fast(0),dea(0),pIndex(p)
{

}


bool StockMacdCfg::loadCfg(QJsonObject* obj)
{
    slow=JsonValueToInt(obj->value("slow"));
    fast=JsonValueToInt(obj->value("fast"));
    dea=JsonValueToInt(obj->value("dea"));
    diffWidth=JsonValueToFloat(obj->value("diffWidth"));
    deaWidth=JsonValueToFloat(obj->value("deaWidth"));
    diffColor=JsonValueToUInt(obj->value("diffColor"));
    deaColor=JsonValueToUInt(obj->value("deaColor"));
    macdWidth=JsonValueToFloat(obj->value("macdWidth"));
    return true;
}
const char* StockMacdCfg::cfgName()const
{
    return pIndex->getTitle();
}

const char* StockMacdCfg::getIndexId()const
{
    return pIndex->getIndexId();
}

bool StockMacdCfg::saveCfg(QJsonObject* obj)
{
    InsertIntToJson(obj,"slow",slow);
    InsertIntToJson(obj,"fast",fast);
    InsertIntToJson(obj,"dea",dea);

    InsertFloatToJson(obj,"diffWidth",diffWidth);
    InsertFloatToJson(obj,"deaWidth",deaWidth);
    InsertFloatToJson(obj,"macdWidth",macdWidth);

    InsertColorToJson(obj,"diffColor",diffColor);
    InsertColorToJson(obj,"deaColor",deaColor);
    return true;
}

bool StockMacdCfg::loadDefault()
{
    slow=26;
    fast=12;
    dea=9;
    deaWidth=1;
    diffWidth=1;
    deaColor=0xffff00;
    diffColor=0xffffff;
    macdWidth=10;
    return true;
}

int StockMacdCfg::getCfgCount()
{
    return MacdCfgCfgVal_Count;
}


bool StockMacdCfg::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    switch((MacdCfgCfgVal)index)
    {
    case MacdCfgCfgVal_Slow:
        cfgValue->setInt(tr("slow"),slow);break;
    case MacdCfgCfgVal_Fast:
        cfgValue->setInt(tr("fast"),fast);break;
    case MacdCfgCfgVal_Dea:
        cfgValue->setInt(tr("dea"),dea);break;
    case MacdCfgCfgVal_DiffWidth:
        cfgValue->setFloat(tr("diff width"),diffWidth);break;
    case MacdCfgCfgVal_DeaWidth:
        cfgValue->setFloat(tr("dea width"),deaWidth);break;
    case MacdCfgCfgVal_MacdWidth:
        cfgValue->setFloat(tr("macd width"),macdWidth);break;
    case MacdCfgCfgVal_DiffColor:
        cfgValue->setColor(tr("diff color"),diffColor);break;
    case MacdCfgCfgVal_DeaColor:
        cfgValue->setColor(tr("dea color"),deaColor);break;
        break;
    default:
        return false;
    }

    return true;
}

bool StockMacdCfg::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    switch(index)
    {
    case MacdCfgCfgVal_Slow:
        slow=cfgValue->vInt;break;
    case MacdCfgCfgVal_Fast:fast=cfgValue->vInt;break;
    case MacdCfgCfgVal_Dea:dea=cfgValue->vInt;break;
    case MacdCfgCfgVal_DiffWidth:diffWidth=cfgValue->vFlt;break;
    case MacdCfgCfgVal_DeaWidth:deaWidth=cfgValue->vFlt;break;
    case MacdCfgCfgVal_MacdWidth:macdWidth=cfgValue->vFlt;break;
    case MacdCfgCfgVal_DiffColor:diffColor=cfgValue->vClr;break;
    case MacdCfgCfgVal_DeaColor:deaColor=cfgValue->vClr;break;
    default:
        return false;
    }

    return true;
}

uint16_t StockMacdCfg::getSlow()const
{
    return slow;
}
uint16_t StockMacdCfg::getFast()const
{
    return fast;
}
uint16_t StockMacdCfg::getDea()const
{
    return dea;
}

float StockMacdCfg::getDeaWidth() const
{
    return deaWidth;
}

float StockMacdCfg::getDiffWidth() const
{
    return diffWidth;
}

float StockMacdCfg::getMacdWidth() const
{
    return macdWidth;
}

uint32_t StockMacdCfg::getDiffColor() const
{
    return diffColor;
}

uint32_t StockMacdCfg::getDeaColor() const
{
    return deaColor;
}
}
