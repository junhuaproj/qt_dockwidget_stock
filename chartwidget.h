/*
 * K线图，折线图的基类
*/
#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include "cfgchart.h"
//图表中断的位置
struct ChartPos
{
    int32_t viewIdx;//视图序号,一个图表里有多个视图
    int xIndex;//X轴数据的位置
    double yValue;//Y轴方向对应的数值
    float x;//相对于窗口的X坐标
    float y;//相对于窗口的Y坐标
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
    //绘制图表区
    virtual void paintCharts(QPainter* painter,const QRectF& rcChart)=0;
    //绘制边框区，坐标轴上的标注、标题等
    virtual void paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc)=0;
    //绘制选中的区域
    virtual void paintSelectRange(QPainter* painter);

    //获得坐标位置对应的X序号和Y对应的值
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

    //获得有效的X轴索引区间
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
