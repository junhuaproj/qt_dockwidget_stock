#include "mystockrecordlist.h"
#include "mysql_data_def.h"
#include "stockrecordlist.h"
#include <string>
#include "mydb.h"

#define STOCKRECORDLIST_FILED_COUNT     5

MyStockRecordList::MyStockRecordList(MyDB* db)
    :MyData(db)
{
}
MyStockRecordList::~MyStockRecordList()
{

}

void MyStockRecordList::del(const data::StockRecordList* record)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"delete from t_stocklist where id=%d",record->getId());
    execute_sql(szSQL);
}

bool MyStockRecordList::bindValue(const data::StockRecordList* record)
{
    resetBind(STOCKRECORDLIST_FILED_COUNT);
    clearBindValue();

    bind_param_time(0,record->getDatetime());
    bind_param_int(1,record->getLevel());

    bind_param_string(2,RECORD_TITLE_LEN,record->getTitle());

    bind_param_string(3,RECORD_CONTENT_LEN,record->getContent());

    if (!bind_param())
    {
        return false;
    }
    if (!exec())
    {
        return false;
    }
    return true;
}

bool MyStockRecordList::add(data::StockRecordList* record)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"insert into t_stocklist(crdate,level,title,content)"
                   "values(?,?,?,?)");
    bool ret=false;
    if(init_stmt(szSQL))
    {
        ret=bindValue(record);
        record->setId(mysql_stmt_insert_id(stmt));
    }
    deInit_stmt();
    return ret;
}

bool MyStockRecordList::update(const data::StockRecordList* record)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"update t_stocklist set crdate=?,level=?,title=?,content=? "
                   "where id=%d",record->getId());
    bool ret=false;
    if(init_stmt(szSQL))
    {
        ret=bindValue(record);
    }
    // mysql_stmt_affected_rows(stmt);

error:
    deInit_stmt();
    return ret;
}

bool MyStockRecordList::getLists(const QDateTime& begin,const QDateTime& end,QList<data::StockRecordList*>& list,int level)
{
    QString sql=QString("select id,crdate,level,title,content from t_stocklist where crdate between '%1' and '%2'")
                      .arg(begin.toString("yyyy-MM-dd")).arg(end.toString("yyyy-MM-dd"));
    if( level!=-1)
    {
        sql=sql.append(QString(" and level=%1").arg(level));
    }
    std::string strSQL=sql.toStdString();
    return getLists(strSQL.c_str(),list);
}

bool MyStockRecordList::getListsByWhere(const QString& where,QList<data::StockRecordList*>& list)
{
    std::string strWhere=where.toStdString();
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select id,crdate,level,title,content from t_stocklist "
                   " %s",strWhere.c_str());
    return getLists(szSQL,list);
}

bool MyStockRecordList::getLists(const char* sql,QList<data::StockRecordList*>& list)
{
    if(!init_stmt(sql))
        return false;
    if (!exec())
    {
        return false;
    }
    resetBind(STOCKRECORDLIST_FILED_COUNT);
    clearBindValue();
    BindValue* id=bind_result_int(0);
    BindValue* crdate=bind_result_time(1);
    BindValue* level=bind_result_int(2);
    BindValue* title=bind_result_string(3,RECORD_TITLE_LEN);

    BindValue* content=bind_result_string(4,RECORD_CONTENT_LEN);

    //ret=mysql_stmt_bind_result(stmt,bind);
    //qDebug()<<szSQL;
    if(!bind_result())
    {
        return false;
    }
    //ret=mysql_stmt_store_result(stmt);
    if(!store_result())
    {
        return false;
    }

    QDateTime datetime;
    while(nextRow())
    {
        data::StockRecordList* item=new data::StockRecordList();

        item->setId(id->i32Value);
        item->setTitle(title->szBuf);
        item->setLevel(level->i32Value);
        item->setContent(content->szBuf);
        mysql_timeToQDateTime(crdate->pTime,datetime);
        item->setDatetime(datetime);

        list.append(item);
    }

    deInit_stmt();
    return true;
}
