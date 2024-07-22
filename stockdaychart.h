/*
 * 日线图
*/
#ifndef STOCKDAYCHART_H
#define STOCKDAYCHART_H

#include <QWidget>

#include <QPainter>
#include <QContextMenuEvent>
#include <vector>

#include "cfgchart.h"
#include "stockday.h"
#include "stockchartdraw.h"
#include "StatisticIndex.h"
#include "chartwidget.h"

namespace data{
class DataLoader;
}

namespace chart {

// void resetDayChartPos(DayChartPos* pos);
class StockDayChart : public ChartWidget
{
    Q_OBJECT
public:
    explicit StockDayChart(QWidget *parent = nullptr);
    virtual ~StockDayChart();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
public:
    //获得所有可用的指数方式
    QList<StatisticIndex*>* getIndexes();

    void loadCfg(data::DataLoader* p);
    void execThemeDlg(data::DataLoader* p);
    //清除所有计算得到的指数数据及内存
    void resetIndexData();

    void setViewIndex(int index,StatisticIndex* p);
    //void setTitle(const QString& title);
    void setStartIndex(int index);
    int getStartIndex()const;
    //设置当前选中的日期序号
    void setCurIndex(int index);

    //获得正在使用的指数
    void getValidateIndex(QList<StatisticIndex*>& indexes);
    void dayItemChanged();
    void setStockDay(StockDay* stockDay);
    //重新计算指数
    void reCalcIndex();

    //获得视图设置
    const ChartViewCfg* getViewCfg()const;
    //设置视图设置
    void setViewCfg(const ChartViewCfg* cfg);

    //获取基础绘图参数
    const DayChartParam* getDrawParam()const;
    //清除所有参考线
    void clearReferLine();

    int getValidCount()const;
    //void getRangeStatus(ChartPos* start,ChartPos* end,QString& out);

    virtual bool isDataValid()const;
    virtual bool isIndexValid(int index)const;
    //virtual int getValueCount()const;
    virtual float getYValue(int view,float yPos);

    virtual bool getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const;
    virtual bool getItemDescribe(const ChartPos* pos,QString& out)const;
    virtual bool getItemDescribeEx(const ChartPos* pos,QString& out)const;
public slots:
    void showReferLineDlg(bool);

    void onReferLineUpdate(DayChartType chartType);
protected:

    void paintCurStatus(QPainter* painter,const QRectF& rc,const QFont& font);

    void sizeChanged();
    //void getChartRect(QRectF& rc);

    inline float barFullWidth()
    {
        return chartCfg.itemSep+chartCfg.itemWidth;
    }

    void loadDefault();
    void getAllCfg(QList<StatisticIndexCfg*>& cfgs);
    void loadDayChartParam(ChartViewCfg* cfg,int len);

    bool isViewVisible(DayChartType chartType)const
    {
        return chart::isViewVisible(m_view,chartType);
    }
    //bool isDataValid();

    //void initPaintParam(PaintParam* p);
    virtual void paintCharts(QPainter* painter,const QRectF& rcChart);
    virtual void paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc);
    void painterCurStatus(QPainter* p,const QRectF& rc,const QList<ColorString*>& colors,const QFont& font);
    void painterChart(StatisticIndex* pIndex,ReferLines* lines,PaintParam* param);
    void painterUser(int chartIndex,ChartDraw* pDraw,PaintParam* param);
    void paintLine(PaintParam* param,PChartLine pLine);
    void paintPillar(PaintParam* param,PChartPillar pPillar);
    void paintYLabel(PaintParam* param);
    //void paintRange(QPainter* p);

    void onViewCfgChanged();
protected:
    //视图设置，高度，指数
    //QRectF m_rcs[DayChartType_Count];
    ///macd2

    //视图显示第一个的绝对索引
    int m_startIndex;
    //当前选中的索引(相对位置)
    //int m_curIndex;
    //当前选中的Y坐标
    //int m_curY;
    int m_countItem;//当前可以显示的最多个数
    int m_countValid;//当前有效的个数，当数据个数小于能显示的个数时会小于m_countItem

    //QString m_title;
    ChartViewCfg m_view[DayChartType_Count];
    DayChartParam m_drawParam;


    //指向原始数据的指针
    StockDay* m_stockDay;

    //所有可用的指数
    QList<StatisticIndex*> indexes;

signals:
    void selectDay(StockDayChart*,int index);
    void viewDataChanged(StockDayChart*,int start,int count);
};
}
#endif // STOCKDAYCHART_H
