/*
 * KDJ指标配置
*/

#ifndef KDJINDEXCFG_H
#define KDJINDEXCFG_H

#include <QObject>
#include "StatisticIndex.h"
namespace chart{
class KdjIndex;
class KdjIndexCfg : public QObject,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit KdjIndexCfg(KdjIndex* pIndex,QObject *parent = nullptr);


    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual const char* cfgName()const;
    virtual const char* getIndexId()const;
    virtual bool loadDefault();
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);
    /*virtual DataValueType getCfgType(int cfgIndex);
    virtual bool getCfgValue(int index,QString& name,uint32_t& v,QString& description);
    virtual bool getCfgValue(int index,QString& name,QString& v,QString& description);
    virtual bool getCfgValue(int index,QString& name,int& v,QString& description);

    virtual bool setCfgValue(int index,uint32_t v);
    virtual bool setCfgValue(int index,const QString& v);
    virtual bool setCfgValue(int index,int v);*/

    /**
     * K线颜色
    */
    uint32_t getKColor() const;
    /**
     * D线颜色
    */
    uint32_t getDColor() const;

    /**
     * J线颜色
    */
    uint32_t getJColor() const;
    /**
     * Fast K周期
    */
    int getFaskKday() const;
    /**
     * Slow K周期
    */
    int getSLowKday() const;
    /**
     * Slow D周期
    */
    int getSlowDday() const;

    /**
     * 线宽
    */
    float getLineWidth() const;

protected:
    KdjIndex* pIndex;
    uint32_t kColor;
    uint32_t dColor;
    uint32_t jColor;
    int fastKday;
    int slowKday;
    int slowDday;

    float lineWidth;
signals:
};
}
#endif // KDJINDEXCFG_H
