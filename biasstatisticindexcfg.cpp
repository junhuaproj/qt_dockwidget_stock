#include "biasstatisticindexcfg.h"
#include "biasstatisticindex.h"

namespace chart {
#define BIAS_KEY_NUM    3
const char biasKeys[BIAS_KEY_NUM][8]={"day","color","width"};
const char biasNames[BIAS_KEY_NUM][8]={"day","color","width"};

BiasStatisticIndexCfg::BiasStatisticIndexCfg(BiasStatisticIndex* p,QObject *parent)
    :QObject(parent),cfgLen(0),days(NULL),pIndex(p)
{


}

BiasStatisticIndexCfg::~BiasStatisticIndexCfg()
{
    freeCfg();
}

const char* BiasStatisticIndexCfg::getIndexId()const
{
    return pIndex->getIndexId();
}
const char* BiasStatisticIndexCfg::cfgName()const
{
    return pIndex->getTitle();
}
void BiasStatisticIndexCfg::freeCfg()
{
    if(cfgLen)
    {
        free(days);
        free(lineWidth);
        free(colors);
        days=NULL;
        lineWidth=NULL;
        colors=NULL;
    }
}

bool BiasStatisticIndexCfg::loadDefault()
{
    freeCfg();
    initCfg(3);
    //days=(int*)malloc(sizeof(int)*cfgLen);
    days[0]=6;
    days[1]=12;
    days[2]=24;

    //lineWidth=(float*)malloc(sizeof(float)*cfgLen);
    //colors=(uint32_t*)malloc(sizeof(uint32_t)*cfgLen);
    lineWidth[0]=1;
    colors[0]=0xffffff;

    lineWidth[1]=1;
    colors[1]=0xffff00;

    lineWidth[2]=1;
    colors[2]=0xff00ff;
    return true;
}

void BiasStatisticIndexCfg::initCfg(int cfgLen)
{
    this->cfgLen=cfgLen;
    days=(int*)malloc(sizeof(int)*cfgLen);
    lineWidth=(float*)malloc(sizeof(float)*cfgLen);
    colors=(uint32_t*)malloc(sizeof(uint32_t)*cfgLen);
    memset(days,0,sizeof(int)*cfgLen);
}
bool BiasStatisticIndexCfg::loadCfg(QJsonObject* obj)
{
    int len=obj->length();
    freeCfg();
    initCfg(len/BIAS_KEY_NUM);

    QString key;
    for(int i=0;i<cfgLen;i++)
    {
        key=QString("%1%2").arg(biasKeys[0]).arg(i+1);
        days[i]=JsonValueToInt(obj->value(key));

        key=QString("%1%2").arg(biasKeys[1]).arg(i+1);
        colors[i]=JsonValueToUInt(obj->value(key));

        key=QString("%1%2").arg(biasKeys[2]).arg(i+1);
        lineWidth[i]=JsonValueToFloat(obj->value(key));
    }
    return true;
}
bool BiasStatisticIndexCfg::saveCfg(QJsonObject* obj)
{
    QString key;
    int valueIndex;
    for(int i=0;i<cfgLen;i++)
    {
        key=QString("%1%2").arg(biasKeys[0]).arg(i+1);
        InsertIntToJson(obj,key,days[i]);

        key=QString("%1%2").arg(biasKeys[1]).arg(i+1);
        InsertColorToJson(obj,key,colors[i]);

        key=QString("%1%2").arg(biasKeys[2]).arg(i+1);
        InsertFloatToJson(obj,key,lineWidth[i]);

    }
    return true;
}
int BiasStatisticIndexCfg::getCfgCount()
{
    return cfgLen*BIAS_KEY_NUM;
}

bool BiasStatisticIndexCfg::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    const int lineIndex=index%BIAS_KEY_NUM;
    const int valueIndex=index/BIAS_KEY_NUM;

    QString key=QString("%1%2").arg(biasNames[lineIndex]).arg(valueIndex+1);
    switch(lineIndex)
    {
    case 0:
        cfgValue->setInt(key,days[valueIndex]);
        break;
    case 1:
        cfgValue->setColor(key,colors[valueIndex]);
        break;
    case 2:
        cfgValue->setFloat(key,lineWidth[valueIndex]);
        break;
    }

    return true;
}

bool BiasStatisticIndexCfg::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    const int lineIndex=index%BIAS_KEY_NUM;

    QString key=QString("%1%2").arg(biasNames[lineIndex]).arg(lineIndex+1);
    switch(lineIndex)
    {
    case 0:
        days[lineIndex]=cfgValue->vInt;
        break;
    case 1:
        colors[lineIndex]=cfgValue->vClr;
        break;
    case 2:
        lineWidth[lineIndex]=cfgValue->vFlt;
        break;
    }

    return true;
}

int BiasStatisticIndexCfg::getCfgLen()const
{
    return cfgLen;
}
const int* BiasStatisticIndexCfg::getDays()const
{
    return days;
}

float *BiasStatisticIndexCfg::getLineWidth() const
{
    return lineWidth;
}

uint32_t *BiasStatisticIndexCfg::getColors() const
{
    return colors;
}
}
