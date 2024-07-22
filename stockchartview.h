/**
 * 分时线
 */
#ifndef STOCKCHARTVIEW_H
#define STOCKCHARTVIEW_H

#include <QWidget>
#include <QList>
#include <QPaintEvent>

#include "stockpriceitem.h"
#include "stockchartlabel.h"

#include "stocklistitem.h"

class StockChartView : public QWidget
{
    Q_OBJECT
public:
    explicit StockChartView(QWidget *parent = nullptr);

    ~StockChartView();
    void paintEvent(QPaintEvent* event);

    void clearPrices();
    void addStockPrice(StockPriceItem* item);

    void addStockPrice(QString hhmm,float price,float avgPrice,int32_t volume,int32_t fund);

    void getPriceRange(float& min,float& max)const;
    void getVolumeRange(int32_t& min,int32_t& max)const;
    int32_t getValueCount()const;
    void setValueCount(int32_t count);

    void clearXChartLabels();
    void addXChartLabel(int value,const QString& label);

//    float getPreClosePrice() const;
    void setPrices(float preClosePrice,float open,float high,float low);

    void setTitle(const QString& title);
    void setStockItem(StockListItem* pStockItem);
    bool hasStock()const;
    const StockListItem* getStockItem()const;
    static void addOneDayXLabel(StockChartView* p,int day);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void setTitleVisible(bool bVisible);
    void getCurPriceItemStatus(const StockPriceItem* pPrice,QString& out);
    void setCurrStatus(bool bShow);
protected:

    bool bTitleVisible;

    StockListItem stockItem;
    QString title;
    QList<StockPriceItem*> timePrices;
    QList<StockChartLabel*> xChartLabels;

    float priceMin;
    float priceMax;
    int32_t valueCount;
    int32_t volumeMin;
    int32_t volumeMax;
    int32_t chartLeft;
    int32_t chartTop;
    int32_t chartBottom;
    int32_t chartRight;

    int32_t curIndex;
    int32_t curMouseY;
    float openPrice;
    float lowPrice;
    float highPrice;
    float preClosePrice;

    bool showCurrStatus;

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    void paintPrice(QPainter* painter,const QRectF& rectChart,qreal itemWidth);
    void paintVolume(QPainter* painter,const QRectF& rectChart,qreal itemWidth);
signals:
    void onCurrTime(StockPriceItem* pPrice);
};

#endif // STOCKCHARTVIEW_H
