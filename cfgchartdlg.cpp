#include "cfgchartdlg.h"
#include "ui_cfgchartdlg.h"

CfgChartDlg::CfgChartDlg(PCfgChartAll pCfg,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CfgChartDlg),pCfgAll(pCfg)
{
    ui->setupUi(this);
    connect(ui->pushButtonTest,SIGNAL(pressed()),this,SLOT(onButtonTest()));

    connect(ui->radioDown2Hide,SIGNAL(clicked(bool)),this,SLOT(onDownRadioHide(bool)));
    connect(ui->radioDownVol,SIGNAL(clicked(bool)),this,SLOT(onDownRadioVol(bool)));
    connect(ui->radioDownMacd2,SIGNAL(clicked(bool)),this,SLOT(onDownRadioMacd2(bool)));

    connect(ui->radioUp2Hide,SIGNAL(clicked(bool)),this,SLOT(onUpRadioHide(bool)));
    connect(ui->radioUpVol,SIGNAL(clicked(bool)),this,SLOT(onUpRadioVol(bool)));
    connect(ui->radioUpMacd2,SIGNAL(clicked(bool)),this,SLOT(onUpRadioMacd2(bool)));
    setCfgToUi();
}

CfgChartDlg::~CfgChartDlg()
{
    delete ui;
}
void CfgChartDlg::onButtonTest()
{
    updateCfg();
    emit cfgChanged(pCfgAll);
}
void CfgChartDlg::onUpRadioVol(bool bCheck)
{
    if(bCheck){
        ui->radioUp2Hide->setChecked(false);
        ui->radioUpMacd2->setChecked(false);
    }
    else
    {
        ui->radioUpVol->setChecked(true);
    }
}
void CfgChartDlg::onUpRadioMacd2(bool bCheck)
{
    if(bCheck){
        ui->radioUp2Hide->setChecked(false);
        ui->radioDown2Hide->setChecked(false);
    }
    else
    {
        ui->radioUpMacd2->setChecked(true);
    }
}
void CfgChartDlg::onUpRadioHide(bool bCheck)
{
    if(bCheck){
        ui->radioUpVol->setChecked(false);
        ui->radioDown2Hide->setChecked(false);
    }
    else
    {
        ui->radioUp2Hide->setChecked(true);
    }
}

void CfgChartDlg::onDownRadioVol(bool bCheck)
{
    if(bCheck){
        ui->radioDown2Hide->setChecked(false);
        ui->radioDownMacd2->setChecked(false);
    }
    else{
        ui->radioDownVol->setChecked(true);
    }
}
void CfgChartDlg::onDownRadioMacd2(bool bCheck)
{
    if(bCheck){
        ui->radioDown2Hide->setChecked(false);
        ui->radioDownVol->setChecked(false);
    }
    else
    {
        ui->radioDownMacd2->setChecked(true);
    }
}
void CfgChartDlg::onDownRadioHide(bool bCheck)
{
    if(bCheck){
        ui->radioDownMacd2->setChecked(false);
        ui->radioDownVol->setChecked(false);
    }
    else
    {
        ui->radioDown2Hide->setChecked(true);
    }
}
void CfgChartDlg::setCfgToUi()
{
    ui->checkUp->setChecked(pCfgAll->flag&CFG_ALL_FLAG_SHOW_UP);

    ui->checkDown->setChecked(pCfgAll->flag&CFG_ALL_FLAG_SHOW_DOWN);

    ui->checkUpK->setChecked(pCfgAll->up.flag&CFG_CHART_SHOW_MIAN);

    ui->checkUpMacd->setChecked(pCfgAll->up.flag&CFG_CHART_SHOW_VIEW3);

    if(pCfgAll->up.flag&CFG_CHART_SHOW_VIEW2)
    {
        ui->radioUp2Hide->setChecked(false);
        ui->radioUpVol->setChecked(pCfgAll->up.flag&CFG_CHART_SHOW_VIEW2_VOL);

        ui->radioUpMacd2->setChecked(pCfgAll->up.flag&CFG_CHART_SHOW_VIEW2_MACD2);
    }
    else
        ui->radioUp2Hide->setChecked(true);



    ui->spinUpKPercent->setValue(pCfgAll->up.percentMain);
    ui->spinUpVolPercent->setValue(pCfgAll->up.percentView2);
    ui->spinUpMacdPercent->setValue(pCfgAll->up.percentView3);

    ui->spinUpMacdFast->setValue(pCfgAll->up.macd1.fast);
    ui->spinUpMacdSlow->setValue(pCfgAll->up.macd1.slow);
    ui->spinUpMacdDea->setValue(pCfgAll->up.macd1.dea);

    ui->spinUpMacd2Fast->setValue(pCfgAll->up.macd2.fast);
    ui->spinUpMacd2Slow->setValue(pCfgAll->up.macd2.slow);
    ui->spinUpMacd2Dea->setValue(pCfgAll->up.macd2.dea);


    ui->checkDownK->setChecked(pCfgAll->down.flag&CFG_CHART_SHOW_MIAN);

    ui->checkDownMacd->setChecked(pCfgAll->down.flag&CFG_CHART_SHOW_VIEW3);

    if(pCfgAll->down.flag&CFG_CHART_SHOW_VIEW2)
    {
        ui->radioDown2Hide->setChecked(false);
        ui->radioDownVol->setChecked(pCfgAll->down.flag&CFG_CHART_SHOW_VIEW2_VOL);

        ui->radioDownMacd2->setChecked(pCfgAll->down.flag&CFG_CHART_SHOW_VIEW2_MACD2);
    }
    else
        ui->radioDown2Hide->setChecked(true);



    ui->spinDownKPercent->setValue(pCfgAll->down.percentMain);
    ui->spinDownVolPercent->setValue(pCfgAll->down.percentView2);
    ui->spinDownMacdPercent->setValue(pCfgAll->down.percentView3);

    ui->spinDownMacdFast->setValue(pCfgAll->down.macd1.fast);
    ui->spinDownMacdSlow->setValue(pCfgAll->down.macd1.slow);
    ui->spinDownMacdDea->setValue(pCfgAll->down.macd1.dea);

    ui->spinDownMacd2Fast->setValue(pCfgAll->down.macd2.fast);
    ui->spinDownMacd2Slow->setValue(pCfgAll->down.macd2.slow);
    ui->spinDownMacd2Dea->setValue(pCfgAll->down.macd2.dea);
}

void CfgChartDlg::updateCfg()
{
    pCfgAll->flag=0;
    if(ui->checkUp->isChecked())
        pCfgAll->flag|=CFG_ALL_FLAG_SHOW_UP;

    if(ui->checkDown->isChecked())
        pCfgAll->flag|=CFG_ALL_FLAG_SHOW_DOWN;

    pCfgAll->up.flag=0;
    if(ui->checkUpK->isChecked())
        pCfgAll->up.flag|=CFG_CHART_SHOW_MIAN;
    if(ui->checkUpMacd->isChecked())
        pCfgAll->up.flag|=CFG_CHART_SHOW_VIEW3;
    if(!ui->radioUp2Hide->isChecked())
    {
        pCfgAll->up.flag|=CFG_CHART_SHOW_VIEW2;
        if(ui->radioUpVol->isChecked())
            pCfgAll->up.flag|=CFG_CHART_SHOW_VIEW2_VOL;
        else if(ui->radioUpMacd2->isChecked())
            pCfgAll->up.flag|=CFG_CHART_SHOW_VIEW2_MACD2;
    }
    pCfgAll->up.percentMain=ui->spinUpKPercent->value();
    pCfgAll->up.percentView2=ui->spinUpVolPercent->value();
    pCfgAll->up.percentView3=ui->spinUpMacdPercent->value();

    pCfgAll->up.macd1.fast=ui->spinUpMacdFast->value();
    pCfgAll->up.macd1.slow=ui->spinUpMacdSlow->value();
    pCfgAll->up.macd1.dea=ui->spinUpMacdDea->value();

    pCfgAll->up.macd2.fast=ui->spinUpMacd2Fast->value();
    pCfgAll->up.macd2.slow=ui->spinUpMacd2Slow->value();
    pCfgAll->up.macd2.dea=ui->spinUpMacd2Dea->value();

    pCfgAll->down.flag=0;
    if(ui->checkDownK->isChecked())
        pCfgAll->down.flag|=CFG_CHART_SHOW_MIAN;
    if(ui->checkDownMacd->isChecked())
        pCfgAll->down.flag|=CFG_CHART_SHOW_VIEW3;
    if(!ui->radioDown2Hide->isChecked())
    {
        pCfgAll->down.flag|=CFG_CHART_SHOW_VIEW2;
        if(ui->radioDownVol->isChecked())
            pCfgAll->down.flag|=CFG_CHART_SHOW_VIEW2_VOL;
        else if(ui->radioDownMacd2->isChecked())
            pCfgAll->down.flag|=CFG_CHART_SHOW_VIEW2_MACD2;
    }
    pCfgAll->down.percentMain=ui->spinDownKPercent->value();
    pCfgAll->down.percentView2=ui->spinDownVolPercent->value();
    pCfgAll->down.percentView3=ui->spinDownMacdPercent->value();

    pCfgAll->down.macd1.fast=ui->spinDownMacdFast->value();
    pCfgAll->down.macd1.slow=ui->spinDownMacdSlow->value();
    pCfgAll->down.macd1.dea=ui->spinDownMacdDea->value();

    pCfgAll->down.macd2.fast=ui->spinDownMacd2Fast->value();
    pCfgAll->down.macd2.slow=ui->spinDownMacd2Slow->value();
    pCfgAll->down.macd2.dea=ui->spinDownMacd2Dea->value();
}
