/*
 * 统计指标参数配置
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
    //整数类型
    void getInt(QString& name,int32_t& v,QString& description);
    //字符串类型
    void getStr(QString& name,QString& v,QString& description);

    //颜色
    void getColor(QString& name,uint32_t& v,QString& description);
    //浮点型
    void getFloat(QString& name,float& v,QString& description);

    void setInt(const QString& name,int32_t v,const QString& description="");
    void setStr(const QString& name,const QString& v,const QString& description="");

    void setColor(const QString& name,uint32_t v,const QString& description="");

    void setFloat(const QString& name,float v,const QString& description="");

    //名称
    QString name;
    //字符串类型的值
    QString vStr;
    //颜色，无符号32位整型
    uint32_t vClr;
    //32位有符号整数
    int32_t  vInt;
    //浮点数
    float vFlt;
    //类型
    DataValueType type;
    //字段描述
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
/*整数指标参数配置接口*/
struct StatisticIndexCfg
{
    //从json加载配置
    virtual bool loadCfg(QJsonObject* obj)=0;
    //保存到json对象
    virtual bool saveCfg(QJsonObject* obj)=0;
    //配置名称
    virtual const char* cfgName()const=0;
    //配置ID
    virtual const char* getIndexId()const=0;
    //加载默认配置
    virtual bool loadDefault()=0;
    //获得配置项数
    virtual int getCfgCount()=0;
    //获得指定配置索引的值
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const=0;
    //设置指定配置索引的值
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue)=0;
};
/*
 * K线图中统计指标
*/
struct IStatisticIndex{
    /*
     * 初始化，一般是需要准备计算结果数据的内存
    */
    virtual void init(const StockDayData* stock)=0;
    /*
     * 反初始化，清理内存
    */
    virtual void deInit()=0;
    /*
     * 数据是否已经准备好
    */
    virtual bool isFinished()const=0;
    /**
     * 指标ID
    */
    virtual const char* getIndexId()const=0;
    /**
     * 指标名称
    */
    virtual const char* getTitle()const=0;
    /**
     * 重新计算指标
    */
    virtual void reCalcIndex()=0;
    /**
     * 重新计算显示区间的最大小值
    */
    virtual void reCalcRange(int start,int count)=0;
    /*
     * 获得显示的数值区间
    */
    virtual const StockFloatRange* getRange()=0;
    /*
     * 获得指定索引的数值描述，已经显示数值的颜色
    */
    virtual void getItemDescription(int index,QList<ColorString*>& strings)=0;
    virtual ChartDraw* getChartDraw()=0;
    /*
     * 获得指标配置
    */
    virtual StatisticIndexCfg* getCfg()=0;
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
