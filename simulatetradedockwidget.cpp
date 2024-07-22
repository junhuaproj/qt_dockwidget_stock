#include "simulatetradedockwidget.h"
#include "stocklistitem.h"
#include "simulatetradeloader.h"
#include "simulatetrade.h"
//#include "AppConfigqss.h"
#include "appconfig.h"
#include "simulatetradeeditdlg.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

namespace simulate{
#define TABLE_TRADE_DATA    (Qt::UserRole+4)

SimulateTradeDockWidget::SimulateTradeDockWidget(const QString& title,QWidget *parent)
    : BaseDockWidget(title,parent),tradeLoader(NULL),tradeEditDlg(NULL)
{
    widgetMain=new QWidget(this);
    setWidget(widgetMain);
    layoutMain=new QVBoxLayout(widgetMain);
    tableWidget=new QTableWidget(widgetMain);
    widgetMain->setLayout(layoutMain);
    layoutMain->addWidget(tableWidget);
    layoutMain->setSpacing(0);


    layoutBtn=new QHBoxLayout(widgetMain);
    cbxTradeType=new QComboBox(widgetMain);
    layoutBtn->addWidget(cbxTradeType);
    layoutMain->addLayout(layoutBtn);


    tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    QStringList horizontalTitle;
    horizontalTitle<<tr("buydate")<<tr("buyprice")<<tr("selldate")<<tr("sellprice")<<tr("diff")<<tr("profit%");
    tableWidget->setColumnCount(horizontalTitle.size());
    tableWidget->setHorizontalHeaderLabels(horizontalTitle);
    tableWidget->setColumnWidth(0,90);
    tableWidget->setColumnWidth(1,60);
    tableWidget->setColumnWidth(2,90);
    tableWidget->setColumnWidth(3,60);
    tableWidget->setColumnWidth(4,50);
    tableWidget->setColumnWidth(5,60);
    btnSaveTrade=new QPushButton(tr("save trades"),widgetMain);
    //layoutMain->addWidget(btnSaveTrade);
    layoutBtn->addWidget(btnSaveTrade);

    connect(btnSaveTrade,SIGNAL(clicked(bool)),this,SLOT(onSaveTrades(bool)));
    connect(tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(onCurrentTradeChanged(int,int,int,int)));
    connect(tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onEditTrade(int,int)));
    //qss.loadQSS(tableWidget,"tableview");

    const QList<HttpAddressItem*>* tradeAddress=appConfig.getTradeAddress();
    for(int i=0;i<tradeAddress->size();i++)
    {
        const HttpAddressItem* item=tradeAddress->at(i);
        cbxTradeType->addItem(item->name,QVariant((uint64_t)item));
    }

}

SimulateTradeDockWidget::~SimulateTradeDockWidget()
{
    clearTrade();
    if(tradeLoader)
        delete tradeLoader;
    if(tradeEditDlg)
        delete tradeEditDlg;
}
void SimulateTradeDockWidget::getThemingWidget(QList<QWidget*>& wids)
{
    wids.push_back(tableWidget);
    wids.push_back(btnSaveTrade);
    wids.push_back(cbxTradeType);
}

void SimulateTradeDockWidget::onSaveTrades(bool)
{
    QJsonObject root;
    root.insert("lastdate",
                QJsonValue(QDateTime::currentDateTime().toString(datetime_format)));
    QJsonArray arr;
    QList<SimulateTrade*>::const_iterator it= trades.begin(),end=trades.end();
    for(;it!=end;it++)
    {
        QJsonObject trade;
        trade.insert("buydate",(*it)->buyDate);
        trade.insert("buyprice",(*it)->buyPrice);

        trade.insert("selldate",(*it)->sellDate);
        trade.insert("sellprice",(*it)->sellPrice);
        trade.insert("note",(*it)->note);
        arr.append(trade);
    }
    root.insert("trades",arr);

    QString path;
    appConfig.getSimulateTradePath(pStock->getCode(),path);
    QFile f(path);
    if(f.open(QFile::WriteOnly))
    {
        QJsonDocument doc(root);
        f.write(doc.toJson());
        f.close();
    }
}

void SimulateTradeDockWidget::onTradeDelete(TradeEditDlg* dlg,int row,SimulateTrade* trade)
{
    dlg->hide();
    tableWidget->removeRow(row);
    trades.removeOne(trade);
}

void SimulateTradeDockWidget::onTradeSave(TradeEditDlg* dlg,int row,SimulateTrade* trade)
{
    dlg->hide();
    setTableRow(row,trade);
}

void SimulateTradeDockWidget::onEditTrade(int row,int)
{
    QTableWidgetItem* pCell=tableWidget->item(row,0);
    if(pCell==NULL)return;
    QVariant var=pCell->data(TABLE_TRADE_DATA);
    SimulateTrade* pTrade=(SimulateTrade*)qvariant_cast<uint64_t>(var);
    if(pTrade==NULL)return ;
    if(tradeEditDlg==NULL)
    {
        tradeEditDlg=new TradeEditDlg(this);
        connect(tradeEditDlg,SIGNAL(tradeDelete(TradeEditDlg*,int,SimulateTrade*)),this,SLOT(onTradeDelete(TradeEditDlg*,int,SimulateTrade*)));
        connect(tradeEditDlg,SIGNAL(tradeSave(TradeEditDlg*,int,SimulateTrade*)),
                this,SLOT(onTradeSave(TradeEditDlg*,int,SimulateTrade*)));
    }

    tradeEditDlg->setTrade(row,pTrade);
    tradeEditDlg->show();
}
void SimulateTradeDockWidget::onCurrentTradeChanged(int row,int,int,int)
{
    QTableWidgetItem* pCell=tableWidget->item(row,0);
    if(pCell==NULL)return ;
    QVariant var= pCell->data(TABLE_TRADE_DATA);
    SimulateTrade* pTrade=(SimulateTrade*)qvariant_cast<uint64_t>(var);
    emit tradeChanged(this,pTrade);
}

void SimulateTradeDockWidget::setStockItem(StockListItem* pStock)
{
    if(tradeLoader)
        delete tradeLoader;
    this->pStock=pStock;
    //先设置行数0，防止发送无效的trade
    tableWidget->setRowCount(0);
    clearTrade();

    QVariant var=cbxTradeType->currentData();
    const HttpAddressItem* address=(HttpAddressItem*)qvariant_cast<uint64_t>(var);
    ///index/kdj/testtrade/<code>/<float:buyp>/<float:sellp>"
    //QString url=QString("/index/kdj/testtrade/%1/0.0/100.0").arg(pStock->getCode());
    QString url=QString(address->address).arg(pStock->getCode());
    QString lastDate;
    if(loadLocalTrades(lastDate))
    {
        url.append(QString("/%1").arg(lastDate));
    }
    tradeLoader=new SimulateTradeLoader();

    tradeLoader->requestUrl(url);
    connect(tradeLoader,SIGNAL(requestResult(SimulateTradeLoader*,bool)),this,SLOT(onRequest(SimulateTradeLoader*,bool)));
}
const QList<SimulateTrade*>* SimulateTradeDockWidget::getTrades()const
{
    return &trades;
}


bool SimulateTradeDockWidget::loadLocalTrades(QString& lastDate)
{
    QString path;
    appConfig.getSimulateTradePath(pStock->getCode(),path);
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        QJsonDocument doc=QJsonDocument::fromJson(f.readAll());
        f.close();
        QJsonObject root=doc.object();
        lastDate=root.value("lastdate").toString();
        //lastDate=QDateTime::fromString(datetime,datetime_format);
        QJsonArray trades=root.value(tr("trades")).toArray();
        loadTrades(trades);
        onTradesUpdated();
        emit loadSimulateTradeFinished(this);
        return true;
    }
    return false;
}

int SimulateTradeDockWidget::loadTrades(const QJsonArray& arr)
{
    int count=arr.size();
    SimulateTrade* trade;
    for(int i=0;i<count;i++)
    {
        QJsonObject obj=arr.at(i).toObject();
        trade=new SimulateTrade();
        trade->buyPrice=obj.value("buyprice").toDouble();
        trade->buyDate= obj.value("buydate").toString();
        trade->sellPrice= obj.value("sellprice").toDouble();
        trade->sellDate=obj.value("selldate").toString();
        trade->note=obj.value("note").toString();
        trades.append(trade);
    }
    return count;
}

void SimulateTradeDockWidget::onRequest(SimulateTradeLoader* pLoader,bool bOK)
{
    if(!bOK)
    {
        return ;
    }
    const QByteArray* data=pLoader->getData();
    if(data->length()==0)return ;

    QJsonDocument doc=QJsonDocument::fromJson(*data);

    int start=trades.size();
    int row=loadTrades(doc.array());
    if(row>0)
    {
        onTradesUpdated(start);
        emit loadSimulateTradeFinished(this);
    }
    if( pLoader==tradeLoader)
    {
        tradeLoader->deleteLater();
        tradeLoader=NULL;
    }
}
void SimulateTradeDockWidget::onTradesUpdated(int start)
{
    QList<SimulateTrade*>::iterator it=trades.begin(),end=trades.end();

    tableWidget->setRowCount(trades.size());
    it+=start;
    for(int row=start;it!=end;it++,row++)
    {
        setTableRow(row,*it);
    }

}
void SimulateTradeDockWidget::setTableRow(int row,SimulateTrade* trade)
{
    QTableWidgetItem* item;
    item=tableWidget->item(row,0);

    if(item==NULL)
    {
        item=new QTableWidgetItem(trade->buyDate);
        tableWidget->setItem(row,0,item);
    }
    else
        item->setText(trade->buyDate);
    item->setData(TABLE_TRADE_DATA,QVariant((uint64_t)trade));


    item=tableWidget->item(row,1);
    if(item==NULL)
    {
        item=new QTableWidgetItem(QString("%1").arg(trade->buyPrice,0,'f',2));
        tableWidget->setItem(row,1,item);
    }
    else
        item->setText(QString("%1").arg(trade->buyPrice,0,'f',2));

    item=tableWidget->item(row,2);
    if(item==NULL)
    {
        item=new QTableWidgetItem(trade->sellDate);
        tableWidget->setItem(row,2,item);
    }
    else
        item->setText(trade->sellDate);

    item=tableWidget->item(row,3);
    if(item==NULL)
    {
        item=new QTableWidgetItem(QString("%1").arg(trade->sellPrice,0,'f',2));
        tableWidget->setItem(row,3,item);
    }
    else
        item->setText(QString("%1").arg(trade->sellPrice,0,'f',2));

    item=tableWidget->item(row,4);
    if(item==NULL)
    {
        item=new QTableWidgetItem(QString("%1").arg(trade->getProfit(),0,'f',2));
        tableWidget->setItem(row,4,item);
    }
    else
        item->setText(QString("%1").arg(trade->getProfit(),0,'f',2));

    item=tableWidget->item(row,5);
    QString percent=QString("%1%").arg(trade->getProfit()*100/trade->buyPrice,0,'f',2);
    if(item==NULL)
    {
        item=new QTableWidgetItem(percent);
        tableWidget->setItem(row,5,item);
    }
    else
    {
        item->setText(percent);
    }

}
void SimulateTradeDockWidget::clearTrade()
{
    while(!trades.isEmpty())
    {
        delete trades.back();
        trades.pop_back();
    }
}
}
