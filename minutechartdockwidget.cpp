#include "minutechartdockwidget.h"
#include "tdxminutedata.h"
#include "dataloadernetsohu.h"
#include "dataloaderlocalsohu.h"
#include "stockminutechart.h"

MinuteChartDockWidget::MinuteChartDockWidget(NetThreadPool* netThreadPool,const QString& title,QWidget* parent)
    :BaseDockWidget(title,parent),pNetPool(netThreadPool)//, netRequest(NULL)
    ,pStock(NULL),bTodayUpdate(false)
{
    createUI();
    loader[0]=new datasohu::DataLoaderLocal();
    loader[1]=new tdx::TDXMinuteData();
}


MinuteChartDockWidget::~MinuteChartDockWidget()
{
    if(pStock)
        pNetPool->removeStock(pStock,UPDATE_MINUTE,WND_FALG_MAIN);
    // if(netRequest)
    //     delete netRequest;
    for(int i=0;i<DATALOADER_MINUTE_COUNT;i++)
    {
        delete loader[i];
    }
}
void MinuteChartDockWidget::onDayMinuteUpdated(const StockListItem* stock,
                                        minutechart::StockDayMinute* pDay)
{
    if(!bTodayUpdate)return;
    if(pStock!=stock)return;
    minutechart::StockDayMinute* pOldDay=minChart->lastDay();
    bool bSame=pOldDay==pDay;
    if(!bSame)
        minChart->addDay(pDay);
    minChart->reCalcRange();
    minChart->repaint();
}

void    MinuteChartDockWidget::getThemingWidget(QList<QWidget*>& wids)
{
    wids.push_back(cbxPriority);
    wids.push_back(lblStatus);
    wids.push_back(cbxPriority);
}

const minutechart::StockMinuteChart* MinuteChartDockWidget::getMinuteChart()const
{
    return minChart;
}
void MinuteChartDockWidget::createUI()
{
    widgetMain=new QWidget(this);
    setWidget(widgetMain);

    layoutMain=new QVBoxLayout(widgetMain);
    widgetMain->setLayout(layoutMain);
    minChart=new minutechart::StockMinuteChart(widgetMain);
    layoutStatus=new QHBoxLayout(widgetMain);
    layoutMain->addLayout(layoutStatus);
    layoutMain->addWidget(minChart);
    minChart->setMouseTracking(true);
    layoutMain->setContentsMargins(0,0,0,0);

    layoutStatus->setContentsMargins(0,0,0,0);
    lblStatus=new QLabel(widgetMain);
    cbxPriority=new QComboBox(widgetMain);
    layoutStatus->addWidget(cbxPriority);
    layoutStatus->addWidget(lblStatus);
    QStringList priority;
    priority<<tr("sohu")<<tr("tdx");
    cbxPriority->addItems(priority);
    layoutMain->setStretch(0,0);
    layoutMain->setStretch(1,1);
    layoutStatus->setStretch(0,1);
    layoutStatus->setStretch(1,3);

    minChart->setCurrStatus(false);
    connect(minChart,SIGNAL(selectItem(ChartWidget*,ChartPos*)),this,SLOT(onSelectItem(ChartWidget*,ChartPos*)));
    connect(minChart,SIGNAL(selectRange(ChartWidget*,ChartPos*,ChartPos*,bool)),
            this,SLOT(onSelectRange(ChartWidget*,ChartPos*,ChartPos*,bool)));

    connect(pNetPool,SIGNAL(dayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*)),
            this,SLOT(onDayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*)));
    //connect(minChart,SIGNAL(onCurrTime(const StockPriceItem* )),this,SLOT(onMinCurrTime(const StockPriceItem*)));
}

void MinuteChartDockWidget::onSelectRange(ChartWidget*,ChartPos* start,ChartPos* end,bool changing)
{
    if(changing||start==NULL||end==NULL)return;
    QString out;
    minChart->getRangeDescribe(start,end,out);
    emit currentMessage(out);
}

void MinuteChartDockWidget::onSelectItem(ChartWidget*,ChartPos* pos)
{
    if(pos==NULL||pos->xIndex==-1)return;
    QString out,detail;
    minChart->getItemDescribe(pos,out);
    minChart->getItemDescribeEx(pos,detail);
    emit currentMessage(out+detail);
}

bool MinuteChartDockWidget::todayIsTradeTime(const QDate& date)const
{
   // return true;//测试
    if(date.dayOfWeek()>5)//周六、日
        return false;
    if(date!=QDate::currentDate())
        return false;
    QTime time=QTime::currentTime();
    if(time.hour()<9||(time.minute()<30&&time.hour()==9))
    {
        return false;
    }
    return true;
}


void MinuteChartDockWidget::setStock(const StockListItem* stock,
                                     const QDate& start,const QDate& end,double preClose,bool today)
{
    if(stock)
        pNetPool->removeStock(stock,UPDATE_MINUTE,WND_FALG_MAIN);
    pStock=stock;

    minChart->reset();
    //minChart->clearXChartLabels();

    bool bLoaded=false;
    if(preClose!=0)
    {
        minChart->setPrices(preClose,0,0,0);
    }
    //data.loadCode(minChart,date,date,code.c_str(),code.at(0)=='6'?"sh":"sz");
    QList<minutechart::StockDayMinute*> days;
    for(int i=0;i<DATALOADER_MINUTE_COUNT&&bLoaded==false;i++)
    {
        bLoaded=loader[i]->loadCode(&days,start,end,stock,minChart);
    }
    minChart->addDays(days);
    minChart->reCalcRange();

    QString title;
    stock->toString(title);
    minChart->setTitle(title);
    //setWindowTitle(QString("minute chart %1").arg(title));

    //StockChartView::addOneDayXLabel(minChart,0);
    minChart->repaint();
    if(today&&todayIsTradeTime(end))
    {
        bTodayUpdate=true;
        pNetPool->addStock(pStock,UPDATE_MINUTE,WND_FALG_MAIN);
    }else bTodayUpdate=false;
}

void MinuteChartDockWidget::setStock(const StockListItem* stock,const QDate& date,double preClose)
{
    //tdx::TDXMinuteData data;
    if(stock)
        pNetPool->removeStock(stock,UPDATE_MINUTE,WND_FALG_MAIN);
    pStock=stock;

    minChart->setStockItem(pStock);
    minChart->reset();
    //minChart->clearXChartLabels();

    bool bLoaded=false;
    QList<minutechart::StockDayMinute*> days;
    if(todayIsTradeTime(date))
        goto today;
    //if(preClose!=0)
    {
        minChart->setPrices(preClose,0,0,0);
    }
    //data.loadCode(minChart,date,date,code.c_str(),code.at(0)=='6'?"sh":"sz");

    for(int i=0;i<DATALOADER_MINUTE_COUNT&&bLoaded==false;i++)
    {
        bLoaded=loader[i]->loadCode(&days,date,date,stock,minChart);
    }
    minChart->addDays(days);
    minChart->reCalcRange();

    minChart->repaint();
    if(!bLoaded&&todayIsTradeTime(date))
    {
today:
        //请求当日数据
        bTodayUpdate=true;
        pNetPool->addStock(pStock,UPDATE_MINUTE,WND_FALG_MAIN);
    }
    else
        bTodayUpdate=false;
}

