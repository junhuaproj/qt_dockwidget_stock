/*
 * 统计指标配置
*/
#ifndef STATISTICINDEX_H
#define STATISTICINDEX_H

#include "stockdaydata.h"
#include "cfgchart.h"
#include <QString>
#include "stockchartdraw.h"
#include <string.h>
#include <QJsonObject>
#include <QJsonObject>

namespace chart {
class IndexCfgValue
{
public:
    IndexCfgValue();
    virtual ~IndexCfgValue();
    void getInt(QString& name,int32_t& v,QString& description);
    void getStr(QString& name,QString& v,QString& description);

    void getColor(QString& name,uint32_t& v,QString& description);

    void getFloat(QString& name,float& v,QString& description);

    void setInt(const QString& name,int32_t v,const QString& description="");
    void setStr(const QString& name,const QString& v,const QString& description="");

    void setColor(const QString& name,uint32_t v,const QString& description="");

    void setFloat(const QString& name,float v,const QString& description="");

    QString name;
    QString vStr;
    uint32_t vClr;
    int32_t  vInt;
    float vFlt;
    DataValueType type;
    QString description;
};
inline float JsonValueToFloat(const QJsonValue& val,float defVal=0)
{
    return (float)val.toDouble(defVal);
}
inline int32_t JsonValueToInt(const QJsonValue& val,int32_t defVal=0)
{
    return val.toInt(defVal);
}

inline uint32_t JsonValueToUInt(const QJsonValue& val,uint32_t defVal=0)
{
    return (uint32_t)val.toInt(defVal);
}
inline bool JsonValueToChars(const QJsonValue& val,char* buf,int len)
{
    Q_UNUSED(len);
    strcpy(buf,val.toString().toStdString().c_str());
    return true;
}

inline void InsertFloatToJson(QJsonObject* parent,const QString& key,float v)
{
    parent->insert(key,QJsonValue(v));
}

inline void InsertIntToJson(QJsonObject* parent,const QString& key,int v)
{
    parent->insert(key,QJsonValue(v));
}

inline void InsertColorToJson(QJsonObject* parent,const QString& key,uint32_t v)
{
    parent->insert(key,QJsonValue((int)v));
}
inline void InsertStrToJson(QJsonObject* parent,const QString& key,const QString& val)
{
    parent->insert(key,QJsonValue(val));
}

// struct IndexAlgorithm
// {

// };

struct StatisticIndexCfg
{
    virtual bool loadCfg(QJsonObject* obj)=0;
    virtual bool saveCfg(QJsonObject* obj)=0;
    virtual const char* cfgName()const=0;
    virtual const char* getIndexId()const=0;
    virtual bool loadDefault()=0;
    virtual int getCfgCount()=0;
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const=0;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue)=0;
    //virtual DataValueType getCfgType(int cfgIndex)=0;
    //virtual bool getCfgValue(int index,QString& name,uint32_t& v,QString& description)=0;
    //virtual bool getCfgValue(int index,QString& name,QString& v,QString& description)=0;
    //virtual bool getCfgValue(int index,QString& name,int& v,QString& description)=0;

    //virtual bool setCfgValue(int index,uint32_t v)=0;
    //virtual bool setCfgValue(int index,const QString& v)=0;
    //virtual bool setCfgValue(int index,int v)=0;
};
struct IStatisticIndex{
    virtual void init(const StockDayData* stock)=0;
    virtual void deInit()=0;

    virtual bool isFinished()const=0;
    virtual const char* getIndexId()const=0;
    virtual const char* getTitle()const=0;
    virtual void reCalcIndex()=0;
    virtual void reCalcRange(int start,int count)=0;
    virtual const StockFloatRange* getRange()=0;
    virtual void getItemDescription(int index,QList<ColorString*>& strings)=0;
    virtual ChartDraw* getChartDraw()=0;
    virtual StatisticIndexCfg* getCfg()=0;
    //virtual void paint(const PaintParam& p,DayChartDrawParam* draw)=0;
    ////

};

class StatisticIndex:public IStatisticIndex
{
public:
    StatisticIndex(const char* indexId,const char* title)
        :finished(false)
    {
        this->indexId=(char*)malloc(strlen(indexId)+1);
        memcpy(this->indexId,indexId,strlen(indexId)+1);

        this->title=(char*)malloc(strlen(title)+1);
        //strcpy(this->title,title);
        memcpy(this->title,title,strlen(title)+1);
    }
    virtual ~StatisticIndex()
    {
        if(indexId)
            free(indexId);
        if(title)
            free(title);
    }
    virtual const char* getIndexId()const
    {
        return indexId;
    }
    virtual const char* getTitle()const
    {
        return title;
    }
    virtual bool isFinished()const
    {
        return finished;
    }
    virtual void init(const StockDayData* stock)
    {
        this->stock=stock;
    }
protected:
    void setFinished(bool bFinished)
    {
        finished=bFinished;
    }

    char* indexId;
    char* title;
    const StockDayData* stock;
private:
    bool finished;
};
}
#endif // STATISTICINDEX_H
