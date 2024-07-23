#include "simulatetradeeditdlg.h"
#include "ui_simulatetradeeditdlg.h"
#include "appconfig.h"
namespace simulate{
TradeEditDlg::TradeEditDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SimulateTradeEditDlg),row(-1)
{
    ui->setupUi(this);
    connect(ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(onBtnClose(bool)));
    connect(ui->pushButtonDelete,SIGNAL(clicked(bool)),this,SLOT(onBtnDelete(bool)));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(onBtnSave(bool)));

    connect(ui->spinBoxBuy,SIGNAL(valueChanged(double)),this,SLOT(onPriceChanged(double)));
    connect(ui->spinBoxSell,SIGNAL(valueChanged(double)),this,SLOT(onPriceChanged(double)));
}

TradeEditDlg::~TradeEditDlg()
{
    delete ui;
}

void TradeEditDlg::onPriceChanged(double)
{
    double buy=ui->spinBoxBuy->value();
    double diff=ui->spinBoxSell->value()-buy;
    ui->lineEditDiff->setText(QString(tr("%1")).arg(diff));
    ui->lineEditPercent->setText(QString(tr("%1")).arg(diff/buy));
}

void TradeEditDlg::setTrade(int row,SimulateTrade* trade)
{
    this->row=row;
    this->trade=trade;

    ui->dateEditBuy->setDate(QDate::fromString(trade->buyDate,appConfig.getDateFormat()));
    ui->spinBoxBuy->setValue(trade->buyPrice);
    ui->dateEditSell->setDate(QDate::fromString(trade->sellDate,appConfig.getDateFormat()));
    ui->spinBoxSell->setValue(trade->sellPrice);
    ui->textEditNote->setText(trade->note);
}
void TradeEditDlg::onBtnDelete(bool)
{
    emit tradeDelete(this,row,trade);
}
void TradeEditDlg::onBtnSave(bool)
{
    trade->buyDate=ui->dateEditBuy->date().toString(appConfig.getDateFormat());
    trade->buyPrice=ui->spinBoxBuy->value();
    trade->sellDate=ui->dateEditSell->date().toString(appConfig.getDateFormat());
    trade->sellPrice=ui->spinBoxSell->value();
    trade->note=ui->textEditNote->placeholderText();
    emit tradeSave(this,row,trade);
}
void TradeEditDlg::onBtnClose(bool)
{
    reject();
}
}
