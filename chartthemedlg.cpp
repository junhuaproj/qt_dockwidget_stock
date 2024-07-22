#include "chartthemedlg.h"
#include "ui_chartthemedlg.h"

namespace chart {

#define DATA_INDEX_CFG (Qt::UserRole+2)

#define DATA_VALUE_TYPE (Qt::UserRole+1)
ChartThemeDlg::ChartThemeDlg(QList<StatisticIndexCfg*>& cfgs,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChartThemeDlg),pCurCfg(NULL)
{
    ui->setupUi(this);

    //memcpy(&themes,param,sizeof(DayChartParam));
    //QStringList listStrings;
    QList<StatisticIndexCfg*>::iterator it=cfgs.begin(),end=cfgs.end();
    QListWidgetItem* pItem;
    for(;it!=end;it++)
    {
        pItem=new QListWidgetItem((*it)->cfgName());
        pItem->setData(DATA_INDEX_CFG,QVariant((uint64_t)(*it)));
        ui->listWidget->addItem(pItem);
    }

    //listStrings<<"generate"<<"kline"<<"volume"<<"macd"<<"kdj";
    //ui->listWidget->addItems(listStrings);

    QStringList headTitle;
    headTitle<<"type"<<"value"<<"description";
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,120);
    ui->tableWidget->setHorizontalHeaderLabels(headTitle);
    ui->tableWidget->setRowCount(0);

    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(onCellEdit(int,int)));

    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(onChartTypeChanged(int)));
    connect(this,SIGNAL(accepted()),this,SLOT(onAccepted()));
}

ChartThemeDlg::~ChartThemeDlg()
{
    delete ui;
}
void ChartThemeDlg::onAccepted()
{

}


void ChartThemeDlg::onCellChanged(int row,int col)
{
    if(col!=1)return;

    QTableWidgetItem* item=ui->tableWidget->item(row,col);
    IndexCfgValue cfgValue;
    cfgValue.type=qvariant_cast<DataValueType>(item->data(DATA_VALUE_TYPE));
    QString text=item->text();
    switch(cfgValue.type)
    {
    case DataValue_Clr:
    {
        bool bOK;
        cfgValue.vClr=text.toInt(&bOK,16);
    }
        break;
    case DataValue_Font:
    case DataValue_Text:
        cfgValue.vStr=text;
        break;
    case DataValue_Int:
        cfgValue.vInt=text.toInt();
        break;
    case DataValue_Flt:
        cfgValue.vFlt=text.toFloat();
        break;
    }

    pCurCfg->setCfgValue(row,&cfgValue);

}

void ChartThemeDlg::onCellEdit(int row,int col)
{

}

void ChartThemeDlg::setTableRow(int row,IndexCfgValue* cfg)
{
    QTableWidgetItem* item=new QTableWidgetItem(cfg->name);

    ui->tableWidget->setItem(row,0,item);
    QString value;
    switch(cfg->type)
    {
    case DataValue_Clr:
        value=QString("%1").arg(cfg->vClr,8,16,QLatin1Char('0'));break;
    case DataValue_Int:
        value=QString("%1").arg(cfg->vInt);break;
    case DataValue_Flt:
        value=QString("%1").arg(cfg->vFlt,0,'f',2);break;
    case DataValue_Text:
    case DataValue_Font:
        value=cfg->vStr;
        break;
    }

    item=new QTableWidgetItem(value);
    item->setData(DATA_VALUE_TYPE,cfg->type);
    ui->tableWidget->setItem(row,1,item);
    item=new QTableWidgetItem(cfg->description);
    ui->tableWidget->setItem(row,2,item);
}
void ChartThemeDlg::onChartTypeChanged(int index)
{
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));

    QListWidgetItem* pItem=ui->listWidget->item(index);
    QVariant var=pItem->data(DATA_INDEX_CFG);
    pCurCfg=(StatisticIndexCfg*)qvariant_cast<uint64_t>(var);

    int count=pCurCfg->getCfgCount();
    ui->tableWidget->setRowCount(count);
    IndexCfgValue cfgValue;
    for(int i=0;i<count;i++)
    {
        pCurCfg->getCfgValue(i,&cfgValue);
        setTableRow(i,&cfgValue);
    }

    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(onCellChanged(int,int)));
}
}
