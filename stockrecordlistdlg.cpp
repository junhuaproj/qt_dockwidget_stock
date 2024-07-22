#include "stockrecordlistdlg.h"
#include "ui_stockrecordlistdlg.h"
#include "stockrecordlist.h"
#include "stocklistitem.h"
#include "appconfig.h"

#define DATA_TABLE_STOCK    (Qt::UserRole+2)
#define DATA_TABLE_RECORD    (Qt::UserRole+3)

#include <functional>
#include <memory>


using namespace std::placeholders;

StockRecordListDlg::StockRecordListDlg(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::StockRecordListDlg)
{
    ui->setupUi(this);
    QStringList header;
    header<<tr("stock")<<tr("condtion")<<tr("key")<<tr("level");
    ui->tableWidget->setColumnCount(header.size());
    ui->tableWidget->setHorizontalHeaderLabels(header);
    appConfig.updateQss(this);
}

StockRecordListDlg::~StockRecordListDlg()
{
    delete ui;
}

void StockRecordListDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(ui->lineEditDateTime);
    wids.push_back(ui->lineEditTitle);
    wids.push_back(ui->spinBoxLevel);
    wids.push_back(ui->tableWidget);
    wids.push_back(ui->textEditContent);
    getDlgBoxButtons(wids,ui->buttonBox);
}
void StockRecordListDlg::setStocks(const QList<StockListItem*>& s)
{
    stocks.append(s);
    QList<StockListItem*>::const_iterator it=stocks.constBegin(),end=stocks.constEnd();
    QTableWidgetItem* pItem;
    ui->tableWidget->setRowCount(stocks.size());
    for(int i=0;it!=end;it++,i++)
    {
        pItem=new QTableWidgetItem(QString(tr("%1 %2")).arg((*it)->getCode(),(*it)->getName()));
        pItem->setData(DATA_TABLE_STOCK,QVariant((uint64_t)(*it)));
        ui->tableWidget->setItem(i,0,pItem);
    }
}
void StockRecordListDlg::accept()
{
    QDialog::accept();
}
void StockRecordListDlg::setRecordList(const data::StockRecordList* list)
{
    ui->lineEditTitle->setText(list->getTitle());
    ui->lineEditDateTime->setText(list->getDatetime().toString(datetime_format));
    ui->textEditContent->setText(list->getContent());
    ui->spinBoxLevel->setValue(list->getLevel());

    setWindowTitle(QString(tr("#%1 %2").arg(list->getId()).arg(list->getTitle())));

    QList<data::StockRecordItem*> items;
    items.append(list->getItems());
    int rowCount=ui->tableWidget->rowCount();

    QList<data::StockRecordItem*>::iterator itFinded;
    QTableWidgetItem* pTableItem;
    //QList<StockRecordItem*>::const_iterator it=list->getItems().begin(),end=list->getItems().end();
    for(int i=0;i<rowCount;i++)
    {
        pTableItem=ui->tableWidget->item(i,0);
        QVariant var=pTableItem->data(DATA_TABLE_STOCK);
        StockListItem* pStock=(StockListItem*)qvariant_cast<uint64_t>(var);
#ifdef DB_MYSQL
        itFinded=std::find_if(items.begin(),items.end(),
                                std::bind(data::find_id_from_record_func,_1,pStock));
#else
        itFinded=std::find_if(items.begin(),items.end(),
                     std::bind(data::find_code_from_record_func,_1,pStock->getCode()));
#endif
        if(itFinded!=items.end())
        {
            pTableItem=new QTableWidgetItem((*itFinded)->getCondtion());
            pTableItem->setData(DATA_TABLE_RECORD,QVariant((uint64_t)(*itFinded)));
            ui->tableWidget->setItem(i,1,pTableItem);

            pTableItem=new QTableWidgetItem((*itFinded)->getKey());
            ui->tableWidget->setItem(i,2,pTableItem);

            pTableItem=new QTableWidgetItem(QString("%1").arg((*itFinded)->getLevel()));
            ui->tableWidget->setItem(i,3,pTableItem);


            items.erase(itFinded);
        }
    }
    int beginIndex=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(beginIndex+items.size());
    for(itFinded=items.begin();itFinded!=items.end();itFinded++,beginIndex++)
    {
#ifdef DB_MYSQL
        pTableItem=new QTableWidgetItem((*itFinded)->getStock()->getCode());
        ui->tableWidget->setItem(beginIndex,0,pTableItem);
#else
        pTableItem=new QTableWidgetItem((*itFinded)->getCode());
        ui->tableWidget->setItem(beginIndex,0,pTableItem);
#endif
        pTableItem=new QTableWidgetItem((*itFinded)->getCondtion());
        pTableItem->setData(DATA_TABLE_RECORD,QVariant((uint64_t)(*itFinded)));
        ui->tableWidget->setItem(beginIndex,1,pTableItem);

        pTableItem=new QTableWidgetItem((*itFinded)->getKey());
        ui->tableWidget->setItem(beginIndex,2,pTableItem);

        pTableItem=new QTableWidgetItem(QString("%1").arg((*itFinded)->getLevel()));
        ui->tableWidget->setItem(beginIndex,3,pTableItem);
    }
}
void StockRecordListDlg::getRecordList(data::StockRecordList* list)
{
    list->setTitle(ui->lineEditTitle->text());
    list->setContent(ui->textEditContent->toPlainText());
    list->setLevel(ui->spinBoxLevel->value());
    int count=ui->tableWidget->rowCount();
    QVariant var;
    QTableWidgetItem* pTableItem;
    data::StockRecordItem* pRecord;
    for(int i=0;i<count;i++)
    {
        pTableItem=ui->tableWidget->item(i,1);

        if(pTableItem==NULL)
        {
            var=ui->tableWidget->item(i,0)->data(DATA_TABLE_STOCK);
            StockListItem* pStock=(StockListItem*)qvariant_cast<uint64_t>(var);
            pRecord=new data::StockRecordItem();
            pRecord->setStock(pStock);
#ifdef DB_MYSQL
#else
            pRecord->setCode(pStock->getCode());
#endif
            pRecord->setLevel(1);
            pRecord->setDatetime(list->getDatetime());
            list->addItem(pRecord);
        }
        else
        {
            var=pTableItem->data(DATA_TABLE_RECORD);
            pRecord=(data::StockRecordItem*)qvariant_cast<uint64_t>(var);
            if(!list->isRecordExist(pRecord))
            {
                list->addItem(pRecord);
            }
        }
    }
}
