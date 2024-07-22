#include "mydb.h"
#include "mystocklistitem.h"
#include "mystockindex.h"
#include "mystockrecord.h"
#include "mystockrecordlist.h"
#include "myblackliststock.h"
#include <string>
#include "mystockprop.h"
#include "stockrecordlist.h"

MyDB::MyDB(data::DataLoader* pDataLoader):db(NULL),dataLoader(pDataLoader)
{
    szSQL=(char*)malloc(512);
}

MyDB::~MyDB()
{
    close();
    delete szSQL;
}
char* MyDB::getSQLBuf(int* len)
{
    *len=512;
    return szSQL;
}


void MyDB::getNameByCode(const char* sql,QList<StockProp*>& result)
{
    //sprintf(szSQL,"select id,name from %s where code='%s'",table,strCode.c_str());
    MYSQL_STMT* stmt = mysql_stmt_init(db);
    if(stmt==NULL)return ;

    int ret=mysql_stmt_prepare(stmt,sql,strlen(sql));
    if(ret)
    {
        //showError(ret);
        goto error ;
    }
    ret=mysql_stmt_execute(stmt);
    if (ret)
    {
        //showError(ret);
        goto error ;
    }
    MYSQL_BIND bind[2];
    bool is_null[2];
    char szName[64];
    unsigned long length[2];
    int id;
    memset(bind,0,sizeof(MYSQL_BIND)*2);
    bind[0].buffer_type=MYSQL_TYPE_LONG;
    bind[0].buffer=(char*)&id;
    bind[0].is_null=&is_null[0];
    bind[0].length=&length[0];

    bind[1].buffer_type=MYSQL_TYPE_VAR_STRING;
    bind[1].buffer=szName;
    bind[1].buffer_length=64;
    bind[1].is_null=&is_null[1];
    bind[1].length=&length[1];

    ret=mysql_stmt_bind_result(stmt,bind);
    //qDebug()<<szSQL;
    if(ret)
    {
        //showError(ret);
        goto error ;
    }
    ret=mysql_stmt_store_result(stmt);
    if(ret)
    {
        //showError(ret);
        goto error ;
    }

    while(!mysql_stmt_fetch(stmt))
    {
        StockProp* item=new StockProp();
        item->id=id;
        item->name=szName;
        result.append(item);
    }
error:
    mysql_stmt_close(stmt);
}

void MyDB::getNameByCode(const char* table,const QString& code,QStringList& result)
{
    char szSQL[256];
    std::string strCode=code.toStdString();
    sprintf(szSQL,"select name from %s where code='%s'",table,strCode.c_str());
    int ret=mysql_query(db,szSQL);
    if(ret)
    {
        return;
    }
    MYSQL_RES* res=mysql_store_result(db);
    if(res)
    {
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res)))
        {
            result.append(row[0]);
        }
        mysql_free_result(res);
    }
}
void MyDB::showInfo()
{
    //show variables like '%max_allowed%'
    int ret=mysql_query(db,"show variables like '%character%'");
    if(ret)
    {
        return;
    }
    MYSQL_RES* res=mysql_store_result(db);
    if(res)
    {
        unsigned int cols=mysql_num_fields(res);
        // unsigned int rows=mysql_num_rows(res);
        MYSQL_ROW row;
        while((row=mysql_fetch_row(res)))
        {
            for(int i=0;i<cols;i++)
                qDebug()<<row[i];
        }
        mysql_free_result(res);
    }
}

void MyDB::execute_sql(const char* sql)
{
    mysql_real_query(db,sql,strlen(sql));
}

bool MyDB::open(const char* user,const char* pw,const char* host,const char* db,int port)
{
    close();
    this->db=mysql_init(NULL);
    this->db=mysql_real_connect(this->db,host,user,pw,db,port,NULL,0);
    if(db==NULL)
        return false;
    showInfo();
    return true;
}
void MyDB::close()
{
    if(db)
        mysql_close(db);
}
bool MyDB::getProp(int stockid,QList<StockProp*>& indexes,QList<StockProp*>& notions,QList<StockProp*>& bussiness)
{
    MyStockProp* prop=new MyStockProp(this);
    prop->getProp(stockid,indexes,notions,bussiness);
    delete prop;
    return true;
}

MyBlackListStock* MyDB::getBlackList()
{
    return new MyBlackListStock(this);
}
MyStockListItem* MyDB::getStockListItem()
{
    return new MyStockListItem(this);
}
MyStockIndex* MyDB::getStockIndexItem()
{
    return new MyStockIndex(this);
}

bool MyDB::updateRecordList(const data::StockRecordList* record)
{
    MyStockRecordList* dbList=getRecordList();
    bool ret=dbList->update(record);
    delete dbList;
    return ret;
}
bool MyDB::addBlackList(const StockListItem* stock,const QDate& outDate)
{
    MyBlackListStock* addBl=getBlackList();
    const char* market="";
    if(stock->isShanghai())
        market="sh";
    else if(stock->isShenzhen())
        market="sz";
    bool ret=addBl->add(stock->getId(),stock->getCode(),market,outDate);
    delete addBl;
    return ret;
}
bool MyDB::getStockRecords(int stockid,const QDateTime* date,const QString* key,
                     QList<data::StockRecordItem*>& items)
{
    QString sql,where;
    sql="select id,crdate,parent,stockid,level,content,keyword,tradecondition from t_stocklistitem";
    if(stockid>=0)
        where.append(QString(" stockid = %1").arg(stockid));
    if(date!=NULL)
    {
        if(where.length()!=0)
            where.append(" and ");
        where.append(QString("crdate > '%1'").arg(date->toString("yyyy-MM-dd")));
    }
    if(key!=NULL&&key->length()!=0)
    {
        if(where.length()!=0)
            where.append(" and ");
        where.append(QString(" keyword like '%1'").arg(*key));
    }
    if(where.length()!=0)
    {
        sql=sql.append(" where ");
        sql=sql.append(where);
    }
    MyStockRecord* record=getRecord();
    bool ret= record->gets(sql.toStdString().c_str(),items);
    delete record;
    return ret;
}

bool MyDB::updateStockNote(const StockListItem* stock)
{
    MyStockListItem* updater=getStockListItem();
    bool ret=updater->updateNote(stock->getId(),stock->getComment(),stock->getCommentLevel());
    delete updater;
    return ret;
}
void MyDB::delRecordList(const data::StockRecordList* record)
{
    MyStockRecordList* dbList=getRecordList();
    dbList->del(record);
    delete dbList;
}

MyStockRecordList* MyDB::getRecordList()
{
    return new MyStockRecordList(this);
}
MyStockRecord* MyDB::getRecord()
{
    MyStockRecord* p=new MyStockRecord(this);
    p->setDataLoader(dataLoader);
    return p;
}
bool MyDB::updateRecord(const data::StockRecordItem* item)
{
    MyStockRecord* record=getRecord();
    bool ret= record->update(item);
    delete record;
    return ret;
}
bool MyDB::getRecordLists(const QString& where,QList<data::StockRecordList*>& list)
{
    MyStockRecordList* dbList=getRecordList();
    bool ret=dbList->getListsByWhere(where,list);
    delete dbList;
    return ret;
}

bool MyDB::saveRecordListAndItem(data::StockRecordList* list)
{
    MyStockRecordList* dbList=getRecordList();
    if(!dbList->add(list))
    {
        delete dbList;
        return false;
    }
    list->updateSubItem();
    delete dbList;
    MyStockRecord* record=getRecord();
    bool ret=record->adds(list->getDeepItems());
    delete record;
    return ret;
}
