#include "stockchartdraw.h"
#include "stockdaychart.h"

namespace chart {
PaintParam::PaintParam(const StockDayChart* chart)
    :colorType(NULL),pDrawParam(chart->getDrawParam())
    ,count(chart->getValidCount())
    ,start(chart->getStartIndex())
    ,pChartCfg(chart->getChartCfg())
{
}

PaintParam::~PaintParam()
{
    if(colorType)
        free(colorType);
}
void PaintParam::freeColorType()
{
    if(colorType)
    {
        free(colorType);
        colorType=NULL;
    }
}
const bool* PaintParam::getClrTypes()const
{
    return colorType;
}
void PaintParam::initColorType(double* inCloses,double* inOpens)
{
    freeColorType();
    colorType=(bool*)malloc(sizeof(bool)*count);

    const double* closes=inCloses+start;
    const double* opens=inOpens+start;

    for(int index=0;index<count;index++)
    {
        if(closes[index]>opens[index])
            colorType[index]=true;//closes[index]>=opens[index];
        else if(closes[index]==opens[index]&&index>0&&opens[index]>closes[index-1])
            colorType[index]=true;
        else
            colorType[index]=false;
    }
}

bool PaintParam::isIncrease(int dataIndex)const
{
    return colorType[dataIndex-start];
}
float PaintParam::getValueHeight(double v)const
{
    return (v-zero_v)*rate;
}
float PaintParam::getValueHeight(int index)const
{
    return getValueHeight(v[index]);
}

float PaintParam::getValuePos(double v)const
{
    return zero_y-getValueHeight(v);
}

float PaintParam::getValuePos(int index)const
{
    return zero_y-getValueHeight(index);
}
void PaintParam::drawHorzionalLine(qreal value,const QColor& clr)
{
    qreal y=zero_y-(value-zero_v)*rate;
    if(y>rc.y()&&y<=rc.bottom())
    {
        p->setPen(clr);
        p->drawLine(rc.left(),y,rc.right(),y);
    }
}
void PaintParam::drawLineAndValue(qreal y,qreal x,qreal width,int textHeight)
{
    if(y>rc.y()&&y<rc.bottom())
    {
        p->setPen(pDrawParam->referline);
        float value=(zero_y-y)/rate+zero_v;
        QString s=QString("%1").arg(value,0,'f',2);
        p->drawLine(rc.x(),y,rc.right(),y);
        QRectF rcText(x,y-textHeight/2,width,y+textHeight);
        //rcText.setY(m_curY-10);
        //rcText.setBottom(rcText.y()+20);
        p->setPen(pChartCfg->fore);
        p->drawText(rcText,s);
    }
}

void PaintParam::drawLines(const PReferLine lines,int count)
{
    for(uint32_t i=0;i<count;i++)
    {
        const PReferLine pLine=lines+i;
        if(pLine->flag&ReferLine_Horizontal)
        {
            qreal y=zero_y-(pLine->value-zero_v)*rate;
            if(y>rc.y()&&y<=rc.bottom())
            {
                QPen pen(pLine->color);
                pen.setWidthF(pLine->lineWidth);
                p->setPen(pen);

                p->drawLine(rc.left(),y,rc.right(),y);
            }
        }
        else if(pLine->flag&ReferLine_Vertical)
        {
            qreal x=rc.x()+pChartCfg->itemSep+pChartCfg->itemWidth/2+pLine->index*pChartCfg->itemWidth;
            QPen pen(pLine->color);
            pen.setWidthF(pLine->lineWidth);
            p->setPen(pen);
            p->drawLine(x,rc.top(),x,rc.bottom());
        }
    }
}

void clearColorStrings(QList<ColorString*>& colors)
{
    while(!colors.isEmpty())
    {
        delete colors.back();
        colors.pop_back();
    }
}
void initChartPillar(ChartPillar* pillar,int start,int count,bool useColor)
{
    memset(pillar,0,sizeof(ChartPillar));
    pillar->start=start;
    pillar->count=count;
    if(!useColor)
    {
        pillar->colors=(uint32_t*)malloc(sizeof(uint32_t)*pillar->count);
        memset(pillar->colors,0,sizeof(uint32_t)*pillar->count);
    }
}
void freeChartPillar(ChartPillar* pillar)
{
    if(pillar->colors)
    {
        free(pillar->colors);
        pillar->colors=NULL;
    }
}
}
