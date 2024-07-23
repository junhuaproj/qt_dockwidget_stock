#include "stocknotedlg.h"
#include "ui_stocknotedlg.h"
//#include "stocknote.h"
#include "stocklistitem.h"
#include <QDateTime>
#include "appconfig.h"
extern const char* datetime_format;

StockNoteDlg::StockNoteDlg(const StockListItem* stock,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::StockNoteDlg)
    ,pStock(stock)
{
    ui->setupUi(this);
    QString title;
    pStock->toString(title);
    ui->labelTitle->setText(title);

    if(stock->getComment().length())
    {
        ui->labelLastDate->setText(stock->getCommentDate().toString(appConfig.getDateFormat()));
        ui->plainTextEdit->setPlainText(stock->getComment());
        ui->spinBoxLevel->setValue(stock->getCommentLevel());
    }
    else
    {
        ui->labelLastDate->setVisible(false);
    }
    appConfig.updateQss(this);
}

StockNoteDlg::~StockNoteDlg()
{
    delete ui;
}
void StockNoteDlg::accept()
{
    QDialog::accept();

}

void StockNoteDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    getDlgBoxButtons(wids,ui->buttonBox);
    wids.push_back(ui->plainTextEdit);
    wids.push_back(ui->spinBoxLevel);
}

void StockNoteDlg::getData(StockListItem* stock)
{
    //note->setCode(pStock->getCode());
    stock->setComment(ui->plainTextEdit->toPlainText());
    stock->setCommentLevel(ui->spinBoxLevel->value());
    stock->setCommentDate(QDate::currentDate());
}
