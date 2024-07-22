/*
 * KDJ指标
*/
#ifndef KDJINDEX_H
#define KDJINDEX_H

#include <QObject>
#include "StatisticIndex.h"
#include "kdjindexcfg.h"
namespace chart{
struct StockKdj
{
    double* k;
    double* d;
    double* j;
};
class KdjIndex : public QObject,public StatisticIndex,public ChartDraw
{
    //Q_OBJECT
public:
    explicit KdjIndex(const char* indexId,const char* title,QObject *parent = nullptr);
    virtual ~KdjIndex();

    virtual void init(const StockDayData* stock);
    virtual void deInit();

    virtual void reCalcIndex();
    virtual void reCalcRange(int start,int count);
    virtual const StockFloatRange* getRange();
    virtual void getItemDescription(int index,QList<ColorString*>& strings);
    virtual ChartDraw* getChartDraw();
    virtual StatisticIndexCfg* getCfg();
    ////
    virtual int chartCount();
    virtual DataChartType chartType(int index);
    virtual double getZeroY();//得到Y轴起点坐标

    virtual bool lineValue(int index,PChartLine pLine);//折线图
    virtual bool isPillarSysColor(int index);//柱状图使用涨跌色
    virtual bool pillarColor(int index,PChartPillar pillar);//得到柱状图颜色
    virtual void chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p);
protected:
    StockKdj kdj;
    KdjIndexCfg* cfg;
    StockFloatRange range;
signals:
};
}
#endif // KDJINDEX_H
