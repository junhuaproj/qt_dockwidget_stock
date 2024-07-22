#include "daychartcfgdlg.h"
#include "ui_daychartcfgdlg.h"
namespace chart {
DayChartCfgDlg::DayChartCfgDlg(const QList<StatisticIndex*> *pindexes,
                               PChartViewCfg pcfg,int len,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DayChartCfgDlg),indexes(pindexes),
    cfg(pcfg),cfgLen(len)
{
    ui->setupUi(this);

    Q_ASSERT(len==DayChartType_Count);
    ui->spinBoxMainHeight->setValue(cfg[DayChartType_Main].percent);
    ui->checkBoxMainVisible->setChecked(isViewVisible(cfg,DayChartType_Main));

    initCbxType(ui->comboMainType,cfg[DayChartType_Main].pIndex);

    QStringList otherViewType;

    initCbxType(ui->comboView2Type,cfg[DayChartType_View2].pIndex);
    initCbxType(ui->comboView3Type,cfg[DayChartType_View3].pIndex);

    ui->spinBoxView2Height->setValue(cfg[DayChartType_View2].percent);
    ui->checkBoxView2Visible->setChecked(isViewVisible(cfg,DayChartType_View2));

    ui->spinBoxView3Height->setValue(cfg[DayChartType_View3].percent);
    ui->checkBoxView3Visible->setChecked(isViewVisible(cfg,DayChartType_View3));


    connect(this,SIGNAL(accepted()),this,SLOT(onAccepted()));
}

DayChartCfgDlg::~DayChartCfgDlg()
{
    delete ui;
}

void DayChartCfgDlg::initCbxType(QComboBox* pBox,StatisticIndex* pSelected)
{
    QList<StatisticIndex*>::const_iterator it=indexes->begin(),end=indexes->end();
    for(;it!=end;it++)
    {
        pBox->addItem((*it)->getTitle(),QVariant((uint64_t)(*it)));
    }
    pBox->setCurrentText(pSelected->getTitle());
}

void DayChartCfgDlg::onAccepted()
{
    cfg[DayChartType_Main].flag=0;
    cfg[DayChartType_View2].flag=0;
    cfg[DayChartType_View3].flag=0;

    cfg[DayChartType_Main].percent=ui->spinBoxMainHeight->value();
    cfg[DayChartType_View2].percent=ui->spinBoxView2Height->value();
    cfg[DayChartType_View3].percent=ui->spinBoxView3Height->value();
    if(ui->checkBoxMainVisible->isChecked())
        cfg[DayChartType_Main].flag|=ChartParam_Flag_Visible;

    if(ui->checkBoxView2Visible->isChecked())
        cfg[DayChartType_View2].flag|=ChartParam_Flag_Visible;

    if(ui->checkBoxView3Visible->isChecked())
        cfg[DayChartType_View3].flag|=ChartParam_Flag_Visible;

    QVariant var=ui->comboMainType->currentData();
    cfg[DayChartType_Main].pIndex= (StatisticIndex*)qvariant_cast<uint64_t>(var);// ui->comboMainType->currentIndex()+1;
    var=ui->comboView2Type->currentData();
    cfg[DayChartType_View2].pIndex=  (StatisticIndex*)qvariant_cast<uint64_t>(var);
    var=ui->comboView3Type->currentData();
    cfg[DayChartType_View3].pIndex=  (StatisticIndex*)qvariant_cast<uint64_t>(var);

}
}
