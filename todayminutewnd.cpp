/**
 * 显示多股分钟线
*/

#include "todayminutewnd.h"
#include <QDebug>
#include "dataloader.h"
#include "tdxminutedata.h"
//#include <openssl/aes.h>
#include <QMenu>
#include <QVariant>
#include "todayminutecfgdlg.h"
#include "stocklaststatus.h"
#include <QHeaderView>

#include "bg/netthreadpool.h"
//#include <functional>
//#define QUICK_TIME_OUT  100

enum TodayColIndex
{
    Col_Code,
    Col_Name,
    Col_Price,
    Col_Pchg,
    Col_Chg,
    Col_Vol,
    //Col_CurVol,
    //Col_Fund,
    Col_Change,
    Col_Rate,
    Col_High,
    Col_Low,Col_PreClose,
    Col_Open,
    Col_Count
};


TodayMinuteWnd::TodayMinuteWnd(NetThreadPool* netThreadPool,data::DataLoader* dataLoader,QWidget *parent)
    : BaseMainWindow(netThreadPool,dataLoader,parent),updateChartIndex(-1)
    ,chartStockIndex(0)
    //,pDataLoader(dataLoader)
    //,netMinuteLoader(NULL)
{
    loadUI();
    onCfgChanged();


    connect(pNetPool,SIGNAL(dayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*)),
            this,SLOT(onDayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*)));

    connect(pNetPool,SIGNAL(lastStatusChanged(const QList<StockLastStatus*>&)),
            this,SLOT(onLastStatusChanged(const QList<StockLastStatus*>&)));
    loadStocks();

}


TodayMinuteWnd::~TodayMinuteWnd()
{
    //delete netMinuteLoader;
    //从后台从删除
    pNetPool->removeStock(stocks,UPDATE_MINUTE|UPDATE_PRICE,WND_FALG_TODAY);
}

void TodayMinuteWnd::loadStocks()
{
    //读取保存的股票编号
    QStringList stockCodes;
    pDataLoader->loadTodayStocks(stockCodes);
    QList<StockListItem*> stocksSaved;
    //读取对应的StockListItem*
    pDataLoader->findStockList(stockCodes,&stocksSaved);
    addStocks(stocksSaved);

}

/*
 *      |grid 1
 * table|grid 2
 *      |buttons
*/
void TodayMinuteWnd::loadUI()
{
    splitter=new QSplitter(Qt::Horizontal,this);

    tableWidget=new QTableWidget(this);

    chartWidget=new QWidget(splitter);

    loadUIRight();

    splitter->addWidget(tableWidget);

    splitter->addWidget(chartWidget);
    this->setCentralWidget(splitter);
    //mainLayout->addWidget(tableWidget);

    QStringList headers;
    headers<<tr("code")<<tr("name");
        //编号，名称，价格，涨跌幅，涨跌额，成交量，现手，
        //金额，换手，量比，最高，最低，市盈率，昨收，今开
    headers << tr("price") << tr("pchg") << tr("chg") << tr("vol");// << tr("现手");
    //headers<<"价格"<<"涨跌幅"<<"涨跌额"<<"成交量"<<"现手";
    //headers << tr("fund") << tr("chang") << tr("rate") << tr("high") << tr("low") << tr("pre close") << tr("open");
    headers << tr("change") << tr("rate") << tr("high") << tr("low") << tr("pre close") << tr("open");
//    headers<<"金额"<<"换手"<<"量比"<<"最高"<<"最低"//<<"市盈率"
  //          <<"昨收"<<"今开";
    tableWidget->setColumnCount(headers.size());
    tableWidget->setColumnWidth(0,80);
    tableWidget->setColumnWidth(1,90);
    for(int i=2;i<14;i++)
        tableWidget->setColumnWidth(i,80);
    tableWidget->setHorizontalHeaderLabels(headers);
    //tableWidget->setSelectionMode(QTableWidget::SingleSelection);
    tableWidget->setSelectionBehavior(QTableWidget::SelectRows);

    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onStockContext(QPoint)));
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onTableCellDoubleClicked(int,int)));
    tableWidget->setDragEnabled(true);
}
void TodayMinuteWnd::getDockWidgets(QList<QDockWidget*>& docks)
{

}

const char* TodayMinuteWnd::qssName()const
{
    return "todayminutewnd";
}

QWidget* TodayMinuteWnd::getWidget()
{
    return this;
}

void TodayMinuteWnd::getThemingWidget(QList<QWidget*>& wids)const
{
    //BaseMainWindow::getThemingWidget(wids);
    wids.push_back(splitter);
    wids.push_back(tableWidget);
    wids.push_back(chartWidget);
    wids.push_back(btnNextPage);
    wids.push_back(btnPrevPage);
    wids.push_back(btnCfgPage);
    wids.push_back(lblSummary);
    wids.push_back(btnSave);
    QList<minutechart::StockMinuteChart*>::const_iterator it= minuteCharts.begin(),
        end=minuteCharts.end();
    for(;it!=end;it++)
        wids.push_back(*it);
}

//股票列表的右键菜单
void TodayMinuteWnd::onStockContext(QPoint)
{
    QAction* actionRemoveStock=new QAction("Remove Stock");

    //QAction* actionAddToDay=new QAction("Add To Today");

    QMenu* menu=new QMenu();

    connect(actionRemoveStock,SIGNAL(triggered(bool)),this,SLOT(onActionRemoveStock(bool)));

    menu->addAction(actionRemoveStock);

    menu->exec(QCursor::pos());
}
void TodayMinuteWnd::onTableCellDoubleClicked(int row,int col)
{
    //双击一个股票
    QTableWidgetItem* pItem= tableWidget->item(row,0);

    StockListItem* pStockItem=getStockFromTableItem(pItem);
    minutechart::StockMinuteChart* chartView= findChart(pStockItem);
    if(chartView==NULL)//判断是否已经在显示
    {
        //没有显示时，找一个空闲的，没有空闲时不处理
        chartView=findEmptyChart();
        if(chartView!=NULL)
        {
            chartView->setStockItem(pStockItem);
        }
    }
}
void TodayMinuteWnd::onActionRemoveStock(bool)
{
    QList<QTableWidgetItem*> items= tableWidget->selectedItems();
    QList<QTableWidgetItem*>::iterator it=items.begin(),end=items.end();

    QList<QTableWidgetItem*> selectedStock;
    QTableWidgetItem* pTableItem;
    QVariant var;
    StockListItem* pStockItem;
    for(;it!=end;it++)
    {
        pTableItem=*it;
        var=pTableItem->data(TABLE_DATA_STOCK);
        //一行有多个Item,每行只要第一列的Item
        if(qvariant_cast<uint64_t>(var))
            selectedStock.append(pTableItem);
    }
    //it=selectedStock.begin();
    //end=selectedStock.end();
    //先从后面开始删除，前面的行索引不会变
    while(!selectedStock.isEmpty())
    {
        pTableItem=selectedStock.back();

        pStockItem=getStockFromTableItem(pTableItem);
        onStockRemoved(pStockItem);//删除其他相关
        removeStockCode(pStockItem);//从stocks删除
        tableWidget->removeRow(pTableItem->row());//从Table里删除
        selectedStock.pop_back();
    }
}
StockListItem* TodayMinuteWnd::getStockFromTableItem(QTableWidgetItem* pItem)
{
    QVariant var=pItem->data(TABLE_DATA_STOCK);
    return (StockListItem*)qvariant_cast<uint64_t>(var);
}

void TodayMinuteWnd::onStockRemoved(StockListItem* pItem)
{
    minutechart::StockMinuteChart* chartView= findChart(pItem);
    pNetPool->removeStock(pItem,UPDATE_MINUTE|UPDATE_PRICE,WND_FALG_TODAY);
    if(chartView==NULL)return;
    //chartView->reset();
    chartView->setStockItem(NULL);
    chartView->repaint();
}

void TodayMinuteWnd::loadUIRight()
{
    gridLayout=new QGridLayout();

    vBoxLayout=new QVBoxLayout(chartWidget);

    hBoxBottomLayout=new QHBoxLayout();

    lblSummary=new QLabel("summary",chartWidget);
    btnCfgPage=new QPushButton(tr("config"),chartWidget);
    btnPrevPage=new QPushButton(tr("prev page"),chartWidget);
    btnNextPage=new QPushButton(tr("next page"),chartWidget);
    btnSave=new QPushButton(tr("save"),chartWidget);
    hBoxBottomLayout->addWidget(lblSummary,1);
    hBoxBottomLayout->addWidget(btnSave,1);
    hBoxBottomLayout->addWidget(btnCfgPage,1);
    hBoxBottomLayout->addWidget(btnPrevPage,1);
    hBoxBottomLayout->addWidget(btnNextPage,1);

    vBoxLayout->addLayout(gridLayout,1);
    vBoxLayout->addLayout(hBoxBottomLayout,1);
    chartWidget->setLayout(vBoxLayout);

    connect(btnSave,SIGNAL(clicked(bool)),this,SLOT(onBtnSave(bool)));
    connect(btnPrevPage,SIGNAL(clicked(bool)),this,SLOT(onBtnPrevPage(bool)));
    connect(btnNextPage,SIGNAL(clicked(bool)),this,SLOT(onBtnNextPage(bool)));
    connect(btnCfgPage,SIGNAL(clicked(bool)),this,SLOT(onBtnCfgPage(bool)));
}
void TodayMinuteWnd::onBtnPrevPage(bool)
{
    if(chartStockIndex==0)return;
    int count=todayCfg.getColumns()*todayCfg.getRows();
    if(chartStockIndex>count)
        chartStockIndex-=count;
    else chartStockIndex=0;
    showStockChart(chartStockIndex);
}
void TodayMinuteWnd::onBtnNextPage(bool)
{
    int count=todayCfg.getColumns()*todayCfg.getRows();
    if(chartStockIndex+count<stocks.size())
    {
        chartStockIndex+=count;
        showStockChart(chartStockIndex);
    }
}
void TodayMinuteWnd::onBtnCfgPage(bool)
{
    TodayMinuteCfgDlg* dlg=new TodayMinuteCfgDlg(&todayCfg,this);
    if(dlg->exec()==QDialog::Accepted)
    {
        onCfgChanged();
        //timerUpdate.setInterval(todayCfg.getUpdateTimeout());
    }
    delete dlg;
}

void TodayMinuteWnd::onBtnSave(bool)
{
    pDataLoader->saveTodayStocks(stocks);
}

void TodayMinuteWnd::onStockChanged()
{

}
void TodayMinuteWnd::onCfgChanged()
{
    clearChartsFromLayout();
    int chartCount=todayCfg.getColumns()*todayCfg.getRows();
    if(minuteCharts.size()>chartCount)
    {
        //删除多余的
        while(minuteCharts.size()>chartCount)
        {
            delete minuteCharts.back();
            minuteCharts.pop_back();
        }
    }
    else
    {
        minutechart::StockMinuteChart* chart;
        while(minuteCharts.size()<chartCount)
        {
            chart=new minutechart::StockMinuteChart(chartWidget);
            chart->setAcceptDrops(true);
            //StockChartView::addOneDayXLabel(chart,0);
            chart->setMouseTracking(true);
            minuteCharts.push_back(chart);


        }
    }
    QList<minutechart::StockMinuteChart*>::iterator it=minuteCharts.begin(),end=minuteCharts.end();
    for(uint32_t col=0;col<todayCfg.getColumns();col++)
    {
        for(int row=0;row<todayCfg.getRows()&&it!=end;row++,it++)
        {
            gridLayout->addWidget(*it,row,col,1,1);
            connect(*it,SIGNAL(selectItem(ChartWidget*,ChartPos*)),this,SLOT(onSelectItem(ChartWidget*,ChartPos*)));
            connect(*it,SIGNAL(selectRange(ChartWidget*,ChartPos*,ChartPos*,bool)),
                    this,SLOT(onSelectRange(ChartWidget*,ChartPos*,ChartPos*,bool)));

        }
    }
    chartStockIndex=0;
    showStockChart(chartStockIndex);
}

void TodayMinuteWnd::onSelectItem(ChartWidget* chart,ChartPos* curPos)
{
    if(curPos==NULL||curPos->xIndex==-1)return;
    minutechart::StockMinuteChart* minuteChart=(minutechart::StockMinuteChart*)chart;
    QString out,detail;
    minuteChart->getItemDescribe(curPos,out);
    minuteChart->getItemDescribeEx(curPos,detail);
    emit currentMessage(out+detail);
}
void TodayMinuteWnd::onSelectRange(ChartWidget* chart,ChartPos* start,ChartPos* end,bool changing)
{
    if(changing||start==NULL||end==NULL)return;
    minutechart::StockMinuteChart* minuteChart=(minutechart::StockMinuteChart*)chart;
    QString out;
    minuteChart->getRangeDescribe(start,end,out);
    emit currentMessage(out);
}

void TodayMinuteWnd::clearChartsFromLayout()
{
    QList<minutechart::StockMinuteChart*>::iterator it=minuteCharts.begin(),end=minuteCharts.end();
    for(;it!=end;it++)
        gridLayout->removeWidget(*it);
}
void TodayMinuteWnd::showStockChart(int firstIndex)
{
    //chartStockIndex=firstIndex;
    QList<minutechart::StockMinuteChart*>::iterator it=minuteCharts.begin(),end=minuteCharts.end();
    uint32_t index=firstIndex;
    minutechart::StockMinuteChart* chart;
    //timerUpdate.stop();
    StockListItem* stock;
    minutechart::StockDayMinute* pDay;
    for(;it!=end&&index<stocks.size();it++,index++)
    {
        chart=*it;
        stock=stocks.at(index);
        chart->setStockItem(stock);

        pDay=pNetPool->getStockDayMinute(stock);
        if(pDay)
        {
            chart->addDay(pDay);
            chart->reCalcRange();
        }
        chart->repaint();
        //chart->addDay(chart->newStockDayMinute(SOHU_FILL_COUNT));
    }
}

void TodayMinuteWnd::addStock(StockListItem* pItem)//,bool checkUpdate)
{
    //removeStockCode(pItem);
    if(isStockExist(pItem))return;
    int index=stocks.size();
    stocks.append(pItem);
    tableWidget->setRowCount(stocks.size());

    QTableWidgetItem* item=new QTableWidgetItem(pItem->getCode());
    item->setData(TABLE_DATA_STOCK,QVariant((uint64_t)pItem));
    tableWidget->setItem(index,0,item);

    item=new QTableWidgetItem(pItem->getName());
    tableWidget->setItem(index,1,item);
    StockLastStatus* lastStatus=pNetPool->findStockListStatus(pItem);
    if(lastStatus)
        setTableStatus(index,lastStatus);
    minutechart::StockMinuteChart* pChartView= findEmptyChart();
    pNetPool->addStock(pItem,UPDATE_MINUTE|UPDATE_PRICE,WND_FALG_TODAY);
    if(pChartView)
    {
        pChartView->setStockItem(pItem);
        //从后台中找到
        pChartView->addDay(pNetPool->getStockDayMinute(pItem));
    }
}

bool TodayMinuteWnd::isStockExist(StockListItem* pItem)
{
    QList<StockListItem*>::const_iterator it= std::find(stocks.constBegin(),stocks.constEnd(),pItem);
    return it!=stocks.end();
}
void TodayMinuteWnd::removeStockCode(StockListItem* pItem)
{
    QList<StockListItem*>::const_iterator it= std::find(stocks.constBegin(),stocks.constEnd(),pItem);
    if(it!=stocks.end())
        stocks.erase(it);
}

void TodayMinuteWnd::addStocks(QList<StockListItem*>& stocks)
{
    QList<StockListItem*>::iterator it=stocks.begin(),end=stocks.end();
    //int index=this->stocks.size();
    for(;it!=end;it++)
        addStock(*it);

}

void TodayMinuteWnd::onDayMinuteUpdated(const StockListItem* stock,
                                        minutechart::StockDayMinute* pDay)
{
    minutechart::StockMinuteChart* pChart=findChart(stock);
    if(pChart==NULL)return ;
    minutechart::StockDayMinute* pOldDay=pChart->lastDay();
    bool bSame=pOldDay==pDay;
    if(!bSame)
        pChart->addDay(pDay);
    pChart->reCalcRange();
    pChart->repaint();


}

minutechart::StockMinuteChart* TodayMinuteWnd::findChart(const StockListItem* pItem)
{
    QList<minutechart::StockMinuteChart*>::iterator it=minuteCharts.begin(),end=minuteCharts.end();
    const StockListItem* pChartStockItem;
    for(;it!=end;it++)
    {
        pChartStockItem=(*it)->getStockItem();
        if(pChartStockItem==NULL)return NULL;
        if(pChartStockItem->getId()==pItem->getId())
        //if(pChartStockItem->getCode()==pItem->getCode())
        {
            return *it;
        }
    }
    return NULL;
}
minutechart::StockMinuteChart* TodayMinuteWnd::findEmptyChart()
{
    QList<minutechart::StockMinuteChart*>::iterator it=minuteCharts.begin(),end=minuteCharts.end();
    //const StockListItem* pChartStockItem;
    for(;it!=end;it++)
    {
        if(!(*it)->isStock())
        {
            return *it;
        }
    }
    return NULL;
}

void TodayMinuteWnd::setTableStatus(int row,StockLastStatus* stockInfo)
{
    QString itemText;
    QTableWidgetItem* pItem=tableWidget->item(row,Col_Price);
    itemText=QString("%1").arg(stockInfo->getPrice(),0,'f',2);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);
        tableWidget->setItem(row,Col_Price,pItem);
    }
    else
        pItem->setText(itemText);

    QColor color;
    //const char* itemStyle=NULL;

    int priceStatus=(int)((stockInfo->getPrice()*100)-(stockInfo->getPreClose()*100));
    //stockInfo->getPriceStatus();
    if(priceStatus<0)
        //itemStyle=itemStyleGreen;
        color=Qt::green;
    else if(priceStatus>0)
        //itemStyle=itemStyleRed;
        color=Qt::red;
    else
    {

        color=this->palette().color(QPalette::Text);//qvariant_cast<QColor>(var);
        //color=Qt::black;
    }
    pItem->setForeground(color);

    pItem=tableWidget->item(row,Col_Pchg);
    itemText=QString("%1").arg(stockInfo->getChangedPer());
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);// getChangedPercent());
        tableWidget->setItem(row,Col_Pchg,pItem);
    }
    else
    {
        pItem->setText(itemText);
    }

    pItem=tableWidget->item(row,Col_Chg);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(stockInfo->getChangedValue());
        tableWidget->setItem(row,Col_Chg,pItem);
    }
    else
        pItem->setText(stockInfo->getChangedValue());
    //pItem->setForeground(foreBrush);
    pItem->setForeground(color);


    ///////////
    pItem=tableWidget->item(row,Col_Vol);
    itemText=QString("%1").arg(stockInfo->getVolume());
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(QString("%1").arg(stockInfo->getVolume()));
        tableWidget->setItem(row,Col_Vol,pItem);
    }
    else
        pItem->setText(itemText);


    pItem=tableWidget->item(row,Col_Change);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(stockInfo->getChange());
        tableWidget->setItem(row,Col_Change,pItem);
    }
    else
    {
        pItem->setText(stockInfo->getChange());
    }
    pItem=tableWidget->item(row,Col_Rate);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(stockInfo->getVolRate());
        tableWidget->setItem(row,Col_Rate,pItem);
    }
    else
    {
        pItem->setText(stockInfo->getVolRate());
    }
    pItem=tableWidget->item(row,Col_High);
    itemText=QString("%1").arg(stockInfo->getHigh(),0,'f',2);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);
        tableWidget->setItem(row,Col_High,pItem);
    }
    else
        pItem->setText(itemText);

    pItem=tableWidget->item(row,Col_Low);
    itemText=QString("%1").arg(stockInfo->getLow(),0,'f',2);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);
        tableWidget->setItem(row,Col_Low,pItem);
    }
    else
    {
        pItem->setText(itemText);
    }

    pItem=tableWidget->item(row,Col_PreClose);
    itemText=QString("%1").arg(stockInfo->getPreClose(),0,'f',2);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);
        tableWidget->setItem(row,Col_PreClose,pItem);
    }
    else
        pItem->setText(itemText);

    pItem=tableWidget->item(row,Col_Open);
    itemText=QString("%1").arg(stockInfo->getOpen(),0,'f',2);
    if(pItem==NULL)
    {
        pItem=new QTableWidgetItem(itemText);
        tableWidget->setItem(row,Col_Open,pItem);
    }
    else
        pItem->setText(itemText);
}

void TodayMinuteWnd::onLastStatusChanged(const QList<StockLastStatus*>& status)
{
    int rowCount=tableWidget->rowCount();
    QTableWidgetItem* pItem;
    StockListItem* pStock;

    //StockLastStatus* stockInfo;
    QList<StockLastStatus*>::const_pointer it;
    for(int i=0;i<rowCount;i++)
    {
        pItem=tableWidget->item(i,0);
        pStock=getStockFromTableItem(pItem);

        it=findLastStatus(status,pStock);
        //if(pStock->getCode().compare(stockInfo->getCode().mid(3))==0)
        if(it!=status.end())
        //if(pStock==stockInfo->getPStock())
        {
            //stockInfo=*it;
            setTableStatus(i,*it);
            //break;
        }
    }
}

