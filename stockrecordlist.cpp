#include "stockrecordlist.h"

//#include "stockrecorddb.h"
#include "mysqldata/mystockrecord.h"
namespace data {
StockRecordList::StockRecordList(QObject *parent)
    : QObject(parent),id(0),level(0)
{}

StockRecordList::~StockRecordList()
{
    clear();
}
void StockRecordList::clear()
{
    while(!items.isEmpty())
    {
        delete items.back();
        items.pop_back();
    }
}
int StockRecordList::getId() const
{
    return id;
}

void StockRecordList::setId(int newId)
{
    id = newId;
}

const QString& StockRecordList::getTitle() const
{
    return title;
}

void StockRecordList::setTitle(const QString &newTitle)
{
    title = newTitle;
}

const QDateTime& StockRecordList::getDatetime() const
{
    return datetime;
}

void StockRecordList::setDatetime(const QDateTime &newDatetime)
{
    datetime = newDatetime;
}

const QString& StockRecordList::getContent() const
{
    return content;
}

void StockRecordList::setContent(const QString &newContent)
{
    content = newContent;
}

int StockRecordList::getLevel() const
{
    return level;
}

void StockRecordList::setLevel(int newLevel)
{
    level = newLevel;
}
#ifdef DB_MYSQL
#else
void StockRecordList::removeItem(const QString& code)
{
    StockRecordItem* pItem=findRecord(code);
    if(pItem!=NULL)
    {
        items.removeOne(pItem);
        delete pItem;
    }
}
#endif
void StockRecordList::removeItem(StockRecordItem* p)
{
    items.removeOne(p);
    delete p;
}

void StockRecordList::addItem(StockRecordItem * pItem)
{
    items.append(pItem);
}
void StockRecordList::addItems(const QList<StockRecordItem *> &newItems)
{
    items.append(newItems);
}
const QList<StockRecordItem *>& StockRecordList::getItems() const
{
    return items;
}

void StockRecordList::updateSubItem()
{
    QList<StockRecordItem *>::iterator it=items.begin(),end=items.end();
    QDateTime now=QDateTime::currentDateTime();
    for(;it!=end;it++)
    {
        (*it)->setParent(id);
        (*it)->setDatetime(now);
    }
}
QList<StockRecordItem *>& StockRecordList::getDeepItems()
{
    return items;
}
bool StockRecordList::isRecordExist(const StockRecordItem* pItem)
{
    return std::find(items.begin(),items.end(),pItem)!=items.end();
}
void StockRecordList::setItems(const QList<StockRecordItem *> &newItems)
{
    items = newItems;
}

StockRecordItem* StockRecordList::findRecord(const StockListItem* p)
{
    QList<StockRecordItem *>::iterator it=items.begin(),end=items.end();
    for(;it!=end;it++)
    {
        if((*it)->getStock()==p)
            return *it;
    }
    return NULL;
}

void StockRecordList::saveItems(MyDB* db)//data::StockRecordDb* db)
{
    QList<StockRecordItem *>::iterator it=items.begin(),end=items.end();
    MyStockRecord* record= db->getRecord();
    for(;it!=end;it++)
    {
        if((*it)->getId())
        {
            record->update(*it);
        }
        else
        {
            (*it)->setParent(id);
            record->add(*it);
            //(*it)->setId(db->addRecord(*it));
        }
    }
    delete record;
}
void StockRecordList::delItems(MyDB* db)
{
    MyStockRecord* record= db->getRecord();
    while(!items.isEmpty())
    {
        //db->delRecord(items.back());
        record->del(items.back());
        delete items.back();
        items.pop_back();
    }
    delete record;
}

void StockRecordList::loadItems(MyDB* db)//data::StockRecordDb* db)
{
    clear();
    MyStockRecord* record= db->getRecord();
    record->getsByParent(id,items);
    delete record;
    //db->getStockRecordsByParent(id,items);
}
}
