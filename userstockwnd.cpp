#include "userstockwnd.h"

#include "filterstockbypercentdlg.h"
#include "filterdata.h"
#include <QHeaderView>
UserStockWnd::UserStockWnd(StockListItem* pItem,NetThreadPool* netThreadPool,data::DataLoader* p,QWidget *parent)
    :UserMainWnd(pItem,netThreadPool,p,parent)//,pStockItem(pItem)
{
    loadUi();
    connectSignal();
    //loadChartCfg(dayChart);
    loadStock(mainStock,&stockDay,dayChart);

}


void UserStockWnd::loadUi()
{
    UserMainWnd::loadUi();
    calcPage1=new QWidget(toolBox);
    calcPage1Layout=new QGridLayout(calcPage1);
    btnPercentChanged=new QPushButton(tr("day percent"),calcPage1);

    calcPage1Layout->addWidget(btnPercentChanged,0,0);
    calcPage1->setLayout(calcPage1Layout);
    toolBox->addItem(calcPage1,tr("statistic"));

    tableWidgetResult=new QTableWidget(0,3,toolBox);

    toolBox->addItem(tableWidgetResult,tr("result list"));

    tableWidgetResult->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    connect(tableWidgetResult,SIGNAL(cellClicked(int,int)),this,SLOT(onCellActivated(int,int)));
    //minChart->setTitleVisible(false);

    connect(dayChart,SIGNAL(viewDataChanged(StockDayChart*,int,int)),this,SLOT(onDayChartChanged(chart::StockDayChart*,int,int)));
    //loadChartCfg(dayChart);
}

void UserStockWnd::onCellActivated(int row,int column)
{
    //qDebug()<<row<<"col"<<column;
    QTableWidgetItem* item=tableWidgetResult->item(row,0);
    //QDate date=QDate::fromString(item->text(),"yyyy-MM-dd");
    int index=stockDay.findDate(item->text());
    if(index>=0)
        dayChart->setStartIndex(index);
}

void UserStockWnd::connectSignal()
{
    UserMainWnd::connectSignal();
    connect(btnPercentChanged,SIGNAL(clicked(bool)),this,SLOT(onBtnPercentChanged(bool)));
}

void UserStockWnd::onBtnPercentChanged(bool)
{
    FilterStockByStockDlg* dlg=new FilterStockByStockDlg(7,FilterCompareType_AbsGreatThan,this);
    if(dlg->exec()==QDialog::Accepted)
    {
        std::vector<PFilterItemPercent> resultItems;
        filterStockPercent(&stockDay,dlg->getCompareType(),dlg->getPercent(),resultItems);

        QStringList horizontalTitles;
        horizontalTitles<<tr("date")<<tr("close")<<tr("percent");
        tableWidgetResult->setHorizontalHeaderLabels(horizontalTitles);
        for(int i=0;i<horizontalTitles.size();i++)
            tableWidgetResult->setColumnWidth(i,60);

        std::vector<PFilterItemPercent>::iterator it=resultItems.begin(),end=resultItems.end();
        QTableWidgetItem* pItem;
        QString itemText;
        tableWidgetResult->setColumnCount(3);
        tableWidgetResult->setRowCount(resultItems.size());
        StockDate* dates=stockDay.getDates();
        StockFloat* closes=stockDay.getCloses();
        for(int row=0;it!=end;it++,row++)
        {
            pItem=new QTableWidgetItem(dates[(*it)->curr].date);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            tableWidgetResult->setItem(row,0,pItem);

            itemText=QString("%1").arg(closes[(*it)->curr],0,'f',2);
            pItem=new QTableWidgetItem(itemText);
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            tableWidgetResult->setItem(row,1,pItem);

            pItem=new QTableWidgetItem(QString("%1").arg((*it)->percent));
            pItem->setFlags(pItem->flags()&(~Qt::ItemIsEditable));
            tableWidgetResult->setItem(row,2,pItem);
            //qDebug()<<(*it)->date;
        }
        clearFilterItemPercent(resultItems);
    }
    delete dlg;
}
void UserStockWnd::onDayChartChanged(chart::StockDayChart* view,int start,int count)
{
    if(stockDay.getCount()<=0)return;
    reCalcDayChartRange(view,start,count);
}
