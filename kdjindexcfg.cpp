#include "kdjindexcfg.h"
#include "kdjindex.h"
namespace chart{
enum KdjCfgVal
{
    KdjCfgVal_FastK,
    KdjCfgVal_SlowK,
    KdjCfgVal_SlowD,
    KdjCfgVal_ColorK,
    KdjCfgVal_ColorD,
    KdjCfgVal_ColorJ,
    KdjCfgVal_Width,

    KdjCfgVal_Count,
};

KdjIndexCfg::KdjIndexCfg(KdjIndex* p,QObject *parent)
    : QObject(parent),pIndex(p)
{

}

const char* KdjIndexCfg::cfgName()const
{
    return pIndex->getTitle();
}
uint32_t KdjIndexCfg::getKColor() const
{
    return kColor;
}

uint32_t KdjIndexCfg::getDColor() const
{
    return dColor;
}

uint32_t KdjIndexCfg::getJColor() const
{
    return jColor;
}

int KdjIndexCfg::getFaskKday() const
{
    return fastKday;
}

int KdjIndexCfg::getSLowKday() const
{
    return slowKday;
}

int KdjIndexCfg::getSlowDday() const
{
    return slowDday;
}

float KdjIndexCfg::getLineWidth() const
{
    return lineWidth;
}

bool KdjIndexCfg::loadCfg(QJsonObject* obj)
{
    kColor=JsonValueToUInt(obj->value("kColor"),0);
    dColor=JsonValueToUInt(obj->value("dColor"),0);
    jColor=JsonValueToUInt(obj->value("jColor"),0);
    fastKday=JsonValueToInt(obj->value("fastKDay"),0);
    slowKday=JsonValueToInt(obj->value("slowKDay"),0);
    slowDday=JsonValueToInt(obj->value("slowDDay"),0);
    lineWidth=JsonValueToFloat(obj->value("lineWidth"),0);
    return true;
}
bool KdjIndexCfg::saveCfg(QJsonObject* obj)
{
    InsertColorToJson(obj,"kColor",kColor);
    InsertColorToJson(obj,"dColor",dColor);
    InsertColorToJson(obj,"jColor",jColor);

    InsertIntToJson(obj,"fastKDay",fastKday);
    InsertIntToJson(obj,"slowKDay",slowKday);
    InsertIntToJson(obj,"slowDDay",slowDday);
    InsertFloatToJson(obj,"lineWidth",lineWidth);
    return true;
}

const char* KdjIndexCfg::getIndexId()const
{
    return pIndex->getIndexId();
}
bool KdjIndexCfg::loadDefault()
{
    kColor=0xffffff;
    dColor=0xffff00;
    jColor=0xff00ff;
    fastKday=9;
    slowKday=3;
    slowDday=3;
    lineWidth=1;
    return true;
}
int KdjIndexCfg::getCfgCount()
{
    return KdjCfgVal_Count;
}

bool KdjIndexCfg::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    switch((KdjCfgVal)index)
    {
    case KdjCfgVal_FastK:
        cfgValue->setInt(tr("fast k"),fastKday);
        break;
    case KdjCfgVal_SlowK:
        cfgValue->setInt(tr("slow k"),slowKday);
        break;
    case KdjCfgVal_SlowD:
        cfgValue->setInt(tr("slow d"),slowDday);
        break;
    case KdjCfgVal_ColorK:
        cfgValue->setColor(tr("color k"),kColor);
        break;
    case KdjCfgVal_ColorD:
        cfgValue->setColor(tr("color d"),dColor);
        break;
    case KdjCfgVal_ColorJ:
        cfgValue->setColor(tr("color j"),jColor);
        break;
    case KdjCfgVal_Width:
        cfgValue->setFloat(tr("width k"),lineWidth);
        break;
    default:
        return false;
    }

    return true;
}

bool KdjIndexCfg::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    switch((KdjCfgVal)index)
    {
    case KdjCfgVal_FastK:
        fastKday=cfgValue->vInt;
        break;
    case KdjCfgVal_SlowK:
        slowKday=cfgValue->vInt;
        break;
    case KdjCfgVal_SlowD:
        slowDday=cfgValue->vInt;
        break;
    case KdjCfgVal_ColorK:
        kColor=cfgValue->vClr;
        break;
    case KdjCfgVal_ColorD:
        dColor=cfgValue->vClr;
        break;
    case KdjCfgVal_ColorJ:
        jColor=cfgValue->vClr;
        break;
    case KdjCfgVal_Width:
        lineWidth=cfgValue->vFlt;
        break;
    default:
        return false;
    }
    return true;
}


}
