#include "mystockrecord.h"
#include "mysql_data_def.h"
#include "stockrecorditem.h"
#include "stocklistitem.h"
#include "dataloader.h"
#include <string>
#include "mydb.h"

#define STOCKRECORD_FILED_COUNT     8
MyStockRecord::MyStockRecord(MyDB* db)
    :MyData(db),pDataLoader(NULL)
{
}

MyStockRecord::~MyStockRecord()
{

}

void MyStockRecord::del(const data::StockRecordItem* item)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL," delete from t_stocklistitem where id=%d",item->getId());
    execute_sql(szSQL);
}
bool MyStockRecord::gets(QList<data::StockRecordItem*>& items)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select id,crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper from t_stocklistitem");
    return gets(szSQL,items);
}

bool MyStockRecord::update(const data::StockRecordItem* item)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"update t_stocklistitem set crdate=?,parent=?,stockid=?,level=?,content=?,keyword=?,tradecondition=?,downday=?,lastkeyval=? downper=? "
                   "where id=%d",item->getId());
    bool ret=false;
    if(init_stmt(szSQL))
    {
        ret=bindValue(item);
    }
    deInit_stmt();
    return ret;
}
void MyStockRecord::setDataLoader(data::DataLoader* pDataLoader)
{
    this->pDataLoader=pDataLoader;
}
bool MyStockRecord::bindValue(const data::StockRecordItem* record)
{
    bind_param_time(0,record->getDatetime());
    bind_param_int(1,record->getParent());
    bind_param_int(2,record->getStock()->getId());
    bind_param_int(3,record->getLevel());
    bind_param_string(4,RECORD_CONTENT_LEN,record->getText());
    bind_param_string(5,RECORD_KEYWORD_LEN,record->getKey());
    bind_param_string(6,RECORD_CONDITION_LEN,record->getCondtion());
    bind_param_int(7,record->getDownDay());
    bind_param_float(8,record->getLastKeyVal());
    bind_param_float(9,record->getDownPercent());

    if (!bind_param())
    {
        return false;
    }
    if(!exec())
    {
        return false;
    }
    return true;
}
bool MyStockRecord::add(data::StockRecordItem* item)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"insert into t_stocklistitem(crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper)"
                   "values(?,?,?,?,?,?,?,?,?,?)");
    bool ret=false;
    if(init_stmt(szSQL))
    {
        resetBind(10);
        clearBindValue();
        ret=bindValue(item);
        // mysql_stmt_affected_rows(stmt);
        item->setId(mysql_stmt_insert_id(stmt));
    }
error:
    deInit_stmt();
    return ret;
}

bool MyStockRecord::adds(QList<data::StockRecordItem*>& items)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"insert into t_stocklistitem(crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper)"
                   "values(?,?,?,?,?,?,?,?,?,?)");
    QList<data::StockRecordItem*>::iterator it,end;

    if(!init_stmt(szSQL))
    {
        return false;
    }
    resetBind(10);
    clearBindValue();
    it=items.begin();
    end=items.end();
    for(;it!=end;it++)
    {
        data::StockRecordItem* item=*it;
        bindValue(item);

        item->setId(mysql_stmt_insert_id(stmt));
    }

error:
    deInit_stmt();
    //mysql_stmt_close(stmt);
    return true;
}

bool MyStockRecord::gets(const char* sql,QList<data::StockRecordItem*>& items)
{
    int ret;
    QDateTime datetime;

    if(!init_stmt(sql))
    {
        return false;
    }
    if (!exec())
    {
        return false;
    }
    resetBind(11);
    clearBindValue();
    BindValue* id=bind_result_int(0);
    BindValue* crdate=bind_result_time(1);
    BindValue* parent=bind_result_int(2);
    BindValue* stockid=bind_result_int(3);
    BindValue* level=bind_result_int(4);
    BindValue* content=bind_result_string(5,RECORD_CONTENT_LEN);
    BindValue* keyword=bind_result_string(6,RECORD_KEYWORD_LEN);
    BindValue* condition=bind_result_string(7,RECORD_CONDITION_LEN);
    BindValue* downDay=bind_result_int(8);
    BindValue* lastKeyVal=bind_result_float(9);
    BindValue* downPercent=bind_result_float(10);

    if(!bind_result())
    {
        return false;
    }

    if(!store_result())
    {
        return false;
    }

    while(nextRow())
    {
        data::StockRecordItem* item=new data::StockRecordItem();

        item->setId(id->i32Value);
        item->setKey(keyword->szBuf);
        item->setCondtion(condition->szBuf);
        item->setLevel(level->i32Value);
        item->setParent(parent->i32Value);
        item->setText(content->szBuf);
        item->setStock(pDataLoader->findStockById(stockid->i32Value));
        mysql_timeToQDateTime(crdate->pTime,datetime);
        item->setDatetime(datetime);
        item->setDownDay(downDay->i32Value);
        item->setLastKeyVal(lastKeyVal->fValue);
        item->setDownPercent(downPercent->fValue);

        items.append(item);
    }
error:
    deInit_stmt();
    return true;
}

bool MyStockRecord::getsByWhere(const QString& where,QList<data::StockRecordItem*>& items)
{
    std::string strWhere=where.toStdString();
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select id,crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper "
                   "from t_stocklistitem %s",strWhere.c_str());
    return gets(szSQL,items);
}

bool MyStockRecord::getsByParent(int parent,QList<data::StockRecordItem*>& items)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select id,crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper "
                   "from t_stocklistitem where parent=%d",parent);
    return gets(szSQL,items);
}
bool MyStockRecord::gets(QList<data::StockRecordItem*>& items,const QDateTime* begin,const QDateTime* end)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select id,crdate,parent,stockid,level,content,keyword,tradecondition,downday,lastkeyval,downper "
                   "from t_stocklistitem ");

    if(begin&&end)
    {
        QString where=QString(" where crdate between '%1' and '%2'")
                             .arg(begin->toString("yyyy-MM-dd")).arg(end->toString("yyyy-MM-dd"));
        std::string strWhere=where.toStdString();
        strcat(szSQL,strWhere.c_str());
    }
    return gets(szSQL,items);
}
