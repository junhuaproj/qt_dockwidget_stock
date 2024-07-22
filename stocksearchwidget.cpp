#include "stocksearchwidget.h"

#include "dataloader.h"

#include <functional>
#include <memory>
#include <algorithm>

#include "stockfilterdlg.h"
#include "stockrecordlistdlg.h"
#include "stockrecordlist.h"
//#include "stockrecorddb.h"
#include "tablewidgetitemint.h"
#include "stocknotedlg.h"
//#include "stocknote.h"
#include "appconfig.h"
//#include "AppConfigqss.h"
#include "stocklaststatus.h"

#include "blacklistdlg.h"
#include "stocklistitem.h"
#include "stockrecordlistdlg.h"

#define LIST_DATA_COLUMN    0

#define LIST_DATA_STOCK     (Qt::UserRole+1)
#define LIST_DATA_RECORD    (Qt::UserRole+2)
#define LIST_DATA_LAST      (Qt::UserRole+3)
using namespace std::placeholders;

StockSearchWidget::StockSearchWidget(NetThreadPool* netThreadPool,data::DataLoader* p,QWidget *parent)
    : BaseWidget(parent),pDataLoader(p),pFilterDlg(NULL),sortColumn(-1),
    sortOrder(Qt::AscendingOrder)
    //,dataLoaderNet(NULL)
    ,pNetPool(netThreadPool),
    columns(NULL),columnCount(0),recordList(NULL)
{
    createUI();

    connect(pNetPool,SIGNAL(lastStatusChanged(const QList<StockLastStatus*>&)),this,SLOT(onLastStatusChanged(const QList<StockLastStatus*>&)));

    onColumnChanged();
}

StockSearchWidget::~StockSearchWidget()
{
    onFilterFinished(NULL);
#ifndef DB_MYSQL
    clearNotes();
#endif
    // if(dataLoaderNet)
    //     delete dataLoaderNet;
    pNetPool->removeStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    stockItems.clear();
    if(columns)
        free(columns);
    delete recordList;
}

TableStockMember* StockSearchWidget::getTableCols(int* count,QStringList& titles)
{
    *count=2;
    if(chkExtendInfo->isChecked())
    {
        *count=(*count)+3;
    }
    if(chkUpdateLastPrice->isChecked())
    {
        *count=(*count)+10;
    }
    if(recordList&&chkRecordList->isChecked())
    {
        *count=(*count)+3;
    }
    TableStockMember* ret=(TableStockMember*)malloc(sizeof(TableStockMember)*(*count));
    int index=0;
    ret[index++]=TableStockMember_Code;
    ret[index++]=TableStockMember_Name;
    if(chkExtendInfo->isChecked())
    {
        ret[index++]=TableStockMember_Indexex;
        ret[index++]=TableStockMember_Bussiness;
        ret[index++]=TableStockMember_Notions;
    }
    if(chkUpdateLastPrice->isChecked())
    {
        ret[index++]=TableStockMember_Price;
        ret[index++]=TableStockMember_Chg;
        ret[index++]=TableStockMember_PChg;

        ret[index++]=TableStockMember_Volume;
        ret[index++]=TableStockMember_TradeRate;
        ret[index++]=TableStockMember_VolumeRate;

        ret[index++]=TableStockMember_High;
        ret[index++]=TableStockMember_Low;
        ret[index++]=TableStockMember_PreClose;

        ret[index++]=TableStockMember_Open;
        //ret[index++]=TableStockMember_Count;
    }
    if(recordList&&chkRecordList->isChecked())
    {
        ret[index++]=TableStockMember_DownDay;
        ret[index++]=TableStockMember_LastKeyVal;
        ret[index++]=TableStockMember_DownPercent;
    }
    appConfig.getTableStockColumnNames(ret,*count,titles);
    return ret;
}

void StockSearchWidget::onLastStatusChanged(const QList<StockLastStatus*>& status)
{
    attachLastStatus(status);
}

void StockSearchWidget::onRecordInfo(bool)
{
    resetTableRecordList();
}

void StockSearchWidget::setTableRow(int row,const StockLastStatus* lastStatus)
{
    QTableWidgetItem*  pItem;
    bool bNull;
    QString title;
    QColor color;
    int priceStatus=(int)((lastStatus->getPrice()*100)-(lastStatus->getPreClose()*100));
    if(priceStatus<0)
        //itemStyle=itemStyleGreen;
        color=Qt::green;
    else if(priceStatus>0)
        //itemStyle=itemStyleRed;
        color=Qt::red;
    else
    {
        color=this->palette().color(QPalette::Text);
    }

    for(int i=0;i<columnCount;i++)
    {
        pItem=tableStock->item(row,i);
        bNull=pItem==NULL;
        switch(columns[i])
        {
        case TableStockMember_Price:
            title=QString("%1").arg(lastStatus->getPrice());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_Chg:
            if(pItem)
                pItem->setText(lastStatus->getChangedValue());
            else{
                pItem=new QTableWidgetItem(lastStatus->getChangedValue());
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_PChg:
            if(pItem)
                pItem->setText(lastStatus->getChangedPer());
            else{
                pItem=new QTableWidgetItem(lastStatus->getChangedPer());
            }
            pItem->setForeground(color);
        break;
        case TableStockMember_Volume:
            title=QString("%1").arg(lastStatus->getVolume());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_TradeRate://换手
            //title=QString("%1").arg(lastStatus->getVolRate());
            if(pItem)
                pItem->setText(lastStatus->getChange());
            else{
                pItem=new QTableWidgetItem(lastStatus->getChange());
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_VolumeRate://量比
            if(pItem)
                pItem->setText(lastStatus->getVolRate());
            else{
                pItem=new QTableWidgetItem(lastStatus->getVolRate());
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_High:
            title=QString("%1").arg(lastStatus->getHigh());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_Low:
            title=QString("%1").arg(lastStatus->getLow());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_PreClose:
            title=QString("%1").arg(lastStatus->getPreClose());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;
        case TableStockMember_Open:
            title=QString("%1").arg(lastStatus->getOpen());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            pItem->setForeground(color);
            break;

        default:
            bNull=false;
            break;
        }
        if(bNull)
            tableStock->setItem(row,i,pItem);
    }
    tableStock->item(row,LIST_DATA_COLUMN)->setData(LIST_DATA_LAST,QVariant((uint64_t)lastStatus));
}

void StockSearchWidget::attachLastStatus(const QList<StockLastStatus*>& status)
{
    QVariant var;
    StockListItem* pStock;
    QTableWidgetItem* pItem;//*pPriceItem;
    int count=tableStock->rowCount();
    //StockLastStatus* lastStatus;
    //QString text;
    //int priceStatus;
    //QColor color;
    QList<StockLastStatus*>::const_pointer it;
    for(int i=0;i<count;i++)
    {
        pItem=tableStock->item(i,LIST_DATA_COLUMN);
        if(pItem==NULL)
        {
            qDebug()<<"error;attachLastStatus;line:"<<__LINE__;
            continue;
        }

        var=pItem->data(LIST_DATA_STOCK);
        pStock=(StockListItem*)qvariant_cast<uint64_t>(var);
        it=findLastStatus(status,pStock);
        if(it!=status.end())
        {
            setTableRow(i,*it);
            //lastStatus=*it;

        }
    }

}


void StockSearchWidget::onExtendInfo(bool)
{
    onColumnChanged();
    QTableWidgetItem* pItem;
    QVariant var;
    StockListItem* pStock;
    MyDB* db=pDataLoader->getDB();
    QList<StockProp*> propIndex,propNotion,propBussiness;
    QString out;
    for(int row=0;row<tableStock->rowCount();row++)
    {
        pItem=tableStock->item(row,LIST_DATA_COLUMN);
        var=pItem->data(LIST_DATA_STOCK);
        pStock=(StockListItem*) qvariant_cast<uint64_t>(var);

        db->getProp(pStock->getId(),propIndex,propNotion,propBussiness);

        for(int i=0;i<columnCount;i++)
        {
            pItem=tableStock->item(row,i);
            //bNull=pItem==NULL;
            out.clear();
            switch(columns[i])
            {
            case TableStockMember_Bussiness:
                propListToString(propBussiness,out);
                if(pItem)
                    pItem->setText(out);
                else
                {
                    pItem=new QTableWidgetItem(out);
                    tableStock->setItem(row,i,pItem);
                }
                break;
            case TableStockMember_Indexex://指数
                propListToString(propIndex,out);
                if(pItem)
                    pItem->setText(out);
                else
                {
                    pItem=new QTableWidgetItem(out);
                    tableStock->setItem(row,i,pItem);
                }
                break;
            case TableStockMember_Notions:
                propListToString(propNotion,out);
                if(pItem)
                    pItem->setText(out);
                else
                {
                    pItem=new QTableWidgetItem(out);
                    tableStock->setItem(row,i,pItem);
                }
                break;
            }
        }
        clearPropList(propIndex);
        clearPropList(propNotion);
        clearPropList(propBussiness);
    }
}

void StockSearchWidget::onUpdateLastPrice(bool)
{
    onColumnChanged();
    if(chkUpdateLastPrice->isChecked())
    {
        pNetPool->addStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    }
    else
    {
        pNetPool->removeStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    }
}


void StockSearchWidget::onCurrentCellChanged(int row,int,int,int)
{
    QTableWidgetItem* item=tableStock->item(row,0);
    StockListItem* p=(StockListItem*) qvariant_cast<uint64_t>(item->data(LIST_DATA_STOCK));
    emit stockDbClicked(p);
}
void StockSearchWidget::onSelectStock(StockListItem* p)
{
    emit stockDbClicked(p);
}

void StockSearchWidget::onRadioStock(bool)
{
    QList<StockListItem*> stocks;
    pDataLoader->filterStock(&stocks,StockItemFlag_CN|StockItemFlag_SH|StockItemFlag_SZ);
    setRecordList(NULL);
    setStockToList(&stocks);

}
void StockSearchWidget::onRadioIndex(bool)
{
    QList<StockListItem*> stocks;
    pDataLoader->filterStock(&stocks,StockItemFlag_CN|StockItemFlag_ZS);
    setRecordList(NULL);
    setStockToList(&stocks);
}

void StockSearchWidget::onBtnSearch(bool)
{
    QList<StockListItem*> stocks;
    uint32_t flag=StockItemFlag_CN;
    if(radioStock)
        flag|=StockItemFlag_SH|StockItemFlag_SZ;
    else if(radioIndex)
        flag|=StockItemFlag_ZS;
    pDataLoader->filterStock(&stocks,flag,lineEditKey->text());
    setRecordList(NULL);
    setStockToList(&stocks);
}

void StockSearchWidget::onRecordListStockChanged(data::StockRecordList* newRecordList)
{
    pNetPool->removeStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    tableStock->setRowCount(0);
    stockItems.clear();
    if(recordList)
        delete recordList;
    if(newRecordList)
    {
        const QList<data::StockRecordItem *>& records=newRecordList->getItems();
        QList<data::StockRecordItem *>::const_iterator it= records.begin(),end=records.end();

        for(;it!=end;it++)
            stockItems.push_back((StockListItem*)(*it)->getStock());

        if(chkUpdateLastPrice->isChecked())
            pNetPool->addStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    }
    recordList=newRecordList;
}

void StockSearchWidget::setRecordList(data::StockRecordList* recordList)
{
    //clearLastStatus();
    onRecordListStockChanged(recordList);
    if(recordList==NULL)
    {
        tableStock->setRowCount(0);
        return ;
    }
    resetTableRecordList();
}
void StockSearchWidget::resetTableRecordList()
{
    const QList<data::StockRecordItem *>& records= recordList->getItems();
    QList<data::StockRecordItem *>::const_iterator it=records.begin(),end=records.end();
    onColumnChanged();
    tableStock->setRowCount(records.size());

    //QList<StockListItem*> stocks;
    //StockListItem* pItem;

    for(int row=0;it!=end;it++,row++)
    {
        setTableRow(row,(*it)->getStock());
        setTableRow(row,*it);
    }
}
void StockSearchWidget::setTableRow(int row,const data::StockRecordItem* pRecord)
{
    QTableWidgetItem*  pItem;
    bool bNull;
    QString title;
    for(int i=0;i<columnCount;i++)
    {
        pItem=tableStock->item(row,i);
        bNull=pItem==NULL;
        switch(columns[i])
        {
        case TableStockMember_Level:
            title=QString("%1").arg(pRecord->getLevel());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            break;
        case TableStockMember_Comment:
            if(pItem)
                pItem->setText(pRecord->getText());
            else{
                pItem=new QTableWidgetItem(pRecord->getText());
            }
            break;
        case TableStockMember_DownDay:
            title=QString("%1").arg(pRecord->getDownDay());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            break;
        case TableStockMember_LastKeyVal:
            title=QString("%1").arg(pRecord->getLastKeyVal());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            break;
        case TableStockMember_DownPercent:
            title=QString("%1").arg(pRecord->getDownPercent());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            break;
        default:
            bNull=false;
            break;
        }
        if(bNull)
            tableStock->setItem(row,i,pItem);
    }
    tableStock->item(row,LIST_DATA_COLUMN)->setData(LIST_DATA_RECORD,QVariant((uint64_t)pRecord));
}
void StockSearchWidget::setTableRow(int row,const StockListItem* pStock)
{
    QTableWidgetItem*  pItem;
    bool bNull;
    QString title;
    for(int i=0;i<columnCount;i++)
    {
        pItem=tableStock->item(row,i);
        bNull=pItem==NULL;
        switch(columns[i])
        {
        case TableStockMember_Code:
            if(pItem)
                pItem->setText(pStock->getCode());
            else
            {
                pItem=new QTableWidgetItem(pStock->getCode());
                //tableStock->setItem(row,i,pItem);
            }
            break;
        case TableStockMember_Name:
            if(pItem)
                pItem->setText(pStock->getName());
            else{
                pItem=new QTableWidgetItem(pStock->getName());

            }
            break;
        case TableStockMember_Level:
            title=QString("%1").arg(pStock->getCommentLevel());
            if(pItem)
                pItem->setText(title);
            else{
                pItem=new QTableWidgetItem(title);
            }
            break;
        case TableStockMember_Comment:
            if(pItem)
                pItem->setText(pStock->getComment());
            else{
                pItem=new QTableWidgetItem(pStock->getComment());
            }
            break;
        default:
            bNull=false;
        }
        if(bNull)
            tableStock->setItem(row,i,pItem);
    }

    tableStock->item(row,LIST_DATA_COLUMN)->setData(LIST_DATA_STOCK,QVariant((uint64_t)pStock));
}

void StockSearchWidget::updateRecord(const StockListItem* pStock,data::StockRecordItem* pRecord)
{
    QList<QTableWidgetItem*> selected=tableStock->selectedItems();
    QList<QTableWidgetItem*>::iterator it=selected.begin(),end=selected.end();
    QTableWidgetItem* pTableItem;
    for(;it!=end;it++)
    {
        pTableItem=( *it);

        if(pTableItem->column()!=LIST_DATA_COLUMN)
            continue;
        QVariant var=pTableItem->data(LIST_DATA_STOCK);
        if(//pTableItem->column()==LIST_COLUMN_CODE&&
            ((uint64_t)pStock)== qvariant_cast<uint64_t>(var))//pTableItem->data(LIST_DATA_STOCK)))
        {
            setTableRow((*it)->row(),pRecord);
            break;
        }
    }
}

void StockSearchWidget::setStockToList(const QList<StockListItem*>* stocks)
{
    pNetPool->removeStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);
    stockItems.clear();
    stockItems.append(stocks->begin(),stocks->end());
    if(chkUpdateLastPrice->isChecked())
        pNetPool->addStock(stockItems,UPDATE_PRICE,WND_FALG_MAIN);

    tableStock->setRowCount(stocks->size());
    QList<StockListItem*>::const_iterator it=stocks->begin(),end=stocks->end();

    for(int i=0;it!=end;it++,i++)
    {
        setTableRow(i,*it);
    }
}

void StockSearchWidget::onTableCellClicked(int row,int column)
{
    Q_UNUSED(column);
    if(stockItems.size()==0)return;
    QTableWidgetItem* pItem=tableStock->item(row,LIST_DATA_COLUMN);
    StockListItem* p=(StockListItem*) qvariant_cast<uint64_t>(pItem->data(LIST_DATA_STOCK));
    emit stockClicked(p);
}
void StockSearchWidget::onDoubleClickStock(QModelIndex index)
{
    QTableWidgetItem* pItem=tableStock->item(index.row(),LIST_DATA_COLUMN);
    StockListItem* p=(StockListItem*) qvariant_cast<uint64_t>(pItem->data(LIST_DATA_STOCK));
    emit stockDbClicked(p);
}

void StockSearchWidget::onActionAddTodayStock(bool)
{
    QList<StockListItem*> stocks;
    getSelectedStocks(stocks);
    emit addTodayStock(stocks);
}


void StockSearchWidget::onActionStockNote(bool)
{
    QList<QTableWidgetItem*> items= tableStock->selectedItems();
    if(items.size()==0)return ;
    int row=items.at(0)->row();
    QTableWidgetItem* pTableItem=tableStock->item(row,LIST_DATA_COLUMN);
    StockListItem* pItem=(StockListItem*)qvariant_cast<uint64_t>(pTableItem->data(LIST_DATA_STOCK));
    if(pItem==NULL)return;
//LIST_DATA_NOTE
    StockNoteDlg* dlg=new StockNoteDlg(pItem,this);
    if(dlg->exec()==QDialog::Accepted)
    {
        dlg->getData(pItem);
        if(pDataLoader->getDB()->updateStockNote(pItem))
        {
            this->setTableRow(row,pItem);
        }
    }
    delete dlg;
    //emit stockNoteAdd(pItem);
}
void StockSearchWidget::onActionChartNew(bool)
{
    StockListItem* p=getCurrSelectStock();
    if(p==NULL) return;
    emit newStockWinCode(p);
}

StockListItem* StockSearchWidget::getCurrSelectStock()
{
    //QList<QTableWidgetItem*> list;= tableStock->selectedItems();
    QList<StockListItem*> list;
    getSelectedStocks(list);
    if(list.size()==0)return NULL;
    return list.at(0);
}

void StockSearchWidget::getSelectedStocks(QList<StockListItem*>& stocks)
{
    QList<QTableWidgetItem*> items=tableStock->selectedItems();
    getStockListItem(items,stocks);
}
void StockSearchWidget::getStockListItem(const QList<QTableWidgetItem*>& items,QList<StockListItem*>& stocks)
{
    QList<QTableWidgetItem*>::const_iterator it=items.begin(),end=items.end();
    StockListItem* p;
    for(;it!=end;it++)
    {
        if((*it)->column()!=LIST_DATA_COLUMN)continue;
        p=(StockListItem*)qvariant_cast<uint64_t>((*it)->data(LIST_DATA_STOCK));
        stocks.append(p);
    }
}
void StockSearchWidget::onActionRemoveStockFromList(bool)
{
    int count=tableStock->rowCount();
    if(count==0)return ;
    QTableWidgetItem* pItem;
    //Qt::ItemFlags flags;
    QVariant var;
    for(count-=1;count>=0;count--)
    {
        pItem=tableStock->item(count,LIST_DATA_COLUMN);

        if(pItem->isSelected())
        {
            //var=pItem->data(LIST_DATA_STOCK);
            //StockListItem* pStock=(StockListItem*)qvariant_cast<uint64_t>(var);

            if(recordList)
            {
                var=pItem->data(LIST_DATA_RECORD);
                data::StockRecordItem* record=(data::StockRecordItem*)qvariant_cast<uint64_t>(var);
                if(record)
                {
                    emit removeRecordFromList(recordList->getId(),record->getId());
                    recordList->removeItem(record);
                }
            }
            //recordListWidget->removeRecordItem(pStock->getCode());
            tableStock->removeRow(count);
        }
    }
    emit stockClicked(NULL);
}

void StockSearchWidget::onActionBlack(bool bin)
{
#ifdef DB_MYSQL
    BlackListDlg* dlg=new BlackListDlg(this);
    if(dlg->exec()!=QDialog::Accepted)
    {
        return ;
    }
    QDate date;
    dlg->getOutDate(date);
    delete dlg;

#endif
    QList<QTableWidgetItem*> selected=tableStock->selectedItems();
    QList<QTableWidgetItem*>::iterator it=selected.begin(),end=selected.end();
    QVariant var;
    StockListItem* pItem;
    for(;it!=end;it++)
    {
        if((*it)->column()!=LIST_DATA_COLUMN)continue;
        var=(*it)->data(LIST_DATA_STOCK);
        pItem=(StockListItem*)qvariant_cast<uint64_t>(var);
        if(pItem)
        {
#ifdef DB_MYSQL
            pDataLoader->getDB()->addBlackList(pItem,date);
#else
            pDataLoader->addBlackList(pItem->getCode());
#endif
        }
    }
    onActionRemoveStockFromList(bin);
}
void StockSearchWidget::onActionNewCompare(bool)
{
    QList<StockListItem*> list;
    getSelectedStocks(list);
    if(list.size()==0||list.size()>2)
    {
        return ;
    }
    StockListItem* p1=list.at(0);
    StockListItem* p2=NULL;
    if(list.size()>1)
        p2=list.at(1);
    emit newWinCompare(p1,p2);
}

void StockSearchWidget::onOpenRecordList(data::StockRecordList* p)
{
    // emit openRecordList(p);

    data::StockRecordList* list= new data::StockRecordList();
    list->setId(p->getId());
    list->setTitle(p->getTitle());
    list->setDatetime(p->getDatetime());
    list->setContent(p->getContent());
    list->setLevel(p->getLevel());

    //db->getRecord()
    list->loadItems(pDataLoader->getDB());
    setRecordList(list);

    pFilterDlg->deleteLater();
    pFilterDlg=NULL;
}

void StockSearchWidget::onFilterFinished(StockFilterDlg*)
{
    if(pFilterDlg)
    {
        delete pFilterDlg;
        pFilterDlg=NULL;
    }
}
void StockSearchWidget::onCloseList(bool)
{
    setRecordList(NULL);
}
void StockSearchWidget::onEditList(bool)
{
    if (recordList == NULL)return;
    StockRecordListDlg* dlg = new StockRecordListDlg(this);
    dlg->setRecordList(recordList);
    if (dlg->exec() == QDialog::Accepted)
    {
        dlg->getRecordList(recordList);
        pDataLoader->getDB()->updateRecordList(recordList);
    }
}
void StockSearchWidget::onDeleteList(bool)
{
    recordList->delItems(pDataLoader->getDB());

    pDataLoader->getDB()->delRecordList(recordList);
    setRecordList(NULL);
}

void StockSearchWidget::onFilterApply(StockFilterDlg* dlg)
{
    StockRecordListDlg* dlgNew=new StockRecordListDlg(this);
    //dlgNew->setStocks(stocks);
    if(dlgNew->exec()==QDialog::Accepted)
    {
        data::StockRecordList* recordList=new data::StockRecordList();
        recordList->setDatetime(QDateTime::currentDateTime());
        QList<data::StockRecordItem*> items;
        dlg->moveRecordItems(items);
        recordList->addItems(items);

        pDataLoader->getDB()->saveRecordListAndItem(recordList);

        setRecordList(recordList);
    }
    delete dlgNew;
    onFilterFinished(dlg);
}

