#include "stockdaychartview.h"

#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFontMetrics>

StockDayChartView::StockDayChartView(QWidget *parent)
    : QWidget(parent),stockDay(NULL),
    //m_paddingLeft(5),m_paddingTop(20),m_paddingRight(40),m_paddingBottom(5),
    //m_barSep(2),m_barWidth(7),
    m_curIndex(-1),
    m_startIndex(0), m_countItem(0),
    //m_chartTopPadding(20), m_chartBottomPadding(5),
    //m_view3PaddingTop(15),m_view2PaddingTop(15),m_mainPaddingBottom(20),
    m_countValid(0)
    ,rangeUpdated(false)

{
    this->setFocusPolicy(Qt::ClickFocus);
    loadDayChartParam(&m_cfg);
    loadDayChartDrawParam(&m_drawParam);
}

const DayChartDrawParam* StockDayChartView::getDrawParam()const
{
    return &m_drawParam;
}
void StockDayChartView::setDrawParam(const DayChartDrawParam* param)
{
    memcpy(&m_drawParam,param,sizeof(DayChartDrawParam));
    repaint();
}
void StockDayChartView::setStockDay(StockDay* stockDay)
{
    this->stockDay=stockDay;
}
void StockDayChartView::onCurrChanged()
{
    //PStockDayItem p=items->at(m_startIndex+m_curIndex);
    //this->repaint();
    emit selectDay(m_startIndex+m_curIndex);
}
void StockDayChartView::keyPressEvent(QKeyEvent *event)
{
    if(stockDay==NULL||stockDay->getCount()==0)return ;
    switch(event->key())
    {
    case Qt::Key_Left:
        if(m_curIndex-1>=0)
        {
            m_curIndex--;
            onCurrChanged();
        }
        else
        {
            if(m_startIndex>0)
            {
                m_startIndex--;
                this->sizeChanged();
                onCurrChanged();
            }
        }
        break;
    case Qt::Key_Right:
        if(m_startIndex+m_curIndex+1<stockDay->getCount())
        {
            if(m_curIndex<m_countItem)
                m_curIndex++;
            else
            {
                m_startIndex+=1;
                this->sizeChanged();
            }
            onCurrChanged();
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
            onCurrChanged();
        }
        break;
    case Qt::Key_PageDown:
        if(m_startIndex+m_countItem+1<stockDay->getCount())
        {
            if(m_startIndex+m_countItem*2<stockDay->getCount())
                m_startIndex+=m_countItem;
            else
                m_startIndex=stockDay->getCount()-m_countItem;
            if(m_startIndex<0)
                m_startIndex=0;
            this->sizeChanged();
            onCurrChanged();
        }
        break;
    }
}

void StockDayChartView::setCfg(const PDayChartParam pCfg)
{
    memcpy(&m_cfg,pCfg,sizeof(DayChartParam));
    sizeChanged();
}
const DayChartParam* StockDayChartView::getCfg()const
{
    return &m_cfg;
}
uint8_t StockDayChartView::mainHeight()
{
    return m_cfg.view[DayChartType_Main].percent;
}
uint8_t StockDayChartView::view2Height()
{
    return m_cfg.view[DayChartType_View2].percent;
}
uint8_t StockDayChartView::view3Height()
{
    return m_cfg.view[DayChartType_View3].percent;
}
void StockDayChartView::setMainHeight(uint8_t percent)
{
    m_cfg.view[DayChartType_Main].percent=percent;
}
void StockDayChartView::setView2Height(uint8_t percent)
{
    m_cfg.view[DayChartType_View2].percent=percent;
}
void StockDayChartView::setView3Height(uint8_t percent)
{
    m_cfg.view[DayChartType_View3].percent=percent;
}

void StockDayChartView::setFlag(uint32_t flag)
{
    m_cfg.flag=flag;
}
uint32_t StockDayChartView::getFlag()
{
    return m_cfg.flag;
}
void StockDayChartView::getChartRect(QRectF& rc)
{
    rc=rect();
    rc.adjust(m_drawParam.marginLeft,m_drawParam.marginTop,
              -m_drawParam.marginRight,-m_drawParam.marginBottom);
}
void StockDayChartView::mouseMoveEvent(QMouseEvent* event)
{
    if(stockDay==NULL||stockDay->getCount()==0)return ;
    QRectF chartRect=rect();
    //chartRect.adjust(m_paddingLeft,m_paddingTop,-m_paddingRight,-m_paddingBottom);
    getChartRect(chartRect);

    m_curY=event->pos().y();
    if(chartRect.contains(event->pos()))
    {
        //int curIndex=(event->pos().x()-chartRect.left()-m_barSep)/(m_barSep+m_barWidth);
        int curIndex=(event->pos().x()-chartRect.left()-barSep())/barFullWidth();

        if(m_startIndex+curIndex<stockDay->getCount())
        {

            m_curIndex=curIndex;
            this->repaint();
        }
    }
    else
    {
        if(m_curIndex!=-1)
        {
            m_curIndex=-1;
            repaint();
        }

    }
}

void StockDayChartView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    sizeChanged();
}
void StockDayChartView::sizeChanged()
{
    QRectF rc;//=rect();

    //SetRect(&rc,m_padding, m_padding, cx - m_padding, cy - m_padding);
    getChartRect(rc);
    //rc.adjust(m_paddingLeft,m_paddingTop,-m_paddingRight,-m_paddingBottom);
    int itemHeight = rc.height();///5;

    m_rcMain=rc;

    //当前视图可以显示的最多个数
    //m_countItem = (m_rcMain.right() - m_rcMain.x())/(m_barSep+m_barWidth);
    m_countItem = (m_rcMain.right() - m_rcMain.x())/barFullWidth();
    m_countValid=m_countItem;
    //如果没有数据，或者当前视图太小，不能显示，不处理
    if (m_countItem < 0||stockDay==NULL||stockDay->getCount()==0)m_countItem = 0;
    else
    {
        //if(m_countItem < items->size())
        //    m_startIndex=items->size()-m_countItem;
        //如果当前前面有数据，此视图可显示的数据不足
        if (m_startIndex + m_countItem > stockDay->getCount())
        {
            //当前可以显示的个数大于实际总个数
            if (m_countItem > stockDay->getCount())
            {
                //m_countItem=stockDay->getCount();
                m_countValid=stockDay->getCount();
                m_startIndex = 0;
            }
            else//把开始索引向前调整
                m_startIndex = stockDay->getCount() - m_countItem;
        }
    }
    uint8_t main=m_cfg.view[DayChartType_Main].percent,
        view2=m_cfg.view[DayChartType_View2].percent,
        view3=m_cfg.view[DayChartType_View3].percent,total;

    if(!IsMainVisible(&m_cfg))
        main=0;

    if(!IsView2Visible(&m_cfg))
        view2=0;
    if(!IsView3Visible(&m_cfg))
        view3=0;

    total=main+view2+view3;
    if(total==0)return;

    main=main*100/total;
    view2=view2*100/total;
    view3=view3*100/total;
    uint32_t top=rc.top();
    if(IsMainVisible(&m_cfg))
    {
        m_rcMain.setHeight(itemHeight*main/100);// * 3);
        top=m_rcMain.bottom();
    }
    if(IsView2Visible(&m_cfg))
    {
        getChartRect(m_rc1);
        //m_rc1.setX(m_paddingLeft);
        m_rc1.setY(top);
        //m_rc1.setWidth(rc.width());
        m_rc1.setHeight(itemHeight*view2/100);
        top=m_rc1.bottom();
    }
    if(IsView3Visible(&m_cfg))
    {
        getChartRect(m_rc2);
        //m_rc2.setX(m_paddingLeft);
        m_rc2.setY(top);
        //m_rc2.setWidth(rc.width());
        m_rc2.setHeight(itemHeight*view3/100);
    }
    setRangeUpdate(false);
    emit viewDataChanged(this,m_startIndex,m_countValid);
    //if(items!=NULL)
    //    clacDrawParam();
}
void StockDayChartView::setRangeUpdate(bool update)
{
    rangeUpdated=update;
    repaint();
}
#if 0
void StockDayChartView::paintMACD(int index,const PaintParam& p)
{
    PaintLineParam param(p);

    param.clr=m_drawParam.macd_diff;// Qt::white;
    StockMACD* macd=stockDay->getMacd(index);
    param.v=macd->diff;
    const StockFloatRange* range=&macd->range;
    qreal height=p.rc.height();
    param.rate=height/(range->high-range->low);
    param.zero_v=0;
    param.lineWidth=m_drawParam.macd_line_width;
    //int absBiggest=std::max<int>(std::abs(range->high),std::abs(range->low));
    //if(range->high>0)
    param.zero_y=p.rc.y()+ range->high*param.rate;

    paintLine(param);

    param.v=macd->dea;
    param.clr=m_drawParam.macd_dea;// Qt::yellow;
    paintLine(param);

    PaintPillarParam macdParam(p);
    macdParam.clrInc=m_drawParam.up;// Qt::red;
    macdParam.clrDec=m_drawParam.down;// Qt::green;
    macdParam.zero_v=0;
    macdParam.zero_y=param.zero_y;
    macdParam.rate=param.rate;
    macdParam.v=macd->macd;
    bool* colorType=new bool[p.count];
    for(int i=p.start;i<p.start+p.count;i++)
    {
        colorType[i-p.start]=macd->macd[i]>0;
    }
    macdParam.colorType=colorType;
    paintPillar2(macdParam);
    macdParam.drawLineAndValue(m_curY,macdParam.rc.right(),m_drawParam.marginRight);//m_paddingRight
    delete []colorType;
    //param.clr=Qt::red;
    //paintLine(param);
}
#endif
void StockDayChartView::paintPillar2(const PaintPillarParam& p)
{
    //QRectF rcChart = p.rc;

    p.p->save();

    int index=p.start,end;

    end=p.start+p.count;

    float y,height;// = centerY + (*it)->diff * rate;
    float x = p.rc.x() + barSep();// m_barSep;

    p.p->setPen(p.clrInc);

    for(;index<end;index++)
    {
        if(p.colorType[index-p.start])
        {
            height= p.getValueHeight(index);//p.v[index]- p.zero_v) * p.rate;
            //y = rcChart.bottom() -height;// ((*it)->voturnover- minVolume) * rate;

            y = p.zero_y - height;
            p.p->drawLine(x, p.zero_y, x, y);
        }
        x += barFullWidth();// m_barSep + m_barWidth;
    }
    p.p->setPen(p.clrDec);
    index=p.start;
    x = p.rc.x() +barSep();// m_barSep;
    for(;index<end;index++)
    {
        if(!p.colorType[index-p.start])
        {
            //height=(p.v[index]- p.zero_v) * p.rate;
            height= p.getValueHeight(index);
            //y = rcChart.bottom() -height;// ((*it)->voturnover- minVolume) * rate;

            y = p.zero_y - height;
            p.p->drawLine(x, p.zero_y, x, y);
        }
        x +=barFullWidth();//  m_barSep + m_barWidth;
    }
    p.p->restore();
}

void StockDayChartView::paintLine(const PaintLineParam& p)
{
    QRectF rcChart = p.rc;

    p.p->save();

    int index=p.start,end;

    end=p.start+p.count;

    //float x = rcChart.x() + m_barSep+m_barWidth / 2;
    float x = rcChart.x() + barSep()+barWidth() / 2;
    QPen pen(p.clr);
    pen.setWidthF(p.lineWidth/10);
    p.p->setPen(pen);

    QPointF* pts = new QPointF[p.count];
    int i=0;

    //for (; it < endit && it < end&&i<count; it++,i++)
    for(;index<end;index++,i++)
    {
        pts[i].setX(x);
        //pts[i].setY(p.zero_y - (p.v[index]-p.zero_v) * p.rate);
        pts[i].setY(p.getValuePos(index));
        x += barFullWidth();//m_barSep + m_barWidth;
    }
    p.p->drawPolyline(pts,i);
    delete []pts;
    p.p->restore();
}
#if 0
void StockDayChartView::paintKDJ(const PaintParam& p)
{
    PaintLineParam param(p);
    StockKDJ* kdj=stockDay->getKdj();
    const StockFloatRange* range=&kdj->range;


    param.lineWidth=m_drawParam.kdj_line_width;
    param.rate = (float)p.rc.height() / (range->high-range->low);
    param.zero_v=range->low;
    param.zero_y= p.rc.bottom();// rcChart.y() + rcChart.height()*maxkdj*rate;

    param.clr=m_drawParam.kdj_d;// QColor(0xff,0xff,0);
    param.v=kdj->d;
    paintLine(param);


    param.clr=m_drawParam.kdj_j;//QColor(0xff,0x0,0);
    param.v=kdj->j;
    paintLine(param);

    param.clr=m_drawParam.kdj_k;// QColor(0xff,0xff,0xff);
    param.v=kdj->k;
    paintLine(param);
    param.drawHorzionalLine(20,m_drawParam.referline);// Qt::white);
    param.drawHorzionalLine(80,m_drawParam.referline);//Qt::white);
    param.drawLineAndValue(m_curY,param.rc.right(),m_drawParam.marginRight);// m_paddingRight);

}
void StockDayChartView::paintVolume(const PaintParam& p)
{
    PaintPillarParam param(p);
    param.v=stockDay->getVoturnovers();
    param.clrInc= m_drawParam.up;//Qt::red;
    param.clrDec= m_drawParam.down;//Qt::green;

    //qDebug()<<(uint32_t)p.colorType;
    const StockFloatRange* range=stockDay->getVolRange();
    qreal height=p.rc.height();
    param.rate=height/(range->high-range->low);
    param.zero_v=range->low;
    param.zero_y=p.rc.bottom();

    paintPillar(param);
    param.drawLineAndValue(m_curY,param.rc.right(),m_drawParam.marginRight);// m_paddingRight);
}
#endif
void StockDayChartView::paintPillar(const PaintPillarParam& p)
{
    //QRectF rcChart = p.rc;

    p.p->save();

    int index=p.start,end;

    end=p.start+p.count;

    float y,height;// = centerY + (*it)->diff * rate;
    float x = p.rc.x() +barSep();// m_barSep;

    p.p->setPen(p.clrInc);

    p.p->setBrush(Qt::NoBrush);

    for(;index<end;index++)
    {
        height=(p.v[index]- p.zero_v) * p.rate;
        y = p.zero_y -height;// ((*it)->voturnover- minVolume) * rate;
        //y=((*it)->voturnover- minVolume) * rate;
        if(p.colorType[index-p.start])
            p.p->drawRect(x,y,barWidth(),height);
        else
            p.p->fillRect(x,y,barWidth(),height,p.clrDec);
        //painter->drawLine(x, rcChart.bottom(), x, y);
        x += barFullWidth();// m_barSep + m_barWidth;
        //pDC->LineTo(x, y);

    }
    p.p->restore();
}


void StockDayChartView::dayItemChanged()
{
    sizeChanged();
    repaint();
}

void StockDayChartView::setTitle(const QString& title)
{
    m_title=title;
}
#if 0
void StockDayChartView::paintMainK(const PaintParam& p)
{
    PaintLineParam lineParam(p);

    const StockFloatRange* range=stockDay->getPriceRange();
    float priceDiff = range->high - range->low;

    float height = p.rc.height();// rc.bottom - rc.top - m_chartTopPadding - m_chartBottomPadding;
    lineParam.rate = height / priceDiff;
    lineParam.zero_v=range->low;
    lineParam.zero_y=p.rc.bottom();

    lineParam.lineWidth=m_drawParam.ma_line_width;
    //lineParam.p->setBrush(QBrush(Qt::black));
    //lineParam.p->setBrush(Qt::NoBrush);
    paintMainK1(lineParam);

    //QFont font("宋体", 10);
    //QBrush bshText(Qt::white);
    //p.p->setBrush(bshText);
    p.p->setPen(m_drawParam.fore);// Qt::white);
    //QRectF rcText(p.rc.right(), p.rc.bottom(),p.rc.right()+ m_paddingRight,
    //              p.rc.bottom()+m_mainPaddingBottom);
    QRectF rcText(p.rc.right(), p.rc.bottom(),p.rc.right()+ m_drawParam.marginRight,
                  p.rc.bottom()+m_drawParam.viewDateHeight);
    if(m_cfg.flag&CFG_CHART_SHOW_DATE)
    {
        int index=p.start,end=p.start+p.count;
        StockDate* dates=stockDay->getDates();
        for (uint32_t i=0; index<end; index++,i++)
        {
            if(i%20==0)
            {
                //p.p->drawText(p.rc.x()+i*(m_barWidth + m_barSep)+m_barSep+m_barWidth/2,
                p.p->drawText(p.rc.x()+i*barFullWidth()+barSep()+barWidth()/2,
                                  //p.rc.bottom()+m_mainPaddingBottom/2,
                              p.rc.bottom()+m_drawParam.viewTopPadding/2,
                                  QString("|%1").arg(dates[index].date));
            }
        }
    }
    const int yCount = 6;
    int yLabelItemHeight = p.rc.height() / yCount;
    rcText=QRectF(p.rc.right(), 0,p.rc.right()+ m_drawParam.marginRight, 20);


    //p.p->setFont(font);
    //Pen penLine(Color::Gray);

    //p.p->setPen(QPen(QColor(0x9f,0x9f,0x9f),0.4,Qt::PenStyle::SolidLine));
    p.p->setPen(QPen(QColor(m_drawParam.grid),0.4,Qt::PenStyle::SolidLine));
    for (int i = 0; i < yCount + 1; i++)
    {
        float y=p.rc.bottom()- yLabelItemHeight * i;
        rcText.setY( y-10);
        rcText.setBottom(rcText.y()+20);
        QString s=QString("%1").arg(range->low + priceDiff * i / 6,0,'f',2);
        lineParam.p->drawText(rcText,s);
        lineParam.p->drawLine(p.rc.x(), y, p.rc.right(), y);
    }
    //bshText.setColor(Qt::blue);
    //painter->setBrush(bshText);

    lineParam.drawLineAndValue(m_curY,p.rc.right(),m_drawParam.marginRight);// m_paddingRight);
    /*if(m_curY>p.rc.y()&&m_curY<p.rc.bottom())
    {
        lineParam.p->setPen(Qt::blue);
        float price=(lineParam.zero_y-m_curY)/lineParam.rate+range->low;
        QString s=QString("%1").arg(price,0,'f',2);
        lineParam.p->drawLine(p.rc.x(),m_curY,p.rc.right(),m_curY);
        rcText.setY(m_curY-10);
        rcText.setBottom(rcText.y()+20);
        lineParam.p->drawText(rcText,s);
    }*/
}

void StockDayChartView::paintMainK1(PaintLineParam& lineParam)
{
//    QRectF rcClient = p.rc;
    QPainter* painter=lineParam.p;

    //rcClient.setY(rcClient.y()+ m_chartTopPadding);
    //rcClient.setHeight(rcClient.height() - m_chartTopPadding);

    //vector<PStockDayItem>::const_iterator it = items->begin() + m_startIndex,
    //    end = items->end();
    int index=lineParam.start,end;

    end = lineParam.start + lineParam.count;

    StockFloat* opens=stockDay->getOpens(),
        *closes=stockDay->getCloses(),
            *highs=stockDay->getHighs(),
                *lows=stockDay->getLows();


    painter->save();
    //QPen penRed(Qt::red);
    //QPen penGreen(Qt::green);
    //penRed.setWidthF(1);
    //penGreen.setWidthF(1);
    //float prevPrice = 0;
    float open, close, high, low;
    //int left = rcClient.X + m_barSep;
    QRectF rcItem;
    rcItem.setX(lineParam.rc.x() +barSep());// m_barSep);
    rcItem.setWidth(barWidth());//m_barWidth);
    //QBrush bshGreen(Qt::green);

    QPoint pt1, pt2;
    painter->setPen(m_drawParam.up);
    //显示上涨部分

    //for (; it != end; it++)
    for(;index<end;index++)
    {
        //if (closes[index]  > opens[index])
        if(lineParam.colorType[index-lineParam.start])
        {
            open =lineParam.getValueHeight(opens[index]);
            close = lineParam.getValueHeight(closes[index]);
            high = lineParam.getValueHeight(highs[index]);
            low = lineParam.getValueHeight(lows[index]);

            rcItem.setY(lineParam.zero_y - close);
            rcItem.setHeight(close-open-1);//画矩形时高度会加1
            //pDC->Rectangle(&rcItem);
            painter->drawRect(rcItem);

            pt1.setX((rcItem.right() + rcItem.x()) / 2);
            pt2.setX(pt1.x());
            if (high > close)
            {
                pt1.setY(rcItem.y());
                pt2.setY(lineParam.zero_y - high);
                painter->drawLine(pt1, pt2);
            }
            if (low < open)
            {
                pt1.setY(rcItem.bottom());
                pt2.setY(lineParam.zero_y - low);
                painter->drawLine(pt1, pt2);
            }
        }

        rcItem.moveLeft(rcItem.left()+barFullWidth());//m_barWidth + m_barSep);
    }

    //显示下跌部分
    //it = items->begin() + m_startIndex;
    index=m_startIndex;
    QColor clrDown(m_drawParam.down);
    painter->setPen(clrDown);
    //painter->setBrush(bshGreen);
    rcItem.setX(lineParam.rc.x() +barSep());// m_barSep);
    rcItem.setWidth(barWidth());// m_barWidth);
    for(;index<end;index++)
    {
        //if (closes[index] <= opens[index])
        if(!lineParam.colorType[index-lineParam.start])
        {
            open =lineParam.getValueHeight(opens[index]);
            close = lineParam.getValueHeight(closes[index]);
            high = lineParam.getValueHeight(highs[index]);
            low = lineParam.getValueHeight(lows[index]);


            rcItem.setY(lineParam.zero_y - open);
            rcItem.setHeight(open- close);

            if(open==close)
            {
                painter->drawLine(rcItem.x(),lineParam.zero_y-open,rcItem.right(),lineParam.zero_y-open);
            }
            else
                painter->fillRect(rcItem,clrDown);


            if (high > open || low < close)
            {
                pt1.setX((rcItem.right() + rcItem.x()) / 2);
                pt2.setX(pt1.x());

                pt1.setY(lineParam.zero_y - low);
                pt2.setY (lineParam.zero_y - high);
                painter->drawLine(pt1, pt2);
            }
        }

        rcItem.moveLeft(rcItem.left()+barFullWidth());//m_barWidth + m_barSep);
    }
    //begin ma
    if(true)
    {
        //int color[4]={0xffff00,0xffffff,0xff00ff,0x00ff00};

        StockMA* ma= stockDay->getMa();
        for(int mai=0;mai<ma->ma_count;mai++)
        {
            if(ma->cfg[mai]==0)continue;

            lineParam.clr=m_drawParam.ma[mai];// color[mai];
            lineParam.v=ma->ma[mai];
            paintLine(lineParam);
        }
    }

    //end ma

    painter->restore();
}

void StockDayChartView::paintChart(uint16_t type,PaintParam& param)
{
    switch(type)
    {
    case ChartType_KLine:
        if(m_cfg.flag&CFG_CHART_SHOW_DATE)
            param.rc.adjust(0,0,0,-m_drawParam.viewDateHeight);//m_chartBottomPadding
        paintMainK(param);
        break;
    case ChartType_Volume:
    {
        paintVolume(param);
        //paintVolume(painter,rc);
    }
    break;
    case ChartType_Macd:
        paintMACD(0,param);
        break;
    case ChartType_Macd2:
        paintMACD(1,param);
        break;
    case ChartType_KDJ:
        paintKDJ(param);
        break;
    case ChartType_OBV:

        break;
    }
}
#endif
void StockDayChartView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    //if(!rangeUpdated)return;
    QPainter painter(this);
#if 0
    //painter.setRenderHint(QPainter::Antialiasing);
    QRect rcClient=rect();
    //painter.fillRect(rcClient,Qt::black);
    painter.fillRect(rcClient,m_drawParam.bg);
    //QRectF rcMain=rect();
    if(stockDay==NULL||stockDay->getCount()==0)return;
    //QFont font("宋体", 10);
    QFont font(m_drawParam.fontFace, m_drawParam.fontSize);
    painter.save();
    painter.setFont(font);

    painter.setPen(m_drawParam.fore);// Qt::white);
    //painter.drawText(m_paddingLeft,0,rcClient.width()-m_paddingRight-m_paddingLeft,
    //                 m_paddingTop,Qt::AlignCenter,m_title);
    painter.drawText(m_drawParam.marginLeft,0,rcClient.width()-m_drawParam.marginRight-m_drawParam.marginLeft,
                     m_drawParam.marginTop,Qt::AlignCenter,m_title);

    PaintParam param;

    param.p=&painter;
    param.count=m_countValid;
    //param.rc=rc;
    param.start=m_startIndex;

    bool* colorType=new bool[m_countValid];
    param.colorType=colorType;
    const double* closes=stockDay->getCloses();
    const double* opens=stockDay->getOpens();

    for(int start=m_startIndex;start<m_startIndex+m_countValid;start++)
    {
        colorType[start-m_startIndex]=closes[start]>opens[start];
    }

    if(IsMainVisible(&m_cfg))
    {
        //paintChart(m_cfg.view[DayChartType_Main].chartType,&painter,m_rcMain);
        param.rc=m_rcMain;
        //param.rc.adjust(0,m_chartTopPadding,0,-m_chartBottomPadding);
        param.rc.adjust(0,m_drawParam.viewTopPadding,0,0);
        paintChart(m_cfg.view[DayChartType_Main].chartType,param);
    }

    if(IsView2Visible(&m_cfg))
    {
        param.rc=m_rc1;
        //param.rc.adjust(0,m_chartTopPadding,0,-m_chartBottomPadding);
        param.rc.adjust(0,m_drawParam.viewTopPadding,0,0);
        paintChart(m_cfg.view[DayChartType_View2].chartType,param);

    }
    if(IsView3Visible(&m_cfg))
    {
        param.rc=m_rc2;
        //param.rc.adjust(0,m_chartTopPadding,0,-m_chartBottomPadding);
        param.rc.adjust(0,m_drawParam.viewTopPadding,0,0);
        paintChart(m_cfg.view[DayChartType_View3].chartType,param);
    }
    if(m_curIndex!=-1)
    {
        QRectF rc;//=rcClient;
        getChartRect(rc);
        painter.setRenderHint(QPainter::Antialiasing);
        //SetRect(&rc,m_padding, m_padding, cx - m_padding, cy - m_padding);
        //rc.adjust(m_paddingLeft,m_paddingTop,-m_paddingRight,-m_paddingBottom);
        paintCross(&painter,rc,font);
    }
    delete []colorType;

    painter.restore();
#endif
}

void StockDayChartView::painterValue(QPainter* p,const QRectF& rc,const QList<ColorString*>& colors,const QFont& font)
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
void StockDayChartView::paintCross(QPainter* painter,const QRectF& rc,const QFont& font)
{
#if 0
    painter->save();
    //QPen pen(Qt::white);
    //QFont font("宋体", 10);
    painter->setFont(font);

    painter->setPen(m_drawParam.fore);
    //float x = m_paddingLeft + m_barSep + m_curIndex * (m_barWidth + m_barSep) + m_barWidth / 2;
    float x = rc.left() + barSep() + m_curIndex * (barFullWidth()) +barWidth() / 2;

    //qDebug()<<this<<"x:"<<x;
    painter->drawLine(x, rc.y(), x, rc.bottom());

    //PStockDayItem p = items->at(m_startIndex + m_curIndex);
    int index=m_startIndex + m_curIndex;


    QRectF rcText;

    //rcText=QRectF(rc.x() + 2, m_rcMain.y(), rc.right(), m_rcMain.y()+m_drawParam.viewTopPadding);//m_chartTopPadding);

    //SolidBrush textBrush(Color::Black);
    //QString s;
    QList<ColorString*> colors;
    if(IsMainVisible(&m_cfg))
    {
        getChartIndexString(m_cfg.view[DayChartType_Main].chartType,index,colors);
        rcText=m_rcMain;
        rcText.setHeight(m_drawParam.viewTopPadding);
        painterValue(painter,rcText,colors,font);
        clearColorStrings(colors);
        // if(s.length())
        // {
        //     painter->drawText(rcText,s);
        // }

    }
    if(IsView2Visible(&m_cfg))//.flag&CFG_CHART_SHOW_VIEW3)
    {
        getChartIndexString(m_cfg.view[DayChartType_View3].chartType,index,colors);
        rcText=m_rc2;
        rcText.setHeight(m_drawParam.viewTopPadding);
        painterValue(painter,rcText,colors,font);
        clearColorStrings(colors);
        // if(s.length())
        // {
        //     rcText.setY(m_rc2.y());
        //     rcText.setHeight(m_drawParam.viewTopPadding);//m_view3PaddingTop);
        //     painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,s);
        // }
    }

    if(IsView3Visible(&m_cfg))//if(m_cfg.flag&CFG_CHART_SHOW_VIEW2)
    {
        getChartIndexString(m_cfg.view[DayChartType_View2].chartType,index,colors);
        rcText=m_rc1;
        rcText.setHeight(m_drawParam.viewTopPadding);
        painterValue(painter,rcText,colors,font);
        clearColorStrings(colors);

        // if(s.length())
        // {
        //     rcText.setY(m_rc1.y());
        //     rcText.setHeight(m_drawParam.viewTopPadding);//m_view3PaddingTop);
        //     painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,s);
        // }

    }

    painter->restore();
#endif
}

void StockDayChartView::getChartIndexString(uint16_t type,int index,QString& out)
{
#if 0
    //PStockDayItem pDayItem=items->at(index);
    switch(type)
    {
    case ChartType_KLine:
        stockDay->priceIndexStr(index,out);

        break;
    case ChartType_Volume:
        //out="";
        stockDay->volStr(index,out);
        break;
    case ChartType_Macd:
        //out=QString("diff:%1,dea:%2,macd:%3").arg(pDayItem->diff,0,'f',2)
        //          .arg(pDayItem->dea,0,'f',2).arg(pDayItem->macd,0,'f',2);
        stockDay->macdStr(index,0,out);
        break;
    case ChartType_Macd2:
    {
        //PStockDayMacd pMacd=macds->at(m_startIndex + m_curIndex);
        //out=QString("diff:%1,dea:%2,macd:%3").arg(pMacd->diff,0,'f',2)
        //        .arg(pMacd->dea,0,'f',2).arg(pMacd->macd,0,'f',2);
        stockDay->macdStr(index,1,out);
        break;
    }
    case ChartType_KDJ:
        //out=QString("k:%1d:%2,j:%3").arg(pDayItem->kdj_k,0,'f',2).arg(pDayItem->kdj_d,0,'f',2).arg(pDayItem->kdj_j,0,'f',2);
        stockDay->kdjStr(index,out);
        break;
    case ChartType_OBV:
        //out=QString("obv:%1").arg(pDayItem->obv,0,'f',2);
        break;
    }
#endif
}
void StockDayChartView::getChartIndexString(uint16_t type,int index,QList<ColorString*>& out)
{
#if 0
    QString t;
    switch(type)
    {
    case ChartType_KLine:
    {stockDay->priceIndexStr(index,t);
        out.push_back(new ColorString(t,m_drawParam.fore));
        StockMA* ma=stockDay->getMa();
        for(int i=0;i<MA_LEN;i++)
        {
            t=QString("MA%1:%2").arg(m_cfg.maSize[i]).arg(ma->ma[i][index],0,'f',2);
            out.push_back(new ColorString(t,m_drawParam.ma[i]));
        }
    }
        break;
    case ChartType_Volume:
        //out="";
        stockDay->volStr(index,t);
        out.push_back(new ColorString(t,m_drawParam.fore));
        break;
    case ChartType_Macd:
        //out=QString("diff:%1,dea:%2,macd:%3").arg(pDayItem->diff,0,'f',2)
        //          .arg(pDayItem->dea,0,'f',2).arg(pDayItem->macd,0,'f',2);
        {
            StockMACD* macd=stockDay->getMacd(0);
            t=QString("diff=%1").arg(macd->diff[index],0,'f',2);
            out.push_back(new ColorString(t,m_drawParam.macd_diff));
            t=QString("dea:%1").arg(macd->dea[index],0,'f',2);
            out.push_back(new ColorString(t,m_drawParam.macd_dea));
            t=QString("macd:%1").arg(macd->macd[index],0,'f',2);
            out.push_back(new ColorString(t,m_drawParam.fore));
        }
        //stockDay->macdStr(index,0,out);
        break;
    case ChartType_Macd2:
    {
        //PStockDayMacd pMacd=macds->at(m_startIndex + m_curIndex);
        //out=QString("diff:%1,dea:%2,macd:%3").arg(pMacd->diff,0,'f',2)
        //        .arg(pMacd->dea,0,'f',2).arg(pMacd->macd,0,'f',2);
        //stockDay->macdStr(index,1,out);
        {
            StockMACD* macd=stockDay->getMacd(1);
            out.push_back(new ColorString(QString("diff=%1").arg(macd->diff[index],0,'f',2),m_drawParam.macd_diff));
            out.push_back(new ColorString(QString("dea=%1").arg(macd->dea[index],0,'f',2),m_drawParam.macd_dea));
            out.push_back(new ColorString(QString("macd=%1").arg(macd->macd[index],0,'f',2),m_drawParam.fore));
        }
        break;
    }
    case ChartType_KDJ:
        //out=QString("k:%1d:%2,j:%3").arg(pDayItem->kdj_k,0,'f',2).arg(pDayItem->kdj_d,0,'f',2).arg(pDayItem->kdj_j,0,'f',2);
        //stockDay->kdjStr(index,out);
        {
            StockKDJ* kdj=stockDay->getKdj();
            out.push_back(new ColorString(QString("k=%1").arg(kdj->k[index],0,'f',2),m_drawParam.kdj_k));
            out.push_back(new ColorString(QString("d=%1").arg(kdj->d[index],0,'f',2),m_drawParam.kdj_d));
            out.push_back(new ColorString(QString("j=%1").arg(kdj->j[index],0,'f',2),m_drawParam.kdj_j));
        }
        break;
    case ChartType_OBV:
        //out=QString("obv:%1").arg(pDayItem->obv,0,'f',2);
        break;
    }
#endif
}


void StockDayChartView::setStartIndex(int index)
{
    m_startIndex=index;
    if (m_startIndex + m_countItem > stockDay->getCount())
    {
        if (m_countItem > stockDay->getCount())
            m_startIndex = 0;
        else
            m_startIndex = stockDay->getCount() - m_countItem;
    }
    sizeChanged();
    repaint();
}



void StockDayChartView::onActionBuy(bool)
{
    if(m_curIndex>=0)
        emit buy(m_startIndex+m_curIndex);
}
void StockDayChartView::onActionSell(bool)
{
    if(m_curIndex>=0)
        emit sell(m_startIndex+m_curIndex);
}
void StockDayChartView::onActionMin(bool)
{
    if(m_curIndex>=0)
        emit minute(m_startIndex+m_curIndex);
}

void StockDayChartView::contextMenuEvent(QContextMenuEvent* event)
{
    Q_UNUSED(event);
    /*QAction* actionBuy=new QAction("买");
    QAction* actionSell=new QAction("卖");
    QAction* actionMin=new QAction("分时图");
    QMenu* menu=new QMenu();
    connect(actionBuy,SIGNAL(triggered(bool)),this,SLOT(onActionBuy(bool)));
    connect(actionSell,SIGNAL(triggered(bool)),this,SLOT(onActionSell(bool)));
    connect(actionMin,SIGNAL(triggered(bool)),this,SLOT(onActionMin(bool)));
    menu->addAction(actionBuy);
    menu->addAction(actionSell);
    menu->addAction(actionMin);
    menu->exec(event->globalPos());*/
}


void StockDayChartView::setCurAbsoultIndex(int curIndex)
{
    this->m_curIndex=curIndex-m_startIndex;
    repaint();
}

void StockDayChartView::mousePressEvent(QMouseEvent* event)
{
    Qt::MouseButton btn=event->button();
    if(btn!=Qt::MouseButton::LeftButton)
        return;
    if(stockDay==NULL||stockDay->getCount()==0)return ;
    QRectF chartRect;//=rect();
    getChartRect(chartRect);
    //chartRect.adjust(m_paddingLeft,m_paddingTop,-m_paddingRight,-m_paddingBottom);

    if(chartRect.contains(event->pos()))
    {
        //int curIndex=(event->pos().x()-chartRect.left()-m_barSep)/(m_barSep+m_barWidth);
        int curIndex=(event->pos().x()-chartRect.left()-barSep())/barFullWidth();
        if(m_startIndex+curIndex<stockDay->getCount())
        {
            ///PStockDayItem p=items->at(m_startIndex+curIndex);
            emit selectDay(m_startIndex+curIndex);
        }
    }
}

