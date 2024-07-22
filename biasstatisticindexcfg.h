
/*
 * Bias指标参数设置
*/
#ifndef BIASSTATISTICINDEXCFG_H
#define BIASSTATISTICINDEXCFG_H

#include <QObject>
#include "StatisticIndex.h"
namespace chart {
class BiasStatisticIndex;

class BiasStatisticIndexCfg : public QObject,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit BiasStatisticIndexCfg(BiasStatisticIndex* pIndex,QObject *parent = nullptr);

    ~BiasStatisticIndexCfg();
    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual const char* cfgName()const;
    virtual const char* getIndexId()const;
    virtual bool loadDefault();
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);

    int getCfgLen()const;
    const int* getDays()const;
    float *getLineWidth() const;

    uint32_t *getColors() const;

protected:
    BiasStatisticIndex* pIndex;
    void initCfg(int cfgLen);
    void freeCfg();
    float* lineWidth;
    uint32_t* colors;
    int cfgLen;
    int* days;
signals:
};
}
#endif // BIASSTATISTICINDEXCFG_H
