/*
 * MACD指标
*/
#ifndef MACDINDEX_H
#define MACDINDEX_H

#include <QObject>
#include "stockdaydata.h"

#include "StatisticIndex.h"
#include "stockmacdcfg.h"
namespace chart {
typedef struct StockMacd
{
    StockFloat* diff;
    StockFloat* dea;
    StockFloat* macd;
}*PStockMacd;

class MacdIndex : public QObject,public StatisticIndex,public ChartDraw
{
    Q_OBJECT
public:
    explicit MacdIndex(const char* indexId,const char* title,QObject *parent = nullptr);

    virtual ~MacdIndex();
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
    StockMacd macd;
    StockFloatRange range;
    StockMacdCfg* cfg;
signals:
};
}
#endif // MACDINDEX_H
