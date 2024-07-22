#include "dockwidgetdlg.h"
#include "ui_dockwidgetdlg.h"
#include "usermainwnd.h"
#include "qtablecombobox.h"
#define TABLE_DOCK_DATA     (Qt::UserRole+1)
DockWidgetDlg::DockWidgetDlg(QList<QDockWidget*>& docks,UserMainWnd* userWnd,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::DockWidgetDlg)
    ,pUserWnd(userWnd),oldCbx(NULL)
{
    ui->setupUi(this);
    QList<QDockWidget*>::iterator it=docks.begin(),end=docks.end();
    QDockWidget* dock;

    QTableWidgetItem* item;

    ui->tableWidget->setRowCount(docks.size());

    for(int row=0;it!=end;it++,row++)
    {
        dock=(*it);
        item=new QTableWidgetItem(dock->windowTitle());
        item->setData(TABLE_DOCK_DATA,QVariant((uint64_t)dock));

        item->setCheckState(dock->isVisible()?Qt::Checked:Qt::Unchecked);
        ui->tableWidget->setItem(row,0,item);

    }
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onCellDoubleClick(int,int)));
    appConfig.updateQss(this);
}

void DockWidgetDlg::onTableCbxKillFocus(QTableComboBox* cbx)
{
    Qt::DockWidgetArea dockArea=(Qt::DockWidgetArea)(cbx->currentIndex()+1);
    QDockWidget* dock=(QDockWidget*)cbx->getData1();
    pUserWnd->removeDockWidget(dock);
    pUserWnd->addDockWidget(dockArea,dock);
    int row=cbx->getData2();
    ui->tableWidget->removeCellWidget(row,1);
    //ui->tableWidget->cellWidget(row,1);
    cbx->deleteLater();
    oldCbx=NULL;
}
void DockWidgetDlg::onCurrentDockChanged(int index)
{

}
void DockWidgetDlg::onCellDoubleClick(int row,int col)
{
    if(col==1)
    {
        if(oldCbx)
        {
            onTableCbxKillFocus(oldCbx);

        }
        QTableWidgetItem* pItem=ui->tableWidget->item(row,0);

        QDockWidget* dock=(QDockWidget*)qvariant_cast<uint64_t>(pItem->data(TABLE_DOCK_DATA));
        Qt::DockWidgetArea dockArea;
        QStringList cbxItems;
        cbxItems<<tr("Left")<<tr("Right")<<tr("Top")<<tr("Bottom");

        QTableComboBox* cbx=new QTableComboBox(ui->tableWidget);
        cbx->addItems(cbxItems);
        connect(cbx,SIGNAL(currentIndexChanged(int)),SLOT(onCurrentDockChanged(int)));
        ui->tableWidget->setCellWidget(row,1,cbx);
        dockArea=pUserWnd->dockWidgetArea(dock);
        cbx->setCurrentIndex((int)dockArea-1);
        connect(cbx,SIGNAL(tableCbxKillFocus(QTableComboBox*)),this,SLOT(onTableCbxKillFocus(QTableComboBox*)));
        cbx->setData1((uint64_t)dock);
        cbx->setData2(row);
        oldCbx=cbx;
        qDebug()<<"dbcell";
    }
}

void DockWidgetDlg::onCellChanged(int row,int col)
{
    QTableWidgetItem* pItem=ui->tableWidget->item(row,col);
    QVariant var=pItem->data(TABLE_DOCK_DATA);
    QDockWidget* dock=(QDockWidget*)qvariant_cast<uint64_t>(var);
    dock->setVisible(pItem->checkState()==Qt::Checked);
    qDebug()<<"checkstate"<<pItem->checkState();

}

DockWidgetDlg::~DockWidgetDlg()
{
    delete ui;
}
void DockWidgetDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(ui->tableWidget);
    getDlgBoxButtons(wids,ui->buttonBox);
}
