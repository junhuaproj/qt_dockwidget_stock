#include "stockfilterdlg.h"
#include "ui_stockfilterdlg.h"

#include <QJsonDocument>
#include <QJsonArray>
#include "dataloader.h"
//#include "stockrecorddb.h"
#include "stockrecorditem.h"
#include "stockrecordlist.h"
#include "appconfig.h"
#include "AppConfigqss.h"
#include <QFileDialog>
#include "data/parserrecordlist.h"

#define TABLE_DATA_COL_2    (Qt::UserRole+1)
#define TABLE_DATE_ITEM     (Qt::UserRole+2)
#define TABLE_DATA_LIST     (Qt::UserRole+3)

StockFilterDlg::StockFilterDlg(data::DataLoader* pLoad,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::StockFilterDlg),pDataLoad(pLoad),sortColumn(-1),isList(false)
{
    ui->setupUi(this);
    connect(ui->pushButtonParse,SIGNAL(clicked(bool)),this,SLOT(onParse(bool)));
    connect(ui->pushFilter,SIGNAL(clicked(bool)),this,SLOT(onFilter(bool)));
    connect(ui->pushSearch,SIGNAL(clicked(bool)),this,SLOT(onSearch(bool)));
    connect(ui->tableWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onTableDbClick(QModelIndex)));
    connect(ui->pushButtonFromFile,SIGNAL(clicked(bool)),this,SLOT(onParseTable(bool)));
    connect(ui->pushButtonDelete,SIGNAL(clicked(bool)),this,SLOT(onDelete(bool)));

    connect(ui->pushButtonOpenFile,SIGNAL(clicked(bool)),this,SLOT(onOpenFile(bool)));
    connect(ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(onSectionClicked(int)));

    QDateTime curr=QDateTime::currentDateTime();
    ui->dateEditEnd->setDateTime(curr.addDays(1));
    curr=curr.addDays(-1);
    ui->dateEditBegin->setDateTime(curr);
    appConfig.updateQss(this);
    //reloadQss("");
}
StockFilterDlg::~StockFilterDlg()
{
    clearItemAndList();
    delete ui;
}

void StockFilterDlg::moveRecordItems(QList<data::StockRecordItem*>& out)
{
    out.append(items);
    items.clear();
    setTabRecordItem();
}

void StockFilterDlg::setTabRecordItem()
{
    isList=false;
    ui->tableWidget->setRowCount(items.size());
    data::StockRecordItem* item;
    QTableWidgetItem* pTableItem;
    QList<data::StockRecordItem*>::iterator it= items.begin(),end=items.end();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels({tr("name")});
    for(int row=0;it!=end;it++,row++)
    {
        item=*it;
        pTableItem=new QTableWidgetItem(item->getStock()->getName());
        pTableItem->setData(TABLE_DATE_ITEM,QVariant((uint64_t)item));
        ui->tableWidget->setItem(row,0,pTableItem);
    }
}
void StockFilterDlg::onOpenFile(bool)
{
    clearItemAndList();
    ParserRecordList parser(pDataLoad);
    if(parser.openAndParseFile(items,this))
    {
        setTabRecordItem();
    }
}

void StockFilterDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(ui->checkBoxCondtion);
    this->getDlgBoxButtons(wids,ui->buttonBox);
    wids.push_back(ui->checkBoxContent);
    wids.push_back(ui->checkBoxDatetime);
    wids.push_back(ui->checkBoxKey);
    wids.push_back(ui->checkBoxLevel);
    wids.push_back(ui->checkBoxRemoveST);
    wids.push_back(ui->checkBoxTitle);
    wids.push_back(ui->dateEditBegin);
    wids.push_back(ui->dateEditEnd);
    wids.push_back(ui->lineCondition);
    wids.push_back(ui->lineContent);
    wids.push_back(ui->lineKey);
    wids.push_back(ui->lineLevel);
    wids.push_back(ui->lineTitle);
    wids.push_back(ui->pushButtonDelete);
    wids.push_back(ui->pushButtonFromFile);
    wids.push_back(ui->pushButtonParse);
    wids.push_back(ui->pushFilter);
    wids.push_back(ui->pushSearch);
    wids.push_back(ui->radioList);
    wids.push_back(ui->radioStock);
    wids.push_back(ui->tableWidget);
    //wids.push_back(ui->tabRecordList);
    wids.push_back(ui->tabWidget);
    wids.push_back(ui->pushButtonOpenFile);
    getDlgBoxButtons(wids,ui->buttonBox);
}

void StockFilterDlg::onSectionClicked(int index)
{
    if(index==sortColumn)
    {
        if(sortOrder==Qt::AscendingOrder)
            sortOrder=Qt::DescendingOrder;
        else
            sortOrder=Qt::AscendingOrder;
    }
    ui->tableWidget->sortByColumn(index,sortOrder);
    sortColumn=index;
}
void StockFilterDlg::onDelete(bool)
{
    QList<QTableWidgetItem*> items= ui->tableWidget->selectedItems();
    QTableWidgetItem* item;
    QList<int> rows;
    for(int i=items.size()-1;i>=0;i--)
    {
        item=items.at(i);
        if(std::find(rows.begin(),rows.end(),item->row())==rows.end())
        {
            rows.append(item->row());
        }
    }
    QVariant var;
    if(isList)
    {
        QTableWidgetItem* item;
        data::StockRecordList* list;
        QList<int>::reverse_iterator it=rows.rbegin(),end=rows.rend();
        for(;it!=end;it++)
        {
            var=item->data(TABLE_DATA_LIST);
            list=(data::StockRecordList*)qvariant_cast<uint64_t>(var);
            this->lists.removeOne(list);

            ui->tableWidget->removeRow(*it);
        }
    }
    else
    {
        QList<int>::reverse_iterator it=rows.rbegin(),end=rows.rend();
        QTableWidgetItem* item;
        data::StockRecordItem* pItem;
        for(;it!=end;it++)
        {
            item=ui->tableWidget->item(*it,0);
            var=item->data(TABLE_DATE_ITEM);
            pItem=(data::StockRecordItem*)qvariant_cast<uint64_t>(var);
            this->items.removeOne(pItem);
            //TABLE_DATE_ITEM
            ui->tableWidget->removeRow(*it);
        }
    }
}

void StockFilterDlg::onParseTable(bool)
{

}

void StockFilterDlg::parseIndex(const QString& s,QStringList& names)
{
    QJsonDocument doc=QJsonDocument::fromJson(s.toUtf8());
    if(doc.isArray())
    {
        QJsonArray arr=doc.array();
        for(int i=0;i<arr.size();i++)
        {
            names.append(arr.at(i).toObject().value("name").toString());
        }
    }
}

void StockFilterDlg::onTableDbClick(const QModelIndex &index)
{
    QTableWidgetItem* pItem= ui->tableWidget->item(index.row(),0);
    QVariant var;
    if(isList)
    {
        var=pItem->data(TABLE_DATA_LIST);
        data::StockRecordList* list=(data::StockRecordList*)qvariant_cast<uint64_t>(var);
        if(list!=NULL)
        {
            emit openRecordList(list);
        }
        /*else
        {
            var=pItem->data(TABLE_DATA_COL_2);
            StockListItem* item=(StockListItem*)qvariant_cast<uint64_t>(var);
            if(item)
                emit selectStock(item);
        }*/
    }
    else
    {
        /*var=pItem->data(TABLE_DATE_ITEM);
        data::StockRecordItem* list=(data::StockRecordItem*)qvariant_cast<uint64_t>(var);
        if(list!=NULL)
        {
            emit openRecordList(list);
        }*/
    }
}

void StockFilterDlg::clearItemAndList()
{
    ui->tableWidget->clear();
    while(!items.isEmpty())
    {
        delete items.back();
        items.pop_back();
    }
    while(!lists.isEmpty())
    {
        delete lists.back();
        lists.pop_back();
    }
}
void StockFilterDlg::onSearch(bool)
{
    QString where;
    clearItemAndList();
    ui->tableWidget->setRowCount(0);
    if(ui->checkBoxDatetime->isChecked())
    {
        QDateTime begin=ui->dateEditBegin->dateTime(),end=ui->dateEditEnd->dateTime();
        where=QString("crdate between %1 and %2")
                    .arg(begin.toString(date_format))
                    .arg(end.toString(date_format));

        //pDataLoad->getRecordDb()->getStockRecords(items,&begin,&end);
    }
    if(where.length()==0)
        where="where 1=1";
    if(ui->radioList->isChecked())
    {
        isList=true;
#ifdef DB_MYSQL

        pDataLoad->getDB()->getRecordLists(where,lists);
#else
        QString sql=QString("select rowid,crdate,level,title,content from stocklist %1")
                          .arg(where);
        pDataLoad->getRecordDb()->getRecordLists(sql,lists);
#endif
        QList<data::StockRecordList*>::iterator it= lists.begin(),end=lists.end();
        ui->tableWidget->setRowCount(lists.size());
        ui->tableWidget->setColumnCount(4);
        QTableWidgetItem* pTableItem;
        for(int row=0;it!=end;it++,row++)
        {
            pTableItem=new QTableWidgetItem((*it)->getTitle());
            pTableItem->setData(TABLE_DATA_LIST,QVariant((uint64_t)(*it)));
            ui->tableWidget->setItem(row,0,pTableItem);

            pTableItem=new QTableWidgetItem((*it)->getDatetime().toString(datetime_format));
            ui->tableWidget->setItem(row,1,pTableItem);

            pTableItem=new QTableWidgetItem(QString("%1").arg((*it)->getLevel()));
            ui->tableWidget->setItem(row,2,pTableItem);

            pTableItem=new QTableWidgetItem((*it)->getContent());
            ui->tableWidget->setItem(row,3,pTableItem);
        }
    }
#if 0
    else
    {
#ifdef DB_MYSQL
        pDataLoad->getRecordDb()->getStockRecords(where,items);
        QList<data::StockRecordItem*>::iterator it=items.begin(),endit=items.end();
        QList<StockListItem*> stocks;
        //QStringList codes;
        for(;it!=endit;it++)
        {
            //去除重复的
            if(stocks.end()==std::find(stocks.begin(),stocks.end(),(*it)->getStock()))
                stocks.append((StockListItem*)(*it)->getStock());

            //row++;
        }
        while(!items.isEmpty())
        {
            delete items.back();
            items.pop_back();
        }
        setResultCodes(stocks);
#else
        QString sql=QString("select rowid,crdate,parent,level,code,content,keyval,condtion from stockrecord %1")
                          .arg(where);

        pDataLoad->getRecordDb()->getStockRecords(sql,items);

        QList<data::StockRecordItem*>::iterator it=items.begin(),endit=items.end();
        QStringList codes;
        for(;it!=endit;it++)
        {
            //去除重复的
            if(codes.end()==std::find(codes.begin(),codes.end(),(*it)->getCode()))
                codes.append((*it)->getCode());

            //row++;
        }
        while(!items.isEmpty())
        {
            delete items.back();
            items.pop_back();
        }
        setResultCodes(codes);
#endif
    }
#endif
}
#if 0
void StockFilterDlg::getResultList(QList<StockListItem*>& lists)
{
    //if(ui->tableWidget->columnCount()<2)return;
    if(this->lists.size()==0)
    {
        int rowCount=ui->tableWidget->rowCount();
        QTableWidgetItem* pItem;
        QVariant var;
        StockListItem* pStock;
        for(int i=0;i<rowCount;i++)
        {
            pItem=ui->tableWidget->item(i,0);
            var=pItem->data(TABLE_DATA_COL_2);
            pStock=(StockListItem*)qvariant_cast<uint64_t>(var);
            if(pStock)
            {
                lists.append(pStock);
            }
        }
    }
    else// if(lists.size()>0)
    {
        QList<data::StockRecordList*>::iterator it=this->lists.begin(),end=this->lists.end();

        for(;it!=end;it++)
        {
            pDataLoad->getRecordDb()->getStockRecordsByParent((*it)->getId(),items);
        }
        QList<data::StockRecordItem*>::iterator itItem=items.begin(),endit=items.end();
#ifdef DB_MYSQL
        //QStringList codes;
        for(;itItem!=endit;itItem++)
        {
            //去除重复的
            if(lists.end()==std::find(lists.begin(),lists.end(),(*itItem)->getStock()))
                lists.append((StockListItem*)(*itItem)->getStock());

            //row++;
        }

        //pDataLoad->findStockList(codes,&lists);
#else
        QStringList codes;
        for(;itItem!=endit;itItem++)
        {
            //去除重复的
            if(codes.end()==std::find(codes.begin(),codes.end(),(*itItem)->getCode()))
                codes.append((*itItem)->getCode());

            //row++;
        }

        pDataLoad->findStockList(codes,&lists);
#endif
    }
}
#endif
void StockFilterDlg::onFilter(bool)
{
    emit apply(this);
}
void StockFilterDlg::closeEvent(QCloseEvent* event)
{
    QDialog::closeEvent(event);
    emit closed(this);
}
void StockFilterDlg::onParse(bool)
{
#if 0
    QString text=ui->textEditJson->toPlainText();
    QJsonDocument doc=QJsonDocument::fromJson(text.toUtf8());
    if(doc.isArray())
    {
        QJsonArray arr=doc.array();

        QString code;
        QStringList codes;
        for(int i=0;i<arr.size();i++)
        {
            QJsonValue item=arr.at(i);
            /*if(item.isArray())
            {
                QJsonArray itemAttrs=item.toArray();
                if(col<itemAttrs.size())
                {
                    col=itemAttrs.size();
                    ui->tableWidget->setColumnCount(col);
                }
                for(int j=0;j<itemAttrs.size();j++)
                {

                }
            }
            else
            {*/
            code=item.toString();
#ifdef DB_MYSQL
            codes.append(code);
#else
            if(!pDataLoad->isBlackList(code))
                codes.append(code);
#endif
            //pItem=new QTableWidgetItem(item.toString());
            //ui->tableWidget->setItem(i,0,pItem);
            //}
        }
#ifdef DB_MYSQL
        QList<StockListItem*> stocks;
        pDataLoad->findStockList(codes,&stocks);
        setResultCodes(stocks);
#else
        setResultCodes(codes);
#endif
    }
#endif
}

#ifdef DB_MYSQL
void StockFilterDlg::setResultCodes(const QList<StockListItem*>& stocks)
{
#if 0
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(stocks.size());
    QTableWidgetItem* pItem;
    StockListItem* pStock;

    for(int i=0;i<stocks.size();i++)
    {
        QString colText;

        pStock=stocks.at(i);
        pItem=new QTableWidgetItem();
        if(pStock)
        {
            if(pStock->getName().contains("ST")&&!ui->checkBoxRemoveST->isChecked())
                colText=colText+tr("ST--")+pStock->getName();
            //pItem=new QTableWidgetItem(tr("ST--")+pStock->getName());
            else
            {
                colText=colText+pStock->getName();
                //pItem=new QTableWidgetItem(pStock->getName());
                //pItem->setData(TABLE_DATA_COL_2,QVariant((uint64_t)pStock));
            }
            pItem->setData(TABLE_DATA_COL_2,QVariant((uint64_t)pStock));
            //ui->tableWidget->setItem(i,1,pItem);
        }
        pItem->setText(colText);
        ui->tableWidget->setItem(i,0,pItem);
    }
#endif
}
#else
void StockFilterDlg::setResultCodes(const QStringList& codes)
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(codes.size());
    QTableWidgetItem* pItem;
    StockListItem* pStock;
    QList<StockListItem*> stocks;
    pDataLoad->findStockList(codes,&stocks);
    for(int i=0;i<codes.size();i++)
    {
        QString colText;
        colText=codes.at(i);

        pStock=stocks.at(i);
        pItem=new QTableWidgetItem();
        if(pStock)
        {
            if(pStock->getName().contains("ST")&&!ui->checkBoxRemoveST->isChecked())
                colText=colText+tr("ST--")+pStock->getName();
                //pItem=new QTableWidgetItem(tr("ST--")+pStock->getName());
            else
            {
                colText=colText+pStock->getName();
                //pItem=new QTableWidgetItem(pStock->getName());
                //pItem->setData(TABLE_DATA_COL_2,QVariant((uint64_t)pStock));
            }
            pItem->setData(TABLE_DATA_COL_2,QVariant((uint64_t)pStock));
            //ui->tableWidget->setItem(i,1,pItem);
        }
        pItem->setText(colText);
        ui->tableWidget->setItem(i,0,pItem);
    }
}
#endif
