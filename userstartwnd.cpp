#include "userstartwnd.h"
#include <QAction>
#include <QMenu>

#include <QStatusBar>
#include <QListWidgetItem>
#include <QHeaderView>
#include "stockrecorditem.h"

#include "simulatetrade.h"
#include "stockfilterdlg.h"
//#include "stockrecorddb.h"
#include "stockrecorddlg.h"
#include "stockrecordlistdlg.h"
//#include "RecordListWidget.h"
#include "RecordWidget.h"
#include "stocksearchwidget.h"
#include "stockinformation.h"
#include "appconfig.h"
#include "AppConfigqss.h"
#include "stockinfodockwidget.h"
#define LIST_DATA_STOCK     (Qt::UserRole+1)

UserStartWnd::UserStartWnd(NetThreadPool* netThreadPool,data::DataLoader* p,QWidget *parent)
    :UserMainWnd(NULL,netThreadPool,p,parent)//,pFilterDlg(NULL)//,curRecordList(NULL)
{
    loadUi();
    connectSignal();
    setWindowTitle("start window");
    //setAccessibleName("UserStartWnd");

}
UserStartWnd::~UserStartWnd()
{
    //if(pFilterDlg)
    //    delete pFilterDlg;
}

void UserStartWnd::loadUi()
{
    UserMainWnd::loadUi();
    loadDockStock();
}
void UserStartWnd::loadDockStock()
{
    //appConfig.qss()->loadQSS(this,qssName());
    dockStock=new QDockWidget(tr("stock search"),this);

    stockWidget= new StockSearchWidget(pNetPool, pDataLoader,dockStock);

    //connect(stockWidget,SIGNAL(addTodayStock(QList)),this,[=](QList){emit onAddTodayStocks(list)});
    connect(stockWidget,SIGNAL(addTodayStock(QList<StockListItem*>)),this,SIGNAL(onAddTodayStocks(QList<StockListItem*>)));
    connect(stockWidget,SIGNAL(newWinCompare(StockListItem*,StockListItem*)),this,SIGNAL(onNewWinCompare(StockListItem*,StockListItem*)));
    // connect(stockWidget,SIGNAL(openRecordList(data::StockRecordList*)),this,SLOT(onOpenRecordList(data::StockRecordList*)));
    connect(stockWidget,SIGNAL(newStockRecordList(QList<StockListItem*>)),this,SLOT(onNewStockRecordList(QList<StockListItem*>)));
    connect(stockWidget,SIGNAL(stockClicked(StockListItem*)),this,SLOT(onStockClicked(StockListItem*)));
    connect(stockWidget,SIGNAL(newStockWinCode(StockListItem*)),this,SIGNAL(onNewWinCode(StockListItem*)));
    connect(stockWidget,SIGNAL(stockDbClicked(StockListItem*)),this,SLOT(onStockDbClick(StockListItem*)));
    //connect(stockWidget,SIGNAL(removeRecordFromList(StockListItem*)),this,SLOT(onRemoveRecordFromList(StockListItem*)));
    connect(stockWidget,SIGNAL(removeRecordFromList(int,int)),this,SLOT(onRemoveRecordFromList(int,int)));
    //connect(stockWidget,SIGNAL(filterApply(StockFilterDlg*)),this,SLOT(onFilterApply(StockFilterDlg*)));

    dockStock->setWidget(stockWidget);


    addDockWidget(Qt::LeftDockWidgetArea,dockStock);


    dockStockInfo=new StockInfoDockWidget(pDataLoader,tr("stock company"));
    addDockWidget(Qt::RightDockWidgetArea,dockStockInfo);
    tabifyDockWidget(minuteDock,dockStockInfo);


    tradeDockWidget=new simulate::SimulateTradeDockWidget(tr("table"),this);
    connect(tradeDockWidget,SIGNAL(tradeChanged(SimulateTradeDockWidget*,SimulateTrade*)),
            this,SLOT(onSimulateTradeChanged(SimulateTradeDockWidget*,SimulateTrade*)));
    connect(tradeDockWidget,SIGNAL(loadSimulateTradeFinished(SimulateTradeDockWidget*)),
            this,SLOT(onLoadTradeFinished(SimulateTradeDockWidget*)));
    addDockWidget(Qt::LeftDockWidgetArea,tradeDockWidget);

    connect(dayChart,SIGNAL(selectRange(ChartWidget*,ChartPos*,ChartPos*,bool)),
            this,SLOT(onDayChartMainSelectedRange(ChartWidget*,ChartPos*,ChartPos*,bool)));

    connect(dayChart,SIGNAL(selectItem(ChartWidget*,ChartPos*)),
            this,SLOT(onDayChartMainSelectItem(ChartWidget*,ChartPos*)));

    tabifyDockWidget(dockStock,tradeDockWidget);

    pchgLevelDockWidget=new StockPChgLevelDockWidget(tr("pchg"),this);
    addDockWidget(Qt::LeftDockWidgetArea,pchgLevelDockWidget);
    tabifyDockWidget(dockStock,pchgLevelDockWidget);

    chartStatusDockWidget=new ChartStatusDockWidget(tr("chart info"),this);
    tabifyDockWidget(dockTool,chartStatusDockWidget);

    connect(minuteDock->getMinuteChart(),SIGNAL(selectItem(ChartWidget*,ChartPos*)),
            this,SLOT(onMinuteChartSelectItem(ChartWidget*,ChartPos*)));
    connect(minuteDock->getMinuteChart(),SIGNAL(selectRange(ChartWidget*,ChartPos*,ChartPos*,bool)),
            this,SLOT(onMinuteChartSelectRange(ChartWidget*,ChartPos*,ChartPos*,bool)));
}

void UserStartWnd::onLoadTradeFinished(simulate::SimulateTradeDockWidget*)
{

}
void UserStartWnd::onMinuteChartSelectItem(ChartWidget* chart,ChartPos* pos)
{
    if(pos==NULL||pos->xIndex==-1)return;
    QString out,detail;
    chart->getItemDescribe(pos,out);
    chart->getItemDescribeEx(pos,detail);
    chartStatusDockWidget->setMinuteChartSelectItem(out.append(detail));
}

void UserStartWnd::onMinuteChartSelectRange(ChartWidget* chart,ChartPos* start,
                                            ChartPos* end,bool changing)
{
    if(changing||start==NULL||end==NULL)return;
    QString out;
    chart->getRangeDescribe(start,end,out);
    //minChart->getMinuteRange(start,end,out);
    chartStatusDockWidget->setMinuteChartSelectRange(out);
}

void UserStartWnd::onDayChartMainSelectItem(ChartWidget* chart,ChartPos* pos)
{
    QString out;
    chart->getItemDescribe(pos,out);
    chartStatusDockWidget->setDayChartSelectItem(out);
}

void UserStartWnd::onDayChartMainSelectedRange(ChartWidget* chart,ChartPos* start,ChartPos* end,bool changing)
{
    if(start&&end)
    {
        //qDebug()<<"start:"<<start->xIndex;
        //qDebug()<<"end:"<<end->xIndex;
        QString out;
        chart->getRangeDescribe(start,end,out);
        if(!changing)
        {
            PStockDate date=stockDay.getDates();
            int startIndex=dayChart->getStartIndex();
            const char* startDate=date[startIndex+start->xIndex].date;
            const char* endDate=date[startIndex+end->xIndex].date;
            QDate date1=QDate::fromString(startDate,appConfig.getDateFormat());
            QDate date2=QDate::fromString(endDate,appConfig.getDateFormat());
            double close=0;
            if(start->xIndex>0)
                close=stockDay.getCloses()[startIndex+start->xIndex-1];
            minuteDock->setStock(mainStock,date1,date2,close,startIndex+end->xIndex+1==stockDay.getCount());
        }
        chartStatusDockWidget->setDayChartSelectRange(out);
        emit currentMessage(out);
        qDebug()<<out;
    }
}
void UserStartWnd::onStockClicked(StockListItem* pStock)
{

    dockStockInfo->setStockItem(pStock);
}

void UserStartWnd::onStockDbClick(StockListItem* p)
{
    mainStock=p;
    loadStock(p,&stockDay,dayChart);
    pchgLevelDockWidget->setStock(mainStock,stockDay.getPchgs(),stockDay.getCount());
    minuteDock->setStock(p,calendarWidget->selectedDate(),0);
    tradeDockWidget->setStockItem(p);
    //recordWidget->setStockItem(p);
}

bool UserStartWnd::isDockVisible(UserStartDock flag)
{
    bool ret=false;
    if(flag&UserStartDock_Stock)
        ret= dockStock->isVisible();
    else if(flag&UserStartDock_Minute)
        ret= minuteDock->isVisible();
    else if(flag&UserStartDock_tool)
        ret= dockTool->isVisible();
    return ret;
}
void UserStartWnd::setDockStockVisible(UserStartDock flag,bool visible)
{
    if(flag&UserStartDock_Stock)
        dockStock->setVisible(visible);
    if(flag&UserStartDock_Minute)
        minuteDock->setVisible(visible);
    if(flag&UserStartDock_tool)
        dockTool->setVisible(visible);
}

void UserStartWnd::recordListChanged(RecordListWidget*,data::StockRecordList* recordList)
{
    //recordWidget->setRecordList(recordList);
    dockStockInfo->setRecordList(recordList);

    if(recordList==NULL)return ;
    stockWidget->setRecordList(recordList);
}


void UserStartWnd::onNewStockRecordList(QList<StockListItem*> stocks)
{
    StockRecordListDlg* dlg=new StockRecordListDlg(this);
    dlg->setStocks(stocks);
    if(dlg->exec()==QDialog::Accepted)
    {
        data::StockRecordList* recordList=new data::StockRecordList();
        recordList->setDatetime(QDateTime::currentDateTime());
        dlg->getRecordList(recordList);

        pDataLoader->getDB()->saveRecordListAndItem(recordList);

        // recordListWidget->setRecordList(recordList);
        stockWidget->setRecordList(recordList);
    }
    delete dlg;
}


void UserStartWnd::dockStockVisibleChanged(bool visible)
{
    emit dockVisibleChanged(UserStartDock_Stock,visible);
}


void UserStartWnd::onSimulateTradeChanged(SimulateTradeDockWidget*,simulate::SimulateTrade* trade)
{
    int index=stockDay.findDate(trade->buyDate);
    if(index>=0)
    {
        if(index>10)
            index-=10;
        dayChart->setStartIndex(index);
    }
}

void UserStartWnd::onRemoveRecordFromList(int listid,int recordid)//StockListItem* p)
{
    dockStockInfo->removeRecord(listid);

}

void UserStartWnd::getDockWidgets(QList<QDockWidget*>& docks)
{
    UserMainWnd::getDockWidgets(docks);
    docks.append(dockStock);
    docks.append(dockStockInfo);
    docks.append(tradeDockWidget);
    docks.append(pchgLevelDockWidget);

}

void UserStartWnd::onRecordItemChanged(const StockListItem* pStock,data::StockRecordItem* pRecord)
{
    stockWidget->updateRecord(pStock,pRecord);
}

void UserStartWnd::getThemingWidget(QList<QWidget*>& wids)const
{
    UserMainWnd::getThemingWidget(wids);
    wids.push_back(dockStock);
    wids.push_back(dockStockInfo);
    wids.push_back(tradeDockWidget);
    //wids.push_back(recordListWidget);
    wids.push_back(chartStatusDockWidget);

    wids.push_back(stockWidget);
    wids.push_back(pchgLevelDockWidget);
    //wids.push_back(listDockWidget);

    stockWidget->getThemingWidget(wids);
    tradeDockWidget->getThemingWidget(wids);
    pchgLevelDockWidget->getThemingWidget(wids);

    dockStockInfo->getThemingWidget(wids);
}

