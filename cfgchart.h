
/*
 * K线图配置
*/

#ifndef CFGCHART_H
#define CFGCHART_H

#include <stdint.h>
#include "dayitem.h"

enum DataValueType{
    DataValue_Clr,
    DataValue_Text,
    DataValue_Int,
    DataValue_Flt,
    DataValue_Font,
};


#define ChartParam_Flag_Visible 0x01

namespace chart
{
typedef struct ChartParam
{
    uint8_t percent;
    uint8_t flag;
    uint16_t chartType;
}*PChartParam;

enum DayChartType
{
    DayChartType_Main=0,
    DayChartType_View2,
    DayChartType_View3,
    DayChartType_Count,
};

/*
 * 绘图区域，颜色
*/
struct ChartCfg
{
    uint32_t marginLeft;
    uint32_t marginTop;
    uint32_t marginRight;
    uint32_t marginBottom;
    float itemWidth;
    float itemSep;
    uint32_t bg;
    uint32_t fore;//
    int32_t fontSize;
    char fontFace[64];
};


typedef struct DayChartParam
{
    uint8_t mode;//0:dark,1:light


    uint32_t grid;
    uint32_t referline;//参考线

    uint32_t up;//上涨色
    uint32_t down;//下跌

    uint8_t viewTopPadding;
    //uint8_t viewDateHeight;

    uint32_t curCrossColor;
    float curCrossWidth;

}*PDayChartParam;


class StatisticIndex;
/*
 * 参考线
*/
enum ReferLine_flag
{
    ReferLine_Horizontal=1,//value
    ReferLine_Vertical=2,//index
};

typedef struct ReferLine
{
    union
    {
        float value;//数值，用于横线
        int index;//位置,用于竖线
    };
    uint32_t color;
    float lineWidth;
    uint32_t flag;
}*PReferLine;

struct ReferLines
{
    PReferLine lines;
    uint32_t count;
};

PReferLine getNewReferLine(ReferLines* pLines);
typedef struct ChartViewCfg
{
    uint8_t percent;
    uint8_t flag;
    //uint16_t chartType;
    StatisticIndex* pIndex;
    ReferLines lines;
}*PChartViewCfg;

inline bool isViewVisible(const ChartViewCfg* cfg,DayChartType chartTYpe)
{
    return (cfg[chartTYpe].flag&ChartParam_Flag_Visible);
}

}

#endif // CFGCHART_H
