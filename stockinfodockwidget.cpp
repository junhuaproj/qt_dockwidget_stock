#include "stockinfodockwidget.h"

#include "RecordWidget.h"
#include "dataloader.h"
#include "stockinformation.h"

StockInfoDockWidget::StockInfoDockWidget(data::DataLoader* dataLoader,const QString &title,
    QWidget *parent,Qt::WindowFlags flags)
    :BaseDockWidget(title,parent,flags)
    ,pDataLoader(dataLoader)//,parentList(NULL)
{
    createUI();
}

StockInfoDockWidget::~StockInfoDockWidget()
{

}

void StockInfoDockWidget::getThemingWidget(QList<QWidget*>& wids)
{
    wids.push_back(tabWidget);
    wids.push_back(recordWidget);
    recordWidget->getThemingWidget(wids);
}

void StockInfoDockWidget::createUI()
{
    tabWidget=new QTabWidget(this);
    setWidget(tabWidget);
    textStockInfo=new QPlainTextEdit(tabWidget);
    tabWidget->addTab(textStockInfo,tr("about"));

    recordWidget = new RecordWidget(pDataLoader->getDB(),this);
    tabWidget->addTab(recordWidget,tr("record"));
    connect(recordWidget,SIGNAL(recordItemChanged(const StockListItem*,data::StockRecordItem*)),
            this,SIGNAL(recordItemChanged(const StockListItem*,data::StockRecordItem*)));
}
void StockInfoDockWidget::setStockItem(const StockListItem* pStock)
{
    if(pStock==NULL)return;
    StockInformation info;
    pDataLoader->loadStockInformation(pStock,info);
    textStockInfo->clear();
    QString text;
    info.toHtmlText(text);
    textStockInfo->appendHtml(text);
    recordWidget->setStockItem(pStock);
}
void StockInfoDockWidget::setRecordList(data::StockRecordList* recordList)
{
    recordWidget->setRecordList(recordList);
    //parentList = recordList;
}

void StockInfoDockWidget::removeRecord(int parent)
{
    recordWidget->removeRecord(parent);
}
