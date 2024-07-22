#include "todayminutecfgdlg.h"
#include "ui_todayminutewndcfg.h"

TodayMinuteCfgDlg::TodayMinuteCfgDlg(TodayMinuteCfg* pCfg,QWidget *parent)
    : BaseDialog(parent),ui(new Ui::TodayMinuteCfg()),pMinuteCfg(pCfg)
{
    ui->setupUi(this);
    ui->spinBoxColumn->setValue(pMinuteCfg->getColumns());
    ui->spinBoxRow->setValue(pMinuteCfg->getRows());
    ui->spinBoxUpdateTime->setValue(pMinuteCfg->getUpdateTimeout());
    connect(this,SIGNAL(accepted()),this,SLOT(onCfgAccept()));
}

void TodayMinuteCfgDlg::onCfgAccept()
{
    pMinuteCfg->setColumns(ui->spinBoxColumn->value());
    pMinuteCfg->setRows(ui->spinBoxRow->value());
    pMinuteCfg->setUpdateTimeout(ui->spinBoxUpdateTime->value());
}
void TodayMinuteCfgDlg::getThemingWidget(QList<QWidget*>& wids)const
{

}
