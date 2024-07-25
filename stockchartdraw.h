/*
 * K线图绘制的一些相关类
*/
#ifndef STOCKCHARTDRAW_H
#define STOCKCHARTDRAW_H

#include <QList>
#include <QString>
#include <QColor>
#include <QPainter>
#include <QRectF>
#include "cfgchart.h"
namespace chart {

class StockDayChart;
/**
 * 绘图工具，参数
*/
class PaintParam
{
public:
    PaintParam(const StockDayChart* chart);
    //PaintParam(const PaintParam& p);
    virtual ~PaintParam();
    void initColorType(double* closes,double* opens);
    void freeColorType();
    float getValueHeight(double v)const;

    float getValueHeight(int index)const;

    float getValuePos(double v)const;
    float getValuePos(int index)const;
    void drawHorzionalLine(qreal value,const QColor& clr);

    void drawLineAndValue(qreal y,qreal x,qreal width,int textHeight);
    void drawLines(const PReferLine lines,int count);

    bool isIncrease(int dataIndex)const;
    QPainter* p;
    QRectF rc;
    const double* v;//数值

    int start;

    double zero_v;//图表原点的Y值
    qreal zero_y;//图表原点的Y坐标
    double rate;//比例


    int count;//个数
    uint32_t clrUp;
    uint32_t clrDown;

    const DayChartParam* pDrawParam;
    const ChartCfg* pChartCfg;

    inline bool clrType(int index)
    {
        return colorType[index];
    }
    const bool* getClrTypes()const;
protected:
    bool* colorType;
};

/**
 * 坐标点的数值
*/
class ColorString
{
public:
    ColorString(const QString& t,uint32_t c):text(t),color(c){}
    QString text;
    uint32_t color;//显示的颜色
};

void clearColorStrings(QList<ColorString*>& colors);
//绘图的类型
enum DataChartType
{
    Chart_User,//需要接口实现绘图过程，每个数值会调用一次
    Chart_Line,//折线图，接口实现数据获取，由上层实现绘图
    Chart_Pillar,//柱型图,接口实现数据获取，由上层实现绘图
    Chart_Pillar2,//柱型图,接口实现数据获取，由上层实现绘图
};
typedef struct ChartLine{
    const double* v;//绘图数值，接口填充，并使用start偏移
    int start;//上层调用前填充
    int count;//上层调用前填充
    uint32_t color;
    float width;
}*PChartLine;

typedef struct ChartPillar{
    const double* v;//绘图数值，接口填充，并使用start偏移
    int start;//上层调用前填充
    int count;//上层调用前填充
    uint32_t* colors;//每个柱可以有不同的颜色，为NULL时使用涨跌色，initChartPillar初始化
    float width;//柱形宽度，0为1像素
    double zero_v;
}*PChartPillar;

void initChartPillar(ChartPillar* pillar,int start,int count,bool useColor);
void freeChartPillar(ChartPillar* pillar);

struct IChartDraw
{
    virtual int chartCount()=0;
    virtual DataChartType chartType(int index)=0;
    virtual double getZeroY()=0;//得到Y轴起点坐标
    virtual uint8_t paddingTop()const=0;
    virtual uint8_t paddingBottom()const=0;
    virtual bool lineValue(int index,PChartLine pLine)=0;//折线图
    virtual bool isPillarSysColor(int index)=0;//柱状图使用涨跌色
    virtual bool pillarColor(int index,PChartPillar pillar)=0;//得到柱状图颜色
    virtual void chartDraw(int chartIndex,int dataIndex,QRectF& rc,const PaintParam* p)=0;
};

class ChartDraw:public IChartDraw
{
public:
    ChartDraw(){}
    virtual uint8_t paddingTop()const
    {
        return 0;
    }
    virtual uint8_t paddingBottom()const
    {
        return 0;
    }
};

}
#endif // STOCKCHARTDRAW_H
