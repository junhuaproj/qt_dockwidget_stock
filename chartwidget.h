/*
 * K线图，折线图的基类
*/
#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include "cfgchart.h"

struct ChartPos
{
    int32_t viewIdx;
    int xIndex;
    double yValue;
    float x;
    float y;
};
void resetChartPos(ChartPos* pos);
class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(int viewCnt,QWidget *parent = nullptr);
    virtual ~ChartWidget();

    virtual void mouseMoveEvent(QMouseEvent* event)override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent* event);
    virtual void paintCharts(QPainter* painter,const QRectF& rcChart)=0;
    virtual void paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc)=0;
    virtual void paintSelectRange(QPainter* painter);

    bool PointToRealValue(ChartPos* pos);
    void getChartRect(QRectF& rc);

    virtual bool isDataValid()const=0;
    virtual bool isIndexValid(int index)const=0;
    //virtual int getValueCount()const=0;
    virtual float getYValue(int view,float yPos)=0;

    virtual bool getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const=0;
    virtual bool getItemDescribe(const ChartPos* pos,QString& out)const=0;
    virtual bool getItemDescribeEx(const ChartPos* pos,QString& out)const=0;
    void setTitle(const QString& title);

    const chart::ChartCfg* getChartCfg() const;
    void setChartCfg(const chart::ChartCfg &newChartCfg);

    bool getValidIndex(const ChartPos* start,const ChartPos* end,int& startIdx,int& endIdx)const;

    void resetSelect();
protected:
    QRectF* rcViews;
    int viewCount;
    chart::ChartCfg chartCfg;
    QString title;

    ChartPos curPos;
    ChartPos startPos;
    ChartPos stopPos;
signals:
    void selectItem(ChartWidget*,ChartPos*);
    void selectRange(ChartWidget*,ChartPos*,ChartPos*,bool changing);
};

#endif // CHARTWIDGET_H
