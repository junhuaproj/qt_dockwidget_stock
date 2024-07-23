#include "RecordWidget.h"
#include "stockrecorditem.h"
#include "stockrecorddlg.h"
#include "stocklistitem.h"
//#include "stockrecorddb.h"
#include "mysqldata/mydb.h"
#include "appconfig.h"

#define LIST_DATA_RECORD    (Qt::UserRole+2)
//extern const char* date_format;
RecordWidget::RecordWidget(MyDB* db,QWidget* parent)
    :BaseWidget(parent), pStock(NULL), mydb(db),parentList(NULL)
{
	layout = new QVBoxLayout(this);
	listRecord = new QListWidget(this);
	layout->addWidget(listRecord);

	setLayout(layout);

	connect(listRecord, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListDbClick(QModelIndex)));
}

RecordWidget::~RecordWidget()
{
    clear();
}

void RecordWidget::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(listRecord);
}

void RecordWidget::clear()
{
    while (!items.isEmpty())
    {
        delete items.back();
        items.pop_back();
    }
}
void RecordWidget::setStockItem(const StockListItem* pStock)
{
    this->pStock = pStock;
    listRecord->clear();
    clear();
    if(this->pStock==NULL)return ;
    //QListWidgetItem* pListItem;
    if (parentList)
    {

    data::StockRecordItem* pItem = parentList->findRecord(pStock);

        if (pItem)
        {
            addRecordItem(pItem);

        }
    }

    {
        mydb->getStockRecords(pStock->getId(), NULL, NULL, items);


        QList<data::StockRecordItem*>::iterator it = items.begin(), end = items.end();
        for (; it != end; it++)
        {
            if(parentList&&(*it)->getParent()==parentList->getId())
            {
                continue;
            }
            addRecordItem(*it);

        }
    }
}

void RecordWidget::addRecordItem(data::StockRecordItem* pItem)
{

    QString text=QString(tr("%1 %2 %3"))
                       .arg(pItem->getStock()->getName())
                       .arg(pItem->getText())
                       .arg(pItem->getLevel())

    .arg(pItem->getDatetime().toString(appConfig.getDateFormat()));
    QListWidgetItem* pListItem = new QListWidgetItem(text);
    pListItem->setData(LIST_DATA_RECORD, QVariant((uint64_t)pItem));
    listRecord->addItem(pListItem);
}
void RecordWidget::addRecord(data::StockRecordItem* pItem)
{
    addRecordItem(pItem);
}
void RecordWidget::removeRecord(int parent)
{
    int count=listRecord->count()-1;
    QListWidgetItem* pListItem;
    QVariant var;
    data::StockRecordItem* pRecord;
    for(;count>=0;count--)
    {
        pListItem=listRecord->item(count);
        var=pListItem->data(LIST_DATA_RECORD);
        pRecord=(data::StockRecordItem*)qvariant_cast<uint64_t>(var);
        if(pRecord->getParent()==parent)
        {//这里内存由List管理，这里不处理
            listRecord->removeItemWidget(pListItem);
        }
    }
}
void RecordWidget::setRecordList(data::StockRecordList* recordList)
{
    parentList = recordList;
}
void RecordWidget::onListDbClick(QModelIndex index)
{
    QListWidgetItem* pListItem = listRecord->item(index.row());
    QVariant var = pListItem->data(LIST_DATA_RECORD);
    data::StockRecordItem* pItem = (data::StockRecordItem*)qvariant_cast<uint64_t>(var);
    if (pItem == NULL)return;
    
    StockRecordDlg* dlg = new StockRecordDlg(pStock, pItem, parentList, this);
    if (dlg->exec() == QDialog::Accepted)
    {
        mydb->updateRecord(pItem);

        pListItem->setText(pItem->getStock()->getName() + pItem->getText());

        emit recordItemChanged(pStock,pItem);
    }
    delete dlg;
}
