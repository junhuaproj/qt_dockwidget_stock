#include "stockdaychart.h"
#include "stockbasedata.h"
#include "biasstatisticindex.h"
#include "macdindex.h"
#include "volumeindex.h"
#include "kdjindex.h"
#include "daystockchartcfg.h"
#include "bbandsindex.h"
#include "dmiindex.h"
#include "chartthemedlg.h"
#include "dataloader.h"
#include "daychartreferlinedlg.h"
namespace chart {
StockDayChart::StockDayChart(QWidget *parent)
    : ChartWidget(DayChartType_Count,parent),m_startIndex(0)
    //,m_curIndex(-1),m_curY(-1)
    ,m_countItem(0),m_countValid(0),
    m_stockDay(NULL)
{
    this->setFocusPolicy(Qt::ClickFocus);

    indexes.push_back(new StockBaseData("base","base"));
    indexes.push_back(new BiasStatisticIndex("bias","bias"));
    indexes.push_back(new MacdIndex("macd","macd"));
    indexes.push_back(new VolumeIndex("volume","volume"));
    indexes.push_back(new KdjIndex("kdj","kdj"));
    indexes.push_back(new BbandsIndex("bband","boll"));

    indexes.push_back(new DmiIndex("dmi","dmi"));

    loadDefault();
    memset(m_view,0,sizeof(ChartViewCfg)*DayChartType_Count);
    loadDayChartParam(m_view,DayChartType_Count);
}

StockDayChart::~StockDayChart()
{
    clearReferLine();
    while(!indexes.isEmpty())
    {
        delete indexes.back();
        indexes.pop_back();
    }
}

void StockDayChart::clearReferLine()
{
    for(int i=0;i<DayChartType_Count;i++)
    {
        if(m_view[i].lines.lines)
        {
            free(m_view[i].lines.lines);
            memset(&m_view[i].lines,0,sizeof(ReferLines));
        }
    }
}

void StockDayChart::onReferLineUpdate(DayChartType chartType)
{
    QRectF* rc=rcViews+chartType;
    repaint(rc->left(),rc->top(),rc->width(),rc->height());
}
float StockDayChart::getYValue(int view,float y)
{
    //for(int i=0;i<(int)DayChartType_Count;i++)
    //{
        QRectF rc=rcViews[view];
        ChartDraw* pDraw= m_view[view].pIndex->getChartDraw();
        rc.adjust(0,pDraw->paddingTop()+m_drawParam.viewTopPadding,0,-pDraw->paddingBottom());
        const StockFloatRange* range=m_view[view].pIndex->getRange();
        double diff = range->high - range->low;

        double rate = rc.height() / diff;
        double zero_v=pDraw->getZeroY();
        double zero_y=rc.bottom()-(rate*(zero_v-range->low));
        return zero_v+(zero_y-y)/rate;
    //}
}

void StockDayChart::showReferLineDlg(bool)
{
    DayChartReferLineDlg * dlg=new DayChartReferLineDlg(//m_curIndex,value,viewIndex,
        curPos.xIndex,curPos.yValue,(DayChartType)curPos.viewIdx,
        m_view,DayChartType_Count,this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    connect(dlg,SIGNAL(referUpdated(DayChartType)),this,SLOT(onReferLineUpdate(DayChartType)));
    dlg->show();
}

void StockDayChart::loadCfg(data::DataLoader* p)
{
    DayStockChartCfg chartChart(&m_drawParam,&chartCfg);
    QList<StatisticIndexCfg*> cfgs;
    cfgs.append(&chartChart);
    getAllCfg(cfgs);
    if(p->loadDayChartDrawParam(cfgs))
    {

    }
}

void StockDayChart::execThemeDlg(data::DataLoader* p)
{
    DayStockChartCfg chartChart(&m_drawParam,&chartCfg);
    QList<StatisticIndexCfg*> cfgs;
    cfgs.append(&chartChart);
    getAllCfg(cfgs);

    ChartThemeDlg* dlg=new ChartThemeDlg(cfgs,this);
    if(dlg->exec()==QDialog::Accepted)
    {
        p->saveDayChartDrawParam(cfgs);
    }
    delete dlg;
}

QList<StatisticIndex*>* StockDayChart::getIndexes()
{
    return &indexes;
}

void StockDayChart::loadDefault()
{
    DayStockChartCfg chartChart(&m_drawParam,&chartCfg);
    QList<StatisticIndexCfg*> cfgs;
    cfgs.append(&chartChart);
    getAllCfg(cfgs);
    QList<StatisticIndexCfg*>::iterator it=cfgs.begin(),end=cfgs.end();
    for(;it!=end;it++)
    {
        (*it)->loadDefault();
    }
}

void StockDayChart::getAllCfg(QList<StatisticIndexCfg*>& cfgs)
{
    QList<StatisticIndex*>::iterator it=indexes.begin(),end=indexes.end();
    for(;it!=end;it++)
    {
        StatisticIndexCfg* cfg=(*it)->getCfg();
        if(cfg)
            cfgs.append(cfg);
    }
}
void StockDayChart::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    sizeChanged();
}

void StockDayChart::setViewIndex(int index,StatisticIndex* p)
{
    m_view[index].pIndex=p;
}


bool StockDayChart::isDataValid()const
{
    return !(m_countItem < 0||m_stockDay==NULL||m_stockDay->getCount()==0);
}

bool StockDayChart::isIndexValid(int index)const
{
    return index>=0&&index<m_countValid;
}


void StockDayChart::sizeChanged()
{
    QRectF rc;//=rect();

    getChartRect(rc);

    int itemHeight = rc.height();///5;
    qreal top;
    //当前视图可以显示的最多个数
    m_countItem = (rc.width() - chartCfg.itemSep)/barFullWidth();
    m_countValid=m_countItem;
    //如果没有数据，或者当前视图太小，不能显示，不处理
    if (!isDataValid())
    {
        m_countValid=0;
        m_countItem = 0;
    }
    else
    {
        //if(m_countItem < items->size())
        //    m_startIndex=items->size()-m_countItem;
        //如果当前前面有数据，此视图可显示的数据不足
        if (m_startIndex + m_countItem > m_stockDay->getCount())
        {
            //当前可以显示的个数大于实际总个数
            if (m_countItem > m_stockDay->getCount())
            {
                //m_countItem=stockDay->getCount();
                m_countValid=m_stockDay->getCount();
                m_startIndex = 0;
            }
            else//把开始索引向前调整
                m_startIndex = m_stockDay->getCount() - m_countItem;
        }
    }
    uint8_t* percents=(uint8_t*)malloc(sizeof(uint8_t)*DayChartType_Count);
    uint16_t total=0;
    for(int i=0;i<DayChartType_Count;i++)
    {
        percents[i]=m_view[i].percent;
        if(!isViewVisible((DayChartType)i))
        {
            percents[i]=0;
        }
        else
            total+=percents[i];
    }
    if(total==0)
        goto sizeEnd;
    for(int i=0;i<DayChartType_Count;i++)
    {
        percents[i]=100*percents[i]/total;
    }
    top=rc.top();
    for(int i=0;i<DayChartType_Count;i++)
    {
        if(isViewVisible((DayChartType)i))
        {
            rcViews[i]=rc;
            rcViews[i].setY(top);
            rcViews[i].setHeight(itemHeight*percents[i]/100);
            top=rcViews[i].bottom();
        }
        else
        {
            rcViews[i].setHeight(0);
        }
    }
    emit viewDataChanged(this,m_startIndex,m_countValid);
sizeEnd:
    free(percents);
}
int StockDayChart::getValidCount()const
{
    return m_countValid;
}
void StockDayChart::onViewCfgChanged()
{
    StatisticIndex* pIndex;
    for(int i=0;i<DayChartType_Count;i++)
    {
        pIndex=m_view[i].pIndex;
        if(isViewVisible((DayChartType)i)&&pIndex)
        {
            if(!pIndex->isFinished())
            {
                pIndex->init(m_stockDay->getStockData());
                //pIndex->reCalcIndex();
                //pIndex->reCalcRange(m_startIndex,m_countValid);
            }
        }
    }

    QList<StatisticIndex*> vindexes;
    getValidateIndex(vindexes);
    QList<StatisticIndex*>::iterator it=indexes.begin(),end=indexes.end();

    //不需要的数据删除
    for(;it!=end;it++)
    {
        if(std::find(vindexes.begin(),vindexes.end(),*it)!=vindexes.end())
        {
        }
        else
        {
            (*it)->deInit();
        }
    }
}

void StockDayChart::reCalcIndex()
{
    resetIndexData();
    resetSelect();
    QList<StatisticIndex*> indexes;
    getValidateIndex(indexes);

    const StockDayData* data=m_stockDay->getStockData();
    QList<StatisticIndex*>::iterator it=indexes.begin(),end=indexes.end();
    for(;it!=end;it++)
    {
        // if(*it)
        // {
        (*it)->init(data);
        (*it)->reCalcIndex();
        // }
    }
}

void StockDayChart::setStockDay(StockDay* stockDay)
{
    m_stockDay=stockDay;

    reCalcIndex();

}

void StockDayChart::resetIndexData()
{
    QList<StatisticIndex*>::iterator it=indexes.begin(),end=indexes.end();
    for(;it!=end;it++)
    {
        (*it)->deInit();
    }
}


void StockDayChart::paintMargin(QPainter* painter,const QRectF& rcChart,const QRectF& rc)
{
    painter->setPen(chartCfg.fore);
    painter->drawText(rcChart.left(),rc.top(),rcChart.width(),
                      rcChart.top()-rc.top(),Qt::AlignCenter,title);
}
//void StockDayChart::paintEvent(QPaintEvent* event)
void StockDayChart::paintCharts(QPainter* painter,const QRectF& rcChart)
{
    if(m_countValid<=0||!isDataValid())return;

    QFont font(chartCfg.fontFace, chartCfg.fontSize);
    painter->save();
    painter->setFont(font);

    painter->setPen(chartCfg.fore);// Qt::white);

    PaintParam param(this);

    param.initColorType(m_stockDay->getCloses(),m_stockDay->getOpens());
    param.p=painter;

    param.clrDown=m_drawParam.down;
    param.clrUp=m_drawParam.up;
    QFontMetrics fm(painter->font());
    for(int i=0;i<(int)DayChartType_Count;i++)
    {
        if(!isViewVisible((DayChartType)i))
            continue;
        param.rc=rcViews[i];
        param.rc.adjust(0,m_drawParam.viewTopPadding,0,0);
        painterChart(m_view[i].pIndex,&m_view[i].lines,&param);
        if(curPos.viewIdx==i)
        {
            param.drawLineAndValue(curPos.y,param.rc.right(),chartCfg.marginRight,fm.height());
        }
    }

    //if(m_curIndex!=-1)
    if(curPos.xIndex!=-1)//curPos.xIndex!=-1)
    {
        QRectF rc;//=rcClient;
        getChartRect(rc);

        paintCurStatus(painter,rc,font);
    }
    //paintRange(&painter);
    param.freeColorType();
    //free(param.colorType);
    painter->restore();
}

void StockDayChart::paintCurStatus(QPainter* painter,const QRectF& rc,const QFont& font)
{
    painter->save();

    painter->setFont(font);
    painter->setPen(chartCfg.fore);

    //float x = rc.left() + m_drawParam.itemSep + m_curIndex * (barFullWidth()) +m_drawParam.itemWidth / 2;
    float x = rc.left() + chartCfg.itemSep + curPos.xIndex * (barFullWidth()) +chartCfg.itemWidth / 2;

    painter->drawLine(x, rc.y(), x, rc.bottom());
    int index=m_startIndex +curPos.xIndex;// m_curIndex;

    QRectF rcText;

    QList<ColorString*> colors;
    for(int i=0;i<(int)DayChartType_Count;i++)
    {
        if(isViewVisible((DayChartType)i))
        {
            rcText=rcViews[i];
            rcText.setHeight(m_drawParam.viewTopPadding);

            m_view[i].pIndex->getItemDescription(index,colors);

            painterCurStatus(painter,rcText,colors,font);
            clearColorStrings(colors);
        }
    }

    painter->restore();
}
void StockDayChart::getValidateIndex(QList<StatisticIndex*>& indexes)
{
    for(int i=0;i<(int)DayChartType_Count;i++)
    {
        if(isViewVisible((DayChartType)i)&&m_view[i].pIndex)
        {
            indexes.push_back(m_view[i].pIndex);
        }
    }
}

void StockDayChart::dayItemChanged()
{
    sizeChanged();
    repaint();
}

//
void StockDayChart::painterUser(int chartIndex,ChartDraw* pDraw,PaintParam* param)
{
    QRectF rcItem;
    qreal left=param->rc.x() +chartCfg.itemSep;
    for(int i=param->start;i<param->start+param->count;i++)
    {
        rcItem.setX(left);
        rcItem.setWidth(chartCfg.itemWidth);

        pDraw->chartDraw(chartIndex,i,rcItem,param);
        left+=barFullWidth();
    }
}

void StockDayChart::paintPillar(PaintParam* param,PChartPillar pPillar)
{
    param->p->save();

    int i;
    //qreal y0=param->getValuePos(param->z);
    if(pPillar->width<0.01)//宽度按1像素
    {
        float x = param->rc.x() + chartCfg.itemSep+chartCfg.itemWidth / 2;
        if(pPillar->colors)//不使用价格涨跌规则颜色
        {
            for(i=0;i<param->count;i++)
            {
                param->p->setPen(pPillar->colors[i]);
                param->p->drawLine(x,param->zero_y,x,param->getValuePos(pPillar->v[i]));
                x+=barFullWidth();
            }
        }
        else
        {
            for(i=0;i<param->count;i++)
            {
                param->p->setPen(param->clrType(i)?param->clrUp:param->clrDown);
                param->p->drawLine(x,param->zero_y,x,param->getValuePos(pPillar->v[i]));
                x+=barFullWidth();
            }
        }
    }
    else
    {
        QRectF rcItem;
        rcItem.setX(param->rc.x() + chartCfg.itemSep+(chartCfg.itemWidth- pPillar->width) / 2);
        rcItem.setWidth(pPillar->width);
        if(pPillar->colors)//不使用价格涨跌规则颜色
        {
            for(i=0;i<param->count;i++)
            {
                param->p->setPen(pPillar->colors[i]);
                rcItem.setY(param->getValuePos(pPillar->v[i]));
                rcItem.setBottom(param->zero_y);
                param->p->drawRect(rcItem);
                rcItem.moveLeft(rcItem.left()+barFullWidth());
            }
        }
        else
        {
            for(i=0;i<param->count;i++)
            {
                param->p->setPen(param->clrType(i)?param->clrUp:param->clrDown);
                rcItem.setY(param->getValuePos(pPillar->v[i]));
                rcItem.setBottom(param->zero_y);
                param->p->drawRect(rcItem);
                rcItem.moveLeft(rcItem.left()+barFullWidth());
            }
        }
    }
    param->p->restore();
}

void StockDayChart::paintLine(PaintParam* param,PChartLine pLine)
{
    //QRectF rcChart = p.rc;

    param->p->save();

    //float x = rcChart.x() + m_barSep+m_barWidth / 2;
    float x = param->rc.x() + chartCfg.itemSep+chartCfg.itemWidth / 2;
    QPen pen(pLine->color);
    pen.setWidthF(pLine->width);
    param->p->setPen(pen);

    QPointF* pts = new QPointF[param->count];
    int i=0;

    //for (; it < endit && it < end&&i<count; it++,i++)
    for(;i<pLine->count;i++)
    {
        pts[i].setX(x);
        //pts[i].setY(p.zero_y - (p.v[index]-p.zero_v) * p.rate);
        pts[i].setY(param->getValuePos(pLine->v[i]));
        x += barFullWidth();//m_barSep + m_barWidth;
    }
    param->p->drawPolyline(pts,i);
    delete []pts;
    param->p->restore();
}
void StockDayChart::paintYLabel(PaintParam* param)
{
    param->p->setPen(m_drawParam.grid);
    qreal right=param->rc.right();
    const int step=80;
    for(qreal yLine=param->zero_y;yLine>param->rc.top();yLine-=step)
    {
        param->p->drawLine(param->rc.left(),yLine,right,yLine);
    }
    QFontMetrics fm(param->p->font());
    QRectF rcText(right,0,chartCfg.marginRight,fm.height());
    param->p->setPen(chartCfg.fore);
    for(qreal yLine=param->zero_y;yLine>param->rc.top();yLine-=step)
    {
        rcText.moveTop(yLine-fm.height()/2);

        //QString s=QString("%1").arg(range->low + priceDiff * i / 6,0,'f',2);
        param->p->drawText(rcText,QString("%1").arg((param->zero_y-yLine)/param->rate+param->zero_v,0,'f',2));
    }
}

void StockDayChart::painterChart(StatisticIndex* pIndex,ReferLines* lines,PaintParam* param)
{
    if(pIndex==NULL)return;
    ChartDraw* pDraw=pIndex->getChartDraw();
    int count=pDraw->chartCount();
    param->rc.adjust(0,pDraw->paddingTop(),0,-pDraw->paddingBottom());

    const StockFloatRange* range=pIndex->getRange();
    float diff = range->high - range->low;

    param->rate = param->rc.height() / diff;
    param->zero_v=pDraw->getZeroY();
    param->zero_y=param->rc.bottom()-(param->rc.height()*(param->zero_v-range->low)/diff); //param->rc.bottom();

    paintYLabel(param);//Y坐标值
    if(lines->count)
        param->drawLines(lines->lines,lines->count);

    DataChartType chartType;
    for(int i=0;i<count;i++)
    {
        chartType=pDraw->chartType(i);
        switch(chartType)
        {
        case Chart_User:
            painterUser(i,pDraw,param);break;
        case Chart_Line:
        {
            ChartLine line;
            line.start=param->start;
            line.count=param->count;
            pDraw->lineValue(i,&line);
            paintLine(param,&line);
        }
            break;
        case Chart_Pillar:
            //break;
        case Chart_Pillar2:
        {
            ChartPillar pillar;
            initChartPillar(&pillar,param->start,param->count,pDraw->isPillarSysColor(i));
            //memset(&pillar,0,sizeof(ChartPillar));
            pillar.start=param->start;
            pillar.count=param->count;
            pDraw->pillarColor(i,&pillar);
            paintPillar(param,&pillar);
            //if(pillar.colors)
            //    free(pillar.colors);
            freeChartPillar(&pillar);
        }
            break;
        }
    }

}

void StockDayChart::setCurIndex(int index)
{
    curPos.xIndex=index-m_startIndex;
    if(curPos.xIndex>=0&&curPos.xIndex<m_countValid)
    {}
    else
        curPos.xIndex=0;
    repaint();
}

int StockDayChart::getStartIndex()const
{
    return m_startIndex;
}

void StockDayChart::setStartIndex(int index)
{
    m_startIndex=index;
    if(m_stockDay==NULL)return ;
    if (m_startIndex + m_countItem > m_stockDay->getCount())
    {
        if (m_countItem > m_stockDay->getCount())
            m_startIndex = 0;
        else
            m_startIndex = m_stockDay->getCount() - m_countItem;
    }
    sizeChanged();
    repaint();
}
// void StockDayChart::setTitle(const QString& title)
// {
//     m_title=title;
// }
void StockDayChart::painterCurStatus(QPainter* p,const QRectF& rc,const QList<ColorString*>& colors,const QFont& font)
{
    QList<ColorString*>::const_iterator it=colors.begin(),end=colors.end();
    QRectF rcText;
    qreal left=rc.left();
    QFontMetrics fm(font);
    QSize sz;
    for(;it!=end;it++)
    {
        p->setPen((*it)->color);
        sz=fm.size(Qt::TextSingleLine,(*it)->text);
        rcText=rc;
        rcText.setX(left);
        rcText.setWidth(sz.width());
        p->drawText(rcText,(*it)->text);
        left=rcText.right()+5;
    }
}

void StockDayChart::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    if(!isDataValid())return ;
    switch(event->key())
    {
    case Qt::Key_Left:
        if(curPos.xIndex==-1)
        {
            curPos.xIndex=m_countValid-1;
        }
        else if(curPos.xIndex-1>=0)
        {
            curPos.xIndex--;
            emit selectDay(this,m_startIndex+curPos.xIndex);
            repaint();
        }
        else
        {
            if(m_startIndex>0)
            {
                m_startIndex--;
                this->sizeChanged();
                emit selectDay(this,m_startIndex+curPos.xIndex);
                repaint();
            }
        }
        break;
    case Qt::Key_Right:
        if(curPos.xIndex==-1)
        {
            curPos.xIndex=0;
        }
        else if(m_startIndex+curPos.xIndex+1<m_stockDay->getCount())
        {
            if(curPos.xIndex+1<m_countItem)
                curPos.xIndex++;
            else
            {
                m_startIndex+=1;
                this->sizeChanged();
            }
            emit selectDay(this,m_startIndex+curPos.xIndex);
            repaint();
        }
        break;
    case Qt::Key_PageUp:
        if(m_startIndex>0)
        {
            if(m_startIndex>m_countItem)
                m_startIndex-=m_countItem;
            else
                m_startIndex=0;
            this->sizeChanged();
            emit selectDay(this,m_startIndex+curPos.xIndex);
            repaint();
        }
        break;
    case Qt::Key_PageDown:
        if(m_startIndex+m_countItem+1<m_stockDay->getCount())
        {
            if(m_startIndex+m_countItem*2<m_stockDay->getCount())
                m_startIndex+=m_countItem;
            else
                m_startIndex=m_stockDay->getCount()-m_countItem;
            if(m_startIndex<0)
                m_startIndex=0;
            this->sizeChanged();
            emit selectDay(this,m_startIndex+curPos.xIndex);
            repaint();
        }
        break;
    }
}

bool StockDayChart::getItemDescribe(const ChartPos* pos,QString& out)const
{
    if(pos->xIndex==-1)return false;
    QList<ColorString*> colors;
    for(int i=0;i<(int)DayChartType_Count;i++)
    {
        if(isViewVisible((DayChartType)i))
        {
            m_view[i].pIndex->getItemDescription(pos->xIndex+m_startIndex,colors);
            QList<ColorString*>::const_iterator it=colors.begin(),end=colors.end();
            for(;it!=end;it++)
            {
                out.append((*it)->text);
                out.append(";");
            }
            clearColorStrings(colors);
        }
    }
    return true;
}

bool StockDayChart::getItemDescribeEx(const ChartPos* pos,QString& out)const
{
    return false;
}

bool StockDayChart::getRangeDescribe(const ChartPos* start,const ChartPos* end,QString& out)const
{
    int idxBegin,idxEnd;
    if(!getValidIndex(start,end,idxBegin,idxEnd))return false;

    double highs=DBL_MIN,lows=DBL_MAX;
    double startOpen,endClose;
    int pos;
    double volCount=0;
    for(int i=idxBegin;i<=idxEnd;i++)
    {
        pos=i+m_startIndex;
        if(highs<m_stockDay->getHighs()[pos])
            highs=m_stockDay->getHighs()[pos];
        if(lows>m_stockDay->getLows()[pos])
            lows=m_stockDay->getLows()[pos];
        volCount+=m_stockDay->getVoturnovers()[pos];
    }
    startOpen=m_stockDay->getOpens()[idxBegin+m_startIndex];
    endClose=m_stockDay->getCloses()[idxEnd+m_startIndex];

    out=QString(tr("high:%1;low%2;rate:%3\nopen:%4:close:%5;rate:%6;total day:%7,total vol %8"))
              .arg(highs).arg(lows).arg((highs-lows)*100/lows,0,'f',2)
              .arg(startOpen).arg(endClose).arg((endClose-startOpen)*100/startOpen,0,'f',2)
              .arg(idxEnd-idxBegin+1).arg(volCount,0,'f',2);
    return true;
}

const ChartViewCfg* StockDayChart::getViewCfg()const
{
    return m_view;
}
void StockDayChart::setViewCfg(const ChartViewCfg* cfg)
{
    memcpy(m_view,cfg,sizeof(ChartViewCfg)*DayChartType_Count);

    onViewCfgChanged();
    sizeChanged();
    repaint();
}
const DayChartParam* StockDayChart::getDrawParam()const
{
    return &m_drawParam;
}
void StockDayChart::loadDayChartParam(ChartViewCfg* cfg,int len)
{
    cfg[DayChartType_Main].percent=60;
    cfg[DayChartType_Main].flag=ChartParam_Flag_Visible;
    cfg[DayChartType_Main].pIndex=indexes.at(0);

    cfg[DayChartType_View2].percent=20;
    cfg[DayChartType_View2].flag=ChartParam_Flag_Visible;
    cfg[DayChartType_View2].pIndex=indexes.at(1);

    cfg[DayChartType_View3].percent=20;
    cfg[DayChartType_View3].flag=ChartParam_Flag_Visible;
    cfg[DayChartType_View3].pIndex=indexes.at(2);

    //cfg->fontSize=10;
    //strcpy(cfg->fontFace,"宋体");
}

}
