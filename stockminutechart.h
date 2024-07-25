/**
 * 分时线
 * 可显示多天的
 * 如果是当日的折线图，可用覆盖空数据，价格0的不会绘制，根据时间推移，折线逐渐延长
*/
#ifndef STOCKMINUTECHART_H
#define STOCKMINUTECHART_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include "stockpriceitem.h"
#include "stockchartlabel.h"
#include <QList>
#include <QPointF>
#include <QPainter>
#include <QString>
#include "chartwidget.h"
#include "data/stockdayminute.h"

class StockListItem;
namespace minutechart
{


class StockMinuteChart : public ChartWidget
{
    Q_OBJECT
public:
    explicit StockMinuteChart(QWidget *parent = nullptr);
    virtual ~StockMinuteChart();

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintCharts(QPainter* painter,const QRectF& rcChart);
    virtual void paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc);

    //void paintEvent(QPaintEvent* event);
    void paintVolume(QPainter* painter);
    void paintPrice(QPainter* painter);
    void paintVReferLine(QPainter* painter,const QRectF& chartRect);

    void reset();
    /**
     * 增加一天的价格数据
    */
    void addDay(StockDayMinute* day);
    /**
     * 增加多天的价格数据
    */
    void addDays(QList<StockDayMinute*>& days);

    /**
     * 重新计算区间
    */
    void reCalcRange();
    /**
     * 重新计算价格区间
    */
    void reCalcPriceRange(float& min,float& max)const;
    /**
     * 重新计算成交量区间
    */
    void reCalcVolumeRange(int32_t& min,int32_t& max)const;
    int32_t getValueCount()const;
    void setValueCount(int32_t count);

    //void clearXChartLabels();
    //void addXChartLabel(int value,const QString& label);

    //    float getPreClosePrice() const;
    void setPrices(float preClosePrice,float open,float high,float low);

    //void setTitle(const QString& title);
    void setStockItem(const StockListItem* pStockItem);
    bool isStock()const;
    const StockListItem* getStockItem()const;
    //static void addOneDayXLabel(StockChartView* p,int day);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void setTitleVisible(bool bVisible);

    void setCurrStatus(bool bShow);

    //=SOHU_FILL_COUNT
    minutechart::StockDayMinute* newStockDayMinute(int fillCount);

    //void getChartRect(QRectF& rc);
    //bool pointToPos(const QRectF& rc,const QPoint& pt,MinChartPos* curPos);
    StockDayMinute* lastDay();

    /**
     * 数据是否有效
    */
    virtual bool isDataValid()const;
    /**
     * 索引是否有效
    */
    virtual bool isIndexValid(int index)const;
    //virtual int getViewCount()const;
    /**
     * 获取指定视图Y坐标的数值
    */
    virtual float getYValue(int view,float yPos);

    virtual bool getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const;
    /*
     * 得到指定分钟的信息：价格、成交量
    */
    virtual bool getItemDescribe(const ChartPos* pos,QString& out)const;
    /*
     * 得到当前指向价格的分层统计：高于该价格、低于该价格的成交信息
    */
    virtual bool getItemDescribeEx(const ChartPos* pos,QString& out)const;

    //void getCurPriceItemStatus(const ChartPos *pos,QString& out)const;
    //void getMinuteRange(const ChartPos* start,const ChartPos* end,QString& out);
    //void getMinuteDetail(const ChartPos* start,QString& out);
protected:
    //必须在计算区间后调用
    void reCalcRate();
protected:
    QList<StockDayMinute*> days;
    QList<const StockPriceItem*> prices;
    //QString title;
    bool bTitleVisible;
    const StockListItem* pStock;

    uint8_t pricePercent;

    /**
     * 最小价格
    */
    float priceMin;
    /**
     * 最大价格
    */
    float priceMax;
    /**
     * 最小成交量
    */
    int32_t volumeMin;
    /**
     * 最大成交量
    */
    int32_t volumeMax;

    /**
     * 总成交量
    */
    double totalVolume;
    /**
     * 总成交额
    */
    double totalFund;
    /**
     * 绘图的价格比例
    */
    float priceRate;
    /**
     * 成交量比例
    */
    float volumeRate;
    /**
     * 开价
    */
    float openPrice;
    /**
     * 昨收
    */
    float preClose;

    /*是否显示当前选中信息*/
    bool showCurrStatus;
};
}
#endif // STOCKMINUTECHART_H
