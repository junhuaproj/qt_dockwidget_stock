#include "chartwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include "appconfig.h"
ChartWidget::ChartWidget(int viewCnt,QWidget *parent)
    : QWidget(parent)
{
    memset(&chartCfg,0,sizeof(chart::ChartCfg));
    resetSelect();
    chartCfg.bg=0x333333;
    chartCfg.fore=0xffffff;
    chartCfg.fontSize=9;
    QString chartFont=appConfig.getChartFont();
    strcpy(chartCfg.fontFace,chartFont.toStdString().c_str());// "宋体");
    this->viewCount=viewCnt;
    this->rcViews=new QRectF[viewCnt]();
}

ChartWidget::~ChartWidget()
{
    delete []rcViews;
}

void ChartWidget::resetSelect()
{
    resetChartPos(&curPos);
    resetChartPos(&startPos);
    resetChartPos(&stopPos);
}
void ChartWidget::setTitle(const QString& title)
{
    this->title=title;
}

const chart::ChartCfg* ChartWidget::getChartCfg() const
{
    return &chartCfg;
}

bool ChartWidget::getValidIndex(const ChartPos* start,const ChartPos* end,int& startIdx,int& endIdx)const
{
    if(start->xIndex==-1||end->xIndex==-1)return false;
    if(start->xIndex>end->xIndex)
    {
        startIdx=end->xIndex;
        endIdx=start->xIndex;
    }
    else
    {
        startIdx=start->xIndex;
        endIdx=end->xIndex;
    }
    return true;
}
void ChartWidget::setChartCfg(const chart::ChartCfg &newChartCfg)
{
    chartCfg = newChartCfg;
}
void ChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(!isDataValid())return ;
    resetChartPos(&curPos);
    curPos.x=event->pos().x();
    curPos.y=event->pos().y();
    if(PointToRealValue(&curPos))//event->pos(),&index,&value))
    {
        //m_curIndex=index;
        //m_curY=event->pos().y();
        if(event->button()==Qt::MouseButton::LeftButton)
        {
            if(startPos.viewIdx!=-1)
                memcpy(&startPos,&curPos,sizeof(ChartPos));
            emit selectRange(this,&startPos,&curPos,true);
        }
        else
            emit selectItem(this,&curPos);
        this->repaint();
    }
    else
    {
        if(event->button()==Qt::MouseButton::LeftButton)
        {
            if(startPos.xIndex!=-1)
            {
                resetChartPos(&stopPos);//如果没有开始点，结束点也没有用
            }
            //其他情况不处理，因为可能鼠标落在间隔上
        }
        else
        {
            if(startPos.viewIdx!=-1)//发送信号让上层应用更新
                emit selectRange(this,NULL,NULL,false);
        }
        resetChartPos(&curPos);

        this->repaint();
        //m_curIndex=-1;
        //m_curY=-1;
    }
}
void ChartWidget::mousePressEvent(QMouseEvent* event)
{
    Qt::MouseButton btn=event->button();
    if(btn!=Qt::MouseButton::LeftButton)
        return;
    if(!isDataValid())return ;
    resetChartPos(&startPos);
    resetChartPos(&stopPos);
    startPos.x=event->pos().x();
    startPos.y=event->pos().y();
    if(PointToRealValue(&startPos))//event->pos(),&index,&value))
    {
        memcpy(&curPos,&startPos,sizeof(ChartPos));
        qDebug()<<"press index:"<<startPos.xIndex<<"value:"<<startPos.yValue;
        emit selectItem(this,&startPos);
    }
    else
    {
        resetChartPos(&startPos);
    }
}
void ChartWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(!isDataValid())return ;
    //int index;
    //double value;
    resetChartPos(&stopPos);
    stopPos.x=event->pos().x();
    stopPos.y=event->pos().y();
    if(PointToRealValue(&stopPos))//event->pos(),&index,&value))
    {
        qDebug()<<"release index:"<<stopPos.xIndex<<"value:"<<stopPos.yValue;
        emit selectRange(this,&startPos,&stopPos,false);
    }
}

void ChartWidget::getChartRect(QRectF& rc)
{
    rc=rect();
    rc.setLeft(chartCfg.marginLeft);
    rc.setTop(chartCfg.marginTop);
    rc.setBottom(rc.bottom()-chartCfg.marginBottom);
    rc.setRight(rc.right()-chartCfg.marginRight);
    //rc.adjust(chartCfg.marginLeft,chartCfg.marginTop,
    //          -chartCfg.marginRight,-chartCfg.marginBottom);
}

bool ChartWidget::PointToRealValue(ChartPos* pos)
{
    QRectF chartRect;//=rect();
    getChartRect(chartRect);
    if(chartRect.contains(pos->x,pos->y))
    {
        bool bOK=false;
        int curIndex=(pos->x-chartRect.left()-chartCfg.itemSep)/(chartCfg.itemSep+chartCfg.itemWidth);
        if(chartCfg.itemSep!=0)
        {//防止落在间隔上
            float left=(chartRect.left()+chartCfg.itemSep)+(chartCfg.itemSep+chartCfg.itemWidth)*curIndex;
            if(pos->x>=left&&pos->x<=left+chartCfg.itemWidth)
            {

            }
            else
            {
                curIndex=-1;
            }
        }
        if(isIndexValid(curIndex))
        {
            pos->xIndex=curIndex;
            for(int i=0;i<viewCount;i++)
            {
                if(rcViews[i].contains(pos->x,pos->y))
                {
                    bOK=true;
                    pos->yValue=getYValue(i,pos->y);
                    pos->viewIdx=i;
                    break;
                }
            }
        }
        return bOK;
    }
    return false;
}

void ChartWidget::paintSelectRange(QPainter* painter)
{
    if(startPos.xIndex!=-1)
    {
        ChartPos* endPos=&stopPos;
        if(endPos->xIndex==-1)
            endPos=&curPos;
        else
        {
            //qDebug()<<"b";
        }
        if(endPos->xIndex==-1)return ;
        QRect rcSelect;
        if(startPos.x>endPos->x)
        {
            rcSelect.setX(startPos.x);
            rcSelect.setRight(endPos->x);
        }
        else
        {
            rcSelect.setX(endPos->x);
            rcSelect.setRight(startPos.x);
        }

        if(startPos.y>endPos->y)
        {
            rcSelect.setY(startPos.y);
            rcSelect.setBottom(endPos->y);
        }
        else
        {
            rcSelect.setY(endPos->y);
            rcSelect.setBottom(startPos.y);
        }
        painter->fillRect(rcSelect,QColor(0x9f,0x9f,0x9f,0x7f));

    }
}
void ChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QRectF rcClient;
    QRect rc=rect();
    painter.save();
    //painter.setRenderHint(QPainter::TextAntialiasing);
    painter.fillRect(rc,chartCfg.bg);
    getChartRect(rcClient);
    paintCharts(&painter,rcClient);

    paintMargin(&painter,rcClient,rc);
    paintSelectRange(&painter);
    painter.restore();
}

void resetChartPos(ChartPos* pos)
{
    pos->viewIdx=-1;
    pos->xIndex=-1;
    pos->yValue=-1;
}
