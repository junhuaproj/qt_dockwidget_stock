#include "stockrecorddlg.h"
#include "ui_stockrecorddlg.h"
#include "stocklistitem.h"
#include "stockrecordlist.h"
#include "appconfig.h"

StockRecordDlg::StockRecordDlg(const StockListItem* stock,data::StockRecordItem* r,const data::StockRecordList* list,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::StockRecordDlg),pRecord(r),pStock(stock),pList(list)
{
    ui->setupUi(this);

    QString text=QString(tr("%1-%2")).arg(pStock->getCode(),pStock->getName());
    ui->lineEditStock->setText(text);
    if(pList)
    {
        text=QString(tr("#%1-%2")).arg(pList->getId()).arg(pList->getTitle());
        ui->lineEditListInfo->setText(text);
    }
    if(pRecord)
    {
        ui->spinBoxLevel->setValue(pRecord->getLevel());
        ui->lineEditCondition->setText(pRecord->getCondtion());
        ui->lineEditDateTime->setText(pRecord->getDatetime().toString(datetime_format));
        ui->lineEditKeyWord->setText(pRecord->getKey());
        ui->textEditContent->setText(pRecord->getText());
    }
}

StockRecordDlg::~StockRecordDlg()
{
    delete ui;
}
void StockRecordDlg::accept()
{
    QDialog::accept();
    pRecord->setText(ui->textEditContent->toPlainText());
    pRecord->setCondtion(ui->lineEditCondition->text());
    pRecord->setKey(ui->lineEditKeyWord->text());
    pRecord->setLevel(ui->spinBoxLevel->value());
}
void StockRecordDlg::getThemingWidget(QList<QWidget*>& wids)const
{

}
