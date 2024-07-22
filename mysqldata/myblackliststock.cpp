#include "myblackliststock.h"
#include "mydb.h"
#define BLACKLIST_FIELD_COUNT   1
MyBlackListStock::MyBlackListStock(MyDB* db)
    :MyData(db)
{

}

MyBlackListStock::~MyBlackListStock()
{

}

// void MyBlackListStock::initBind()
// {
//     MyData::initBind();
//     bind[0].buffer_type=MYSQL_TYPE_DATE;
//     bind[0].buffer=(void*)&outDate;
// }
bool MyBlackListStock::add(int stockid,const QString& code,const QString& market,const QDate& outDate)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"insert into t_blacklist(srcid,code,market,outdate,flag)values(%d,?,?,?,1)",stockid);

    bool bRet=false;

    if(!init_stmt(szSQL))
    {
        return false;
    }

    resetBind(3);
    clearBindValue();
    bind_param_string(0,12,code);
    bind_param_string(1,12,market);
    bind_param_time(2,outDate);

    //ret=mysql_stmt_bind_named_param(stmt,bind+1,7,NULL);
    if (!bind_param())
    {
        return false;
    }
    if (!exec())
    {
        return false;
    }
    else
    {
        qDebug()<<"blacklist id:"<<mysql_stmt_insert_id(stmt);
        bRet=true;
    }
    deInit_stmt();
    return bRet;
}
