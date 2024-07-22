#include "filterstockbypercentdlg.h"
#include "ui_filterstockbypercentdlg.h"

FilterStockByStockDlg::FilterStockByStockDlg(int inpercent,FilterCompareType type,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::FilterStockByStockDlg)
    ,percent(inpercent)
    ,compareType(type)
{
    ui->setupUi(this);
    connect(this,SIGNAL(accepted()),this,SLOT(onAccept()));
    ui->comboBoxType->addItem(tr("great than"));
    ui->comboBoxType->addItem(tr("equivalent"));
    ui->comboBoxType->addItem(tr("less than"));
    ui->comboBoxType->addItem(tr("abs less than"));
    ui->comboBoxType->addItem(tr("abs great than"));

    if(compareType==FilterCompareType_LessThan)
        ui->comboBoxType->setCurrentIndex(0);
    else if(compareType==FilterCompareType_GreatThan)
        ui->comboBoxType->setCurrentIndex(2);
    else if(compareType==FilterCompareType_AbsLessThan)
    {
        ui->comboBoxType->setCurrentIndex(3);
    }
    else if(compareType==FilterCompareType_AbsGreatThan)
    {
        ui->comboBoxType->setCurrentIndex(4);
    }
    else
        ui->comboBoxType->setCurrentIndex(1);
    ui->spinBoxPercent->setValue(percent);
}

FilterStockByStockDlg::~FilterStockByStockDlg()
{
    delete ui;
}

void FilterStockByStockDlg::onAccept()
{
    switch(ui->comboBoxType->currentIndex())
    {
    case 0:
        compareType=FilterCompareType_LessThan;break;
    case 1:
        compareType=FilterCompareType_Equivalent;break;
    case 2:
        compareType=FilterCompareType_GreatThan;break;
    case 3:
        compareType=FilterCompareType_AbsLessThan;break;
    case 4:
        compareType=FilterCompareType_AbsGreatThan;break;
    }
    percent=ui->spinBoxPercent->value();
}
FilterCompareType FilterStockByStockDlg::getCompareType()const
{
    return compareType;
}
int FilterStockByStockDlg::getPercent()const
{
    return percent;
}

void FilterStockByStockDlg::getThemingWidget(QList<QWidget*>& wids)const
{

}
