/*
 * 成交量图，柱形
*/
#ifndef VOLUMEINDEX_H
#define VOLUMEINDEX_H

#include <QObject>
#include "StatisticIndex.h"
namespace chart {
class VolumeIndex : public QObject,public StatisticIndex,public ChartDraw,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit VolumeIndex(const char* indexId,const char* title,QObject *parent = nullptr);
    virtual void init(const StockDayData* stock);
    virtual void deInit();

    virtual void reCalcIndex();
    virtual void reCalcRange(int start,int count);
    virtual const StockFloatRange* getRange();
    virtual void getItemDescription(int index,QList<ColorString*>& strings);
    virtual ChartDraw* getChartDraw();

    ////
    virtual int chartCount();
    virtual DataChartType chartType(int index);
    virtual double getZeroY();
    virtual bool lineValue(int index,PChartLine pLine);//折线图
    virtual bool isPillarSysColor(int index);//柱状图使用涨跌色
    virtual bool pillarColor(int index,PChartPillar pillar);//得到柱状图颜色
    virtual void chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p);
    virtual StatisticIndexCfg* getCfg();

    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual bool loadDefault();
    virtual const char* cfgName()const;
    const char* getIndexId()const;
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);
protected:
    float pillarWidth;
    //const StockDayData* stock;
    StockFloatRange range;
signals:
};
}
#endif // VOLUMEINDEX_H
