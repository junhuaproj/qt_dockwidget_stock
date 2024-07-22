#include "cfgchart.h"
#include <string.h>

#include "StatisticIndex.h"
namespace chart {
IndexCfgValue::IndexCfgValue()
{

}

IndexCfgValue::~IndexCfgValue()
{

}

void IndexCfgValue::getInt(QString& name,int32_t& v,QString& description)
{
    type=DataValue_Int;
    name=this->name;
    v=vInt;
    description=this->description;
}

void IndexCfgValue::getStr(QString& name,QString& v,QString& description)
{
    type=DataValue_Text;
    name=this->name;
    v=vStr;
    description=this->description;
}

void IndexCfgValue::getColor(QString& name,uint32_t& v,QString& description)
{
    type=DataValue_Clr;
    name=this->name;
    v=vClr;
    description=this->description;
}

void IndexCfgValue::getFloat(QString& name,float& v,QString& description)
{
    type=DataValue_Flt;
    name=this->name;
    v=vFlt;
    description=this->description;
}

void IndexCfgValue::setInt(const QString& name,int32_t v,const QString& description)
{
    this->name=name;
    this->description=description;
    type=DataValue_Int;
    vInt=v;
}
void IndexCfgValue::setStr(const QString& name,const QString& v,const QString& description)
{
    this->name=name;
    this->description=description;
    type=DataValue_Text;
    vStr=v;
}

void IndexCfgValue::setColor(const QString& name,uint32_t v,const QString& description)
{
    this->name=name;
    this->description=description;
    type=DataValue_Clr;
    vClr=v;
}

void IndexCfgValue::setFloat(const QString& name,float v,const QString& description)
{
    this->name=name;
    this->description=description;
    type=DataValue_Flt;
    vFlt=v;
}

PReferLine getNewReferLine(ReferLines* pLines)
{
    PReferLine line;
    if(pLines->count!=0)
    {
        line=pLines->lines;
        pLines->lines=(PReferLine)malloc(sizeof(ReferLine)*(pLines->count+1));
        memcpy(pLines->lines,line,sizeof(ReferLine)*pLines->count);
        free(line);
        line=pLines->lines+pLines->count;
        pLines->count++;

    }
    else
    {
        pLines->lines=(PReferLine)malloc(sizeof(ReferLine));
        pLines->count=1;
        line=pLines->lines;
    }
    return line;
}
}
