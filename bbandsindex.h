/*
 * band指标计算与绘图
*/
#ifndef BBANDSINDEX_H
#define BBANDSINDEX_H

#include <QObject>
#include "StatisticIndex.h"

namespace chart {
struct StockBbands
{
    double* upper;
    double* middle;
    double* lower;
};

class BbandsIndex : public QObject,public StatisticIndex,public ChartDraw,public StatisticIndexCfg
{
    Q_OBJECT
public:
    explicit BbandsIndex(const char* indexId,const char* title,QObject *parent = nullptr);

    virtual ~BbandsIndex();
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
    StockBbands bbands;
    StockFloatRange range;

    uint32_t upperColor;
    uint32_t middleColor;
    uint32_t lowColor;
    float lineWidth;

    uint32_t period;
    float devUp;
    float devDown;
signals:
};
}
#endif // BBANDSINDEX_H
