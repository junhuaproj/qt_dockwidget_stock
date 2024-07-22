#include "userstockcomparewnd.h"

UserStockCompareWnd::UserStockCompareWnd(StockListItem* stock1,NetThreadPool* netThreadPool,data::DataLoader* pDataLoader,StockListItem* stock2,QWidget *parent)
    :UserMainWnd(stock1,netThreadPool,pDataLoader,parent),slaveStock(stock2)
{
    loadUi();
    connectSignal();
    QString title="compare ";
    if(mainStock)
    {
        loadStock(mainStock,&stockDay,dayChart);
        title.append(mainStock->getCode());
        title.append(mainStock->getName());
    }
    if(slaveStock)
    {
        loadStock(slaveStock,&stockDay2,dayChart2);
        title.append(slaveStock->getCode());
        title.append(slaveStock->getName());
    }
    setWindowTitle(title);
}

void UserStockCompareWnd::loadUi()
{
    UserMainWnd::loadUi();
    splitterChart=new QSplitter(Qt::Vertical,this);
    mainLayout->removeWidget(dayChart);
    mainLayout->addWidget(splitterChart);

    dayChart->setParent(splitterChart);
    dayChart2=new chart::StockDayChart(splitterChart);
    splitterChart->addWidget(dayChart);
    splitterChart->addWidget(dayChart2);
    dayChart2->setMouseTracking(true);


    dayChart2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dayChart2,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onDayChart2Context(QPoint)));

    connect(dayChart,SIGNAL(selectDay(int)),
            this,SLOT(onDayChartCurrDay(int)));
    connect(dayChart2,SIGNAL(selectDay(int)),
            this,SLOT(onDayChart2CurrDay(int)));
    //loadChartCfg(dayChart);
    //loadChartCfg(dayChart2);
}

void UserStockCompareWnd::onDayChartCurrDay(int index)
{
    int startIndex=dayChart->getStartIndex();
    int index2=stockDay2.findDate(stockDay.getDates()[startIndex].date);
    if(index2!=-1)//先把第一个显示的日期调成一致
        dayChart2->setStartIndex(index2);
    index2=stockDay2.findDate(stockDay.getDates()[index].date);
    if(index2>=0)
    {
        dayChart2->setCurIndex(index2);
    }
}
void UserStockCompareWnd::onDayChart2CurrDay(int)
{

}

void UserStockCompareWnd::onDayChart2Context(QPoint)
{
    this->showDayChartContextMenu(dayChart2);
}
void UserStockCompareWnd::setMainStock(StockListItem* p)
{
    mainStock=p;
    loadStock(mainStock,&stockDay,dayChart);
}
void UserStockCompareWnd::setSlaveStock(StockListItem* p)
{
    slaveStock=p;
    loadStock(slaveStock,&stockDay2,dayChart2);
}

void UserStockCompareWnd::connectSignal()
{
    UserMainWnd::connectSignal();
    connect(calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(onCalendarChart2(const QDate)));
    connect(dayChart,SIGNAL(viewDataChanged(StockDayChart*,int,int)),this,SLOT(onDayChartChanged(StockDayChart*,int,int)));
    connect(dayChart2,SIGNAL(viewDataChanged(StockDayChart*,int,int)),this,SLOT(onDayChartChanged(StockDayChart*,int,int)));
}

void UserStockCompareWnd::onCalendarChart2(const QDate date)
{
    int index=findDate(&stockDay2, date);
    if(index>=0)
        dayChart2->setStartIndex(index);
}


const StockListItem* UserStockCompareWnd::getMainStock()const
{
    return mainStock;
}
const StockListItem* UserStockCompareWnd::getSlaveStock()const
{
    return slaveStock;
}
void UserStockCompareWnd::onDayChartChanged(chart::StockDayChart* view,int start,int count)
{
    if(view==dayChart)
    {
        if(stockDay.getCount()<=0)return;
        reCalcDayChartRange(view,start,count);
    }
    else
    {
        if(stockDay2.getCount()<=0)return;
        reCalcDayChartRange(view,start,count);
    }
}
