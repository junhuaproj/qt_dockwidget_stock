/*
 * Bias指标与绘图
*/
#ifndef BIASSTATISTICINDEX_H
#define BIASSTATISTICINDEX_H

#include <QObject>
#include "StatisticIndex.h"

//#define BIAS_LEN    4
namespace chart {
typedef struct StockBias
{
    int count;
    int* days;
    StockFloat** values;
}*PStockBias;

class BiasStatisticIndexCfg;

class BiasStatisticIndex : public QObject,public StatisticIndex,public ChartDraw
{
    Q_OBJECT
public:
    explicit BiasStatisticIndex(const char* indexId,const char* title,QObject *parent = nullptr);

    virtual ~BiasStatisticIndex();
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
protected:
    BiasStatisticIndexCfg* cfg;

    StockBias bias;
    //const StockDayData* stock;
    StockFloatRange range;
signals:
};
}
#endif // BIASSTATISTICINDEX_H
