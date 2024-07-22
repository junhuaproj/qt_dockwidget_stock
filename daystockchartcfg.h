/**
 * 统计指标配置基类
*/
#ifndef DAYSTOCKCHARTCFG_H
#define DAYSTOCKCHARTCFG_H

#include <QObject>
#include <StatisticIndex.h>
namespace chart {
class DayStockChartCfg : public QObject,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit DayStockChartCfg(DayChartParam* pCfg,ChartCfg* chartCfg,QObject *parent = nullptr);

    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual const char* cfgName()const;
    virtual const char* getIndexId()const;
    virtual bool loadDefault();
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);
protected:
    DayChartParam* pCfg;
    ChartCfg* pChartCfg;
signals:
};
}
#endif // DAYSTOCKCHARTCFG_H
