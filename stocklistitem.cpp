#include "stocklistitem.h"

StockListItem::StockListItem():flag(0)
#ifdef DB_MYSQL
    ,id(0)
#endif
{

}
#ifdef DB_MYSQL

void StockListItem::setId(int newId)
{
    this->id=newId;
}

QString StockListItem::getComment() const
{
    return comment;
}

void StockListItem::setComment(const QString &newComment)
{
    comment = newComment;
}

QDate StockListItem::getCommentDate() const
{
    return commentDate;
}

void StockListItem::setCommentDate(const QDate &newCommentDate)
{
    commentDate = newCommentDate;
}

int StockListItem::getCommentLevel() const
{
    return commentLevel;
}

void StockListItem::setCommentLevel(int newCommentLevel)
{
    commentLevel = newCommentLevel;
}

#endif
StockListItem::StockListItem(const StockListItem& item)
    :code(item.getCode()),name(item.getName()),flag(item.getFlag())
#ifdef DB_MYSQL
    ,id(item.getId())
#endif
{

}

StockListItem::StockListItem(const QString& c,const QString& n,uint32_t f)
    :code(c),name(n),flag(f)
#ifdef DB_MYSQL
    ,id(0)
#endif
{

}
bool StockListItem::isShanghaiZS()const
{
    return (flag&StockItemFlag_ZS)&&code.at(0)=='0';
}

bool StockListItem::isShenzhenZS()const
{
    return (flag&StockItemFlag_ZS)&&code.at(0)=='3';
}
const QString& StockListItem::getCode() const
{
    return code;
}

void StockListItem::setCode(const QString &newCode)
{
    code = newCode;
}

const QString& StockListItem::getName() const
{
    return name;
}

bool StockListItem::filter(uint32_t flag,const QString& key)
{
    if(flag&&(this->flag&flag)!=flag)
        return false;
    if(key.isEmpty())
        return true;
    else if(code.contains(key)||name.contains(key))
    {
        return true;
    }
    return false;
}

void StockListItem::setName(const QString &newName)
{
    name = newName;
}

uint32_t StockListItem::getFlag() const
{
    return flag;
}

void StockListItem::setFlag(uint32_t newFlag)
{
    flag = newFlag;
}
bool StockListItem::isShanghai()const
{
    return ((flag&(StockItemFlag_CN|StockItemFlag_SH))==(StockItemFlag_CN|StockItemFlag_SH));
}
bool StockListItem::isShenzhen()const
{
    return ((flag&(StockItemFlag_CN|StockItemFlag_SZ))==(StockItemFlag_CN|StockItemFlag_SZ));
}

void StockListItem::toString(QString& str)const
{
    str=getCode();
    str.append(getName());
}

void StockListItem::set(const StockListItem* src)
{
    if(this==src)return;
    if(src==NULL)
    {
#ifdef DB_MYSQL
        setId(0);
#endif
        flag=0;
        code="";
        name="";
    }
    else
    {
#ifdef DB_MYSQL
        setId(src->getId());
#endif
        flag=src->getFlag();
        code=src->getCode();
        name=src->getName();
    }
}
