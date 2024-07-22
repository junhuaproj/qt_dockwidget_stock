#ifndef STOCKDAYCHARTVIEW_H
#define STOCKDAYCHARTVIEW_H

#include <QWidget>
#include <QPainter>
#include <QContextMenuEvent>
#include <vector>
//#include "dayitem.h"
#include "cfgchart.h"
#include "stockday.h"
//using namespace std;

#include "stockchartdraw.h"

class PaintPillarParam:public PaintParam
{
public:
    PaintPillarParam();
    PaintPillarParam(const PaintParam& p):PaintParam(p)
    {}

    QColor clrInc;//增长类颜色
    QColor clrDec;//降低类的颜色

};

class PaintLineParam:public PaintParam
{
public:
    PaintLineParam();
    PaintLineParam(const PaintParam& p):PaintParam(p)
    {}

    float lineWidth;
    QColor clr;

};


class StockDayChartView : public QWidget
{
    Q_OBJECT
public:
    explicit StockDayChartView(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent* event);

    void paintCross(QPainter* painter,const QRectF& rc,const QFont& font);

    void setStockDay(StockDay* stockDay);
    void dayItemChanged();
    //void clacDrawParam();
    //void setView2Type(DayChartView2Type view2Type);
    void setStartIndex(int index);
    void setFlag(uint32_t flag);
    uint32_t getFlag();
    uint8_t mainHeight();
    uint8_t view2Height();
    uint8_t view3Height();
    void setMainHeight(uint8_t percent);
    void setView2Height(uint8_t percent);
    void setView3Height(uint8_t percent);
    void setCfg(const PDayChartParam pCfg);
    const DayChartParam* getCfg()const;

    void sizeChanged();
    void setCurAbsoultIndex(int curIndex);
    void setTitle(const QString& title);

    void keyPressEvent(QKeyEvent *event) override;

    void setRangeUpdate(bool update);

    const DayChartDrawParam* getDrawParam()const;
    void setDrawParam(const DayChartDrawParam*);
protected:
    void paintChart(uint16_t type,PaintParam& param);

    void painterValue(QPainter* p,const QRectF& rc,const QList<ColorString*>& colors,const QFont& font);
    void paintMainK(const PaintParam& p);
    void paintMainK1(PaintLineParam& p);

    void paintVolume(const PaintParam& p);
    void paintKDJ(const PaintParam& p);
    void paintMACD(int index,const PaintParam& p);

    void paintPillar2(const PaintPillarParam& p);

    void paintLine(const PaintLineParam& p);
    void paintPillar(const PaintPillarParam& p);


protected:
    inline float barSep()
    {
        return (float)m_drawParam.itemSeperate/10;
    }
    inline float barWidth()
    {
        return (float)m_drawParam.itemWidth/10;
    }
    inline float barFullWidth()
    {
        return ((float)m_drawParam.itemSeperate+m_drawParam.itemWidth)/10;
    }
    QString m_title;
    //CfgChart m_cfg;
    DayChartParam m_cfg;
    DayChartDrawParam m_drawParam;

    StockDay* stockDay;

    //int m_chartTopPadding;
    //int m_chartBottomPadding;
    //int m_paddingLeft;
    //int m_paddingRight;
    //int m_paddingTop;
    //int m_paddingBottom;

    //int m_view3PaddingTop;
    //int m_view2PaddingTop;
    //int m_mainPaddingBottom;
    //int m_barWidth;
    //int m_barSep;

    QRectF m_rcMain;
    QRectF m_rc1;
    QRectF m_rc2;
    ///macd2

    int m_startIndex;
    int m_curIndex;
    int m_curY;
    int m_countItem;//当前可以显示的最多个数
    int m_countValid;//当前有效的个数，当数据个数小于能显示的个数时会小于m_countItem
protected slots:
    void onActionBuy(bool);
    void onActionSell(bool);
    void onActionMin(bool);
private:
    bool rangeUpdated;
protected:
    void getChartRect(QRectF& rc);

    void paintChart(uint16_t type,QPainter* painter, const QRectF& rc);
    void getChartIndexString(uint16_t type,int index,QString& out);

    void getChartIndexString(uint16_t type,int index,QList<ColorString*>& out);

    void onCurrChanged();
    void contextMenuEvent(QContextMenuEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
signals:

    //void selectDay(PStockDayItem p);
    void selectDay(int index);
    void buy(int index);
    void sell(int index);
    void minute(int index);

    void viewDataChanged(StockDayChartView*,int start,int count);
};

#endif // STOCKDAYCHARTVIEW_H
