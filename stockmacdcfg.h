/*
 * MACD指标配置
*/
#ifndef STOCKMACDCFG_H
#define STOCKMACDCFG_H

#include <QObject>
#include "StatisticIndex.h"
namespace chart {
class MacdIndex;
class StockMacdCfg : public QObject,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit StockMacdCfg(MacdIndex* pIndex,QObject *parent = nullptr);

    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual const char* cfgName()const;
    virtual const char* getIndexId()const;
    virtual bool loadDefault();
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);

    uint16_t getSlow()const;
    uint16_t getFast()const;
    uint16_t getDea()const;
    float getDeaWidth() const;

    float getDiffWidth() const;

    float getMacdWidth() const;

    uint32_t getDiffColor() const;

    uint32_t getDeaColor() const;

protected:
    MacdIndex* pIndex;
    float diffWidth;
    float deaWidth;
    float macdWidth;
    uint32_t diffColor;
    uint32_t deaColor;

    uint16_t slow;
    uint16_t fast;
    uint16_t dea;
signals:
};
}
#endif // STOCKMACDCFG_H
