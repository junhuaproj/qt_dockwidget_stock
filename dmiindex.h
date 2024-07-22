/*
 * DMI指标
*/
#ifndef DMIINDEX_H
#define DMIINDEX_H

#include <QObject>
#include "StatisticIndex.h"
namespace chart {
struct StockDmi
{
    double* pdi;
    double* mdi;
    double* adx;
    double* adxr;
};

class DmiIndex : public QObject,public StatisticIndex,public ChartDraw,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit DmiIndex(const char* indexId,const char* title,QObject *parent = nullptr);
    virtual ~DmiIndex();
    virtual void init(const StockDayData* stock);
    virtual void deInit();

    virtual void reCalcIndex();
    virtual void reCalcRange(int start,int count);
    virtual const StockFloatRange* getRange();
    virtual void getItemDescription(int index,QList<ColorString*>& strings);
    virtual ChartDraw* getChartDraw();
    virtual StatisticIndexCfg* getCfg();

    virtual int chartCount();
    virtual DataChartType chartType(int index);
    virtual double getZeroY();//得到Y轴起点坐标

    virtual bool lineValue(int index,PChartLine pLine);//折线图
    virtual bool isPillarSysColor(int index);//柱状图使用涨跌色
    virtual bool pillarColor(int index,PChartPillar pillar);//得到柱状图颜色
    virtual void chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p);

    virtual bool loadCfg(QJsonObject* obj);
    virtual bool saveCfg(QJsonObject* obj);
    virtual const char* cfgName()const;
    virtual const char* getIndexId()const;
    virtual bool loadDefault();
    virtual int getCfgCount();
    virtual bool getCfgValue(int index,IndexCfgValue* cfgValue)const;
    virtual bool setCfgValue(int index,const IndexCfgValue* cfgValue);
protected:
    StockDmi dmi;
    StockFloatRange range;
signals:
};
}
#endif // DMIINDEX_H
