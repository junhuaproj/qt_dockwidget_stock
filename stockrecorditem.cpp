#include "stockrecorditem.h"
#include <string>
namespace data {
StockRecordItem::StockRecordItem()
    :id(0),parent(0),level(0),pStock(NULL)
{}

void StockRecordItem::setStock(const StockListItem* stock)
{
    pStock=stock;
}

float StockRecordItem::getDownPercent() const
{
    return downPercent;
}

void StockRecordItem::setDownPercent(float newDownPercent)
{
    downPercent = newDownPercent;
}

int StockRecordItem::getDownDay() const
{
    return downDay;
}

void StockRecordItem::setDownDay(int newDownDay)
{
    downDay = newDownDay;
}

float StockRecordItem::getLastKeyVal() const
{
    return lastKeyVal;
}

void StockRecordItem::setLastKeyVal(float newLastKeyVal)
{
    lastKeyVal = newLastKeyVal;
}
const QDateTime& StockRecordItem::getDatetime() const
{
    return datetime;
}

void StockRecordItem::setDatetime(const QDateTime &newDatetime)
{
    datetime = newDatetime;
}
#ifdef DB_MYSQL
#else
const QString&  StockRecordItem::getCode() const
{
    return code;
}

void StockRecordItem::setCode(const QString &newCode)
{
    code = newCode;
}
#endif
// const QString& StockRecordItem::getText() const
// {
//     return text;
// }

void StockRecordItem::setText(const QString &newText)
{
    text = newText;
}

const QString&  StockRecordItem::getKey() const
{
    return key;
}

void StockRecordItem::setKey(const QString &newKey)
{
    key = newKey;
}

const QString&  StockRecordItem::getCondtion() const
{
    return condtion;
}

void StockRecordItem::setCondtion(const QString &newCondtion)
{
    condtion = newCondtion;
}

int StockRecordItem::getId() const
{
    return id;
}

void StockRecordItem::setId(int newId)
{
    id = newId;
}

int StockRecordItem::getParent() const
{
    return parent;
}

void StockRecordItem::setParent(int newParent)
{
    parent = newParent;
}

int StockRecordItem::getLevel() const
{
    return level;
}

void StockRecordItem::setLevel(int newLevel)
{
    level = newLevel;
}

char* StringToStr(const QString& s)
{
    std::string stdstr=s.toStdString();
    char* buf=(char*)malloc(stdstr.size()+1);
    strcpy(buf,stdstr.c_str());
    return buf;
}

bool find_id_from_record_func(StockRecordItem* p,StockListItem* stock)
{
    return p->getStock()==stock;
}

}
