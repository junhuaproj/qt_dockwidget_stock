#include "selectcomparedlg.h"
#include "ui_selectcomparedlg.h"

#include "userstockcomparewnd.h"
#include "stocklistitem.h"
namespace compare{
#define TABLE_DATA_WND      (Qt::UserRole+1)

SelectCompareDlg::SelectCompareDlg(StockListItem* pStock,const QList<QMdiSubWindow*>& wnds,CompareStockReplace replaceType,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectCompareDlg)
    ,stockReplace(replaceType),pStockItem(pStock),destWnd(NULL)
{
    ui->setupUi(this);
    QStringList headerTitles;

    headerTitles<<"title"<<"main"<<"slave";
    ui->tableWidget->setColumnCount(headerTitles.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerTitles);

    QList<QMdiSubWindow*>::const_iterator it=wnds.begin(),end=wnds.end();
    UserStockCompareWnd* pWnd;
    QTableWidgetItem* pTableItem;
    const StockListItem* pStockItem;
    ui->tableWidget->setRowCount(wnds.size()+1);

    pTableItem=new QTableWidgetItem(tr("<New Compare Wnd>"));

    ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget->setSelectionMode(QTableWidget::SingleSelection);
    ui->tableWidget->setItem(0,0,pTableItem);

    for(int i=1;it!=end;it++,i++)
    {
        pWnd=(UserStockCompareWnd*)((*it)->widget());
        pTableItem=new QTableWidgetItem(pWnd->windowTitle());
        pTableItem->setData(TABLE_DATA_WND,QVariant((uint64_t)*it));
        ui->tableWidget->setItem(i,0,pTableItem);

        pStockItem=pWnd->getMainStock();
        if(pStockItem)
        {
            pTableItem=new QTableWidgetItem(QString("%1%2").arg(pStockItem->getCode(),pStockItem->getName()));
            ui->tableWidget->setItem(i,1,pTableItem);
        }

        pStockItem=pWnd->getSlaveStock();
        if(pStockItem)
        {
            pTableItem=new QTableWidgetItem(QString("%1%2").arg(pStockItem->getCode(),pStockItem->getName()));
            ui->tableWidget->setItem(i,2,pTableItem);
        }
    }
    ui->radioMain->setChecked(true);
    ui->tableWidget->selectRow(0);
}

SelectCompareDlg::~SelectCompareDlg()
{
    delete ui;
}

void SelectCompareDlg::accept()
{
    QDialog::accept();
    QList<QTableWidgetItem*> items= ui->tableWidget->selectedItems();
    QList<QTableWidgetItem*>::iterator it=items.begin(),end=items.end();

    if(ui->radioMain->isChecked())
        stockReplace=CompareWndReplace_Main;
    else
        stockReplace=CompareWndReplace_Slave;
    for(;it!=end;it++)
    {
        if((*it)->column()!=0)continue;
        if((*it)->row()==0)
        {
            destWnd=NULL;
        }
        QVariant var=(*it)->data(TABLE_DATA_WND);
        destWnd=(QMdiSubWindow*)qvariant_cast<uint64_t>(var);
        return ;
    }

}

QMdiSubWindow* SelectCompareDlg::getSelectedWin()const
{
    return destWnd;
}
CompareStockReplace SelectCompareDlg::getReplaceMode()const
{
    return stockReplace;
}
}
