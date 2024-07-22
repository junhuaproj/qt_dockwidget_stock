#include "usermainwnd.h"
#include "tdxminutedata.h"

#include "daychartcfgdlg.h"
#include <QMenu>
#include "Algorithm.h"
#include "ta_func.h"
//#include "chartcalcparamdlg.h"
#include "stockpricechgwidget.h"
#include "appconfig.h"
#include "AppConfigqss.h"
using namespace chart;
UserMainWnd::UserMainWnd(StockListItem* stock,NetThreadPool* netThreadPool,
                         data::DataLoader* p,QWidget *parent)
    : BaseMainWindow(netThreadPool,p,parent)//,pDataLoader(p)
    ,mainStock(stock),
    priceChgWidget(NULL),dockPrice(NULL),curDayChart(NULL)

{

}

UserMainWnd::~UserMainWnd()
{
}

void UserMainWnd::connectSignal()
{
    connect(dayChart,SIGNAL(selectDay(chart::StockDayChart*,int)),
            this,SLOT(onDayChartCurrStockDay(chart::StockDayChart*,int)));
}
void UserMainWnd::loadUi()
{
    mainWidget=new QWidget(this);
    mainLayout=new QVBoxLayout(this);
    //dockMin=new QDockWidget(this);
    //minChart=new StockChartView(this);
    dayChart=new StockDayChart(this);

    dockTool=new BaseDockWidget(tr("tool"),this);
    minuteDock=new MinuteChartDockWidget(pNetPool,tr("minute"),this);
    connect(minuteDock,SIGNAL(currentMessage(QString)),SIGNAL(currentMessage(QString)));

    toolBox=new QToolBox(dockTool);
    calendarWidget=new QCalendarWidget(toolBox);
    toolBox->addItem(calendarWidget,"date");
    dockTool->setWidget(toolBox);


    mainLayout->addWidget(dayChart);
    mainWidget->setLayout(mainLayout);

    this->setCentralWidget(mainWidget);
    dockTool->setWindowTitle(tr("tool"));
    //dockMin->setWindowTitle(tr("minute chart"));
    addDockWidget(Qt::RightDockWidgetArea,dockTool);
    addDockWidget(Qt::RightDockWidgetArea,minuteDock);

    dayChart->setMouseTracking(true);
    //minChart->setMouseTracking(true);

    dayChart->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dayChart,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onDayChartContext(QPoint)));
    connect(dayChart,SIGNAL(viewDataChanged(chart::StockDayChart*,int,int)),this,SLOT(onDayChartChanged(chart::StockDayChart*,int,int)));

    connect(calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(onCalendar(const QDate)));

    createPriceChgDock();
    tabifyDockWidget(minuteDock,dockPrice);
    minuteDock->raise();

}
void UserMainWnd::onDayChartChanged(chart::StockDayChart* view,int start,int count)
{
    if(stockDay.getCount()<=0)return;

    reCalcDayChartRange(view,start,count);
}
void UserMainWnd::onDayChartCurrStockDay(StockDayChart*,int index)
{
    if(isStockValid())
    {
        QDate date=QDate::fromString(stockDay.getDates()[index].date,"yyyy-MM-dd");

        //PStockDayItem prev=dayItems.getPrevItem(p);
        onShowStockMinuteChart(index-1,mainStock,date);
    }
}
bool UserMainWnd::isStockValid()const
{
    return mainStock!=NULL;
}
void UserMainWnd::onCalendar(const QDate date)
{
    QDate prev=date.addDays(-10);//先选择前面10天的，
    QString strDate=prev.toString("yyyy-MM-dd");
    int index=stockDay.findDate(strDate);
    //如果不存在选择这天
    if(index==-1)
    {
        strDate=date.toString("yyyy-MM-dd");
        index=stockDay.findDate(strDate);
    }
    if(index>=0)
        dayChart->setStartIndex(index);
    setPriceDate(date);
    if(mainStock)
        onShowStockMinuteChart(stockDay.getCount()-1,mainStock,date);
}

void UserMainWnd::onShowStockMinuteChart(int prevIndex,const StockListItem* stock,const QDate& date)
{
    double preClose=0;
    if(prevIndex>=0)
    {
        preClose=stockDay.getCloses()[prevIndex];
    }
    minuteDock->setStock(stock,date,preClose);

}


void UserMainWnd::loadStock(StockListItem* pItem,StockDay* pStockDay,
                            chart::StockDayChart *dayChart)
{
    pStockDay->clear();
    dayChart->setTitle(pItem->getCode());

    loadFromSQLite(pStockDay,pItem);
    //const StockDayData* data=pStockDay->getStockData();
    dayChart->setStockDay(pStockDay);

    int index=0;
    if(calendarWidget->selectedDate()==QDate::currentDate())
    {
        index=pStockDay->getCount()-1;
    }
    else
    {
        index=findDate(pStockDay,calendarWidget->selectedDate());
    }
    if(index>=0)
        dayChart->setStartIndex(index);
    dayChart->dayItemChanged();
}

void UserMainWnd::showDayChartContextMenu(chart::StockDayChart* chart)
{
    QAction* actionCfg=new QAction("configuration");
    QAction* actionParam=new QAction("calc param");
    QAction* actionTheme=new QAction("theme");
    QAction* actionRefer=new QAction(tr("refer line"));
    QAction* actionReCalc=new QAction(tr("recalc"));

    //QAction* actionAddToDay=new QAction("Add To Today");

    QMenu* menu=new QMenu();

    connect(actionCfg,SIGNAL(triggered(bool)),this,SLOT(onActionConfig(bool)));
    connect(actionParam,SIGNAL(triggered(bool)),this,SLOT(onActionCalcParam(bool)));
    connect(actionTheme,SIGNAL(triggered(bool)),this,SLOT(onActionTheme(bool)));
    connect(actionRefer,SIGNAL(triggered(bool)),dayChart,SLOT(showReferLineDlg(bool)));
    connect(actionReCalc,SIGNAL(triggered(bool)),this,SLOT(onReCalcIndex(bool)));

    //connect(actionAddToDay,SIGNAL(triggered(bool)),this,SLOT(onActionAddTodayStock(bool)));

    //menu->addAction(actionNewWin);
    menu->addAction(actionCfg);
    menu->addAction(actionParam);
    menu->addAction(actionTheme);
    menu->addAction(actionRefer);
    menu->addAction(actionReCalc);

    curDayChart=chart;
    menu->exec(QCursor::pos());
}

void UserMainWnd::onDayChartContext(QPoint)
{
    showDayChartContextMenu(dayChart);
}
chart::StockDayChart* UserMainWnd::getCurDayChart()
{
    return curDayChart;
}
void UserMainWnd::onReCalcIndex(bool)
{
    getCurDayChart()->reCalcIndex();
}
void UserMainWnd::onActionTheme(bool)
{
    getCurDayChart()->execThemeDlg(pDataLoader);
}

void UserMainWnd::onActionCalcParam(bool)
{

}

void UserMainWnd::onActionConfig(bool)
{
    ChartViewCfg param[DayChartType_Count];
    memcpy(param,getCurDayChart()->getViewCfg(),sizeof(ChartViewCfg)*DayChartType_Count);
    chart::DayChartCfgDlg* dlg=new chart::DayChartCfgDlg(getCurDayChart()->getIndexes(),param,DayChartType_Count,this);
    if(dlg->exec()==QDialog::Accepted)
    {
        getCurDayChart()->setViewCfg(param);
        //onValidIndexChanged();
    }
    delete dlg;
}
void UserMainWnd::loadFromSQLite(StockDay* stockDay,const StockListItem* p)
{
    if((p->getFlag()&(StockItemFlag_CN|StockItemFlag_ZS))==(StockItemFlag_CN|StockItemFlag_ZS))
    {
        loadFromSQLite(stockDay,p->getCode(),"zs");
    }
    else if((p->getFlag()&(StockItemFlag_CN))==(StockItemFlag_CN))
    {
        loadFromSQLite(stockDay,p->getCode(),"cn");
    }

}

int UserMainWnd::findDate(StockDay* stockDay,const QDate& date)
{
    char strDate[12];
    sprintf(strDate,"%04d-%02d-%02d",date.year(),date.month(),date.day());
    return stockDay->findDate(strDate);
}

void UserMainWnd::loadFromTdx(StockDay* stockDay,const StockListItem* p,tdx::TDXMinuteData* pTDXLoader)
{
    //mainStock=p;
    std::vector<PStockDayItem> items;
    pTDXLoader->loadDayData(p,items);
    stockDay->load(items);
    Algorithm::clearStockDay(items);
}
void UserMainWnd::loadFromSQLite(StockDay* stockDay,const QString& code,const QString& head)
{
    //this->code=code;
    std::vector<PStockDayItem> items;
    Algorithm::loadStockDataFromSQLiteForCode(code,items,head);
    stockDay->load(items);
    Algorithm::clearStockDay(items);
}

int UserMainWnd::filterStockPercent(StockDay* stockDay,FilterCompareType compareType,int percent,std::vector<PFilterItemPercent>& out)
{
    //vector<PStockDayItem>::iterator it=items.begin(),end=items.end();
    //vector<PStockDayItem>::iterator prev=items.end();
    int32_t prevIndex=-1;
    float curPercent;
    int iPercent;
    StockFloat* closes=stockDay->getCloses();
    stockfilter::FilterCallback pCallback=stockfilter::getCallback(compareType);
    PFilterItemPercent pFilterItem;
    //for(;it!=end;it++)
    for(int32_t i=0;i<stockDay->getCount();i++)
    {
        if(prevIndex==-1)
        {
            prevIndex=i;
            continue;
        }
        curPercent=closes[i]-closes[prevIndex];
        curPercent/=closes[prevIndex];
        iPercent=(int)(curPercent*100);
        if(pCallback(iPercent,percent))
        {
            pFilterItem=newFilterItemPercent();
            pFilterItem->percent=iPercent;
            pFilterItem->curr=i;
            pFilterItem->prev=prevIndex;
            out.push_back(pFilterItem);
        }
        prevIndex=i;
    }
    return out.size();
}
void UserMainWnd::reCalcDayChartRange(StockDayChart* view,int start,int count)
{
    QList<StatisticIndex*> validate;
    view->getValidateIndex(validate);
    QList<StatisticIndex*>::iterator it=validate.begin(),end=validate.end();
    for(;it!=end;it++)
    {
        if(!(*it)->isFinished())
        {
            (*it)->reCalcIndex();
        }
        (*it)->reCalcRange(start,count);
    }
}
void UserMainWnd::setPriceDate(const QDate& date)
{
    if(priceChgWidget)
        priceChgWidget->setDate(date);
}
void UserMainWnd::createPriceChgDock()
{
    if(priceChgWidget)return ;
    dockPrice=new BaseDockWidget(tr("当日涨跌状态"),this);
    //dockPrice->setWindowTitle();
    priceChgWidget=new StockPriceChgWidget(pDataLoader, dockPrice);

    dockPrice->setWidget(priceChgWidget);
    addDockWidget(Qt::RightDockWidgetArea,dockPrice);
}

void UserMainWnd::getDockWidgets(QList<QDockWidget*>& docks)
{
    docks.append(dockTool);
    docks.append(minuteDock);
    docks.append(dockPrice);
}

const char* UserMainWnd::qssName()const
{
    return "usermainwnd";
}
QWidget* UserMainWnd::getWidget()
{
    return this;
}

void UserMainWnd::getThemingWidget(QList<QWidget*>& wids)const
{
    //wids.push_back(curDayChart);
    wids.push_back(mainWidget);
    wids.push_back(dayChart);
    wids.push_back(minuteDock);
    wids.push_back(dockTool);
    wids.push_back(toolBox);
    wids.push_back(calendarWidget);
    //wids.push_back(mainLayout);

    wids.push_back(dockPrice);
    wids.push_back(priceChgWidget);
    getMainWindowTabBar(wids);
    minuteDock->getThemingWidget(wids);
    priceChgWidget->getThemingWidget(wids);
}
