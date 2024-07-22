/**分时线*/
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
    void addDay(StockDayMinute* day);
    void addDays(QList<StockDayMinute*>& days);

    void reCalcRange();
    void reCalcPriceRange(float& min,float& max)const;
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

    virtual bool isDataValid()const;
    virtual bool isIndexValid(int index)const;
    //virtual int getViewCount()const;
    virtual float getYValue(int view,float yPos);

    virtual bool getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const;
    virtual bool getItemDescribe(const ChartPos* pos,QString& out)const;
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

    float priceMin;
    float priceMax;
    int32_t volumeMin;
    int32_t volumeMax;

    double totalVolume;
    double totalFund;
    float priceRate;
    float volumeRate;

    float openPrice;
    float preClose;

    bool showCurrStatus;
};
}
#endif // STOCKMINUTECHART_H
