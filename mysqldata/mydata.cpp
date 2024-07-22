#include "mydata.h"
#include <QDebug>
#include "appconfig.h"
#include "mydb.h"
#include <string>
MyData::MyData(MyDB* db)
    :_db(db),stmt(NULL),bindCount(0),bind(NULL)
{
}

MyData::~MyData()
{
    resetBind(0);
    clearBindValue();
    deInit_stmt();
}


BindValue* MyData::createBindValue(int c)
{
    BindValue* value=columnValues.value(c);
    if(value==NULL)
    {
        value=(BindValue*)malloc(sizeof(BindValue));
        columnValues.insert(c,value);

        memset(value,0,sizeof(BindValue));

        bind[c].is_null=&value->isnull;
        bind[c].length=&value->length;

        bindValues.push_back(value);
    }
    return value;
}

BindValue* MyData::bind_result_float(int c)
{
    BindValue* value=createBindValue(c);
    bind[c].buffer_type=MYSQL_TYPE_FLOAT;
    bind[c].buffer=(char*)&value->fValue;
    return value;
}
BindValue* MyData::bind_result_int(int c)
{
    BindValue* value=createBindValue(c);
    bind[c].buffer_type=MYSQL_TYPE_LONG;
    bind[c].buffer=(char*)&value->i32Value;
    return value;
}

BindValue* MyData::bind_result_string(int c,int len)
{
    BindValue* value=createBindValue(c);
    bind[c].buffer_type=MYSQL_TYPE_VAR_STRING;
    value->buf_len=len;
    value->szBuf=(char*)malloc(len);
    bind[c].buffer=value->szBuf;
    bind[c].buffer_length=value->buf_len;
    return value;
}
BindValue* MyData::bind_result_time(int c)
{
    BindValue* value=createBindValue(c);
    bind[c].buffer_type=MYSQL_TYPE_DATE;

    value->buf_len=sizeof(MYSQL_TIME);
    value->szBuf=(char*)malloc(value->buf_len);
    bind[c].buffer=(char*)value->pTime;
    return value;
}

bool MyData::init_stmt(const char* sql)
{
    int ret;
    deInit_stmt();
    stmt = mysql_stmt_init(_db->getDb());
    if(stmt==NULL)return false;

    ret=mysql_stmt_prepare(stmt,sql,strlen(sql));
    if(ret)
    {
        showError(ret);
        return false;
    }
    return true;
}
void MyData::deInit_stmt()
{
    if(stmt)
    {
        mysql_stmt_close(stmt);
        stmt=NULL;
    }
}
void MyData::resetBind(int count)
{
    if(bind)
        free(bind);
    if(count==0)
        bind=NULL;
    else
    {
        this->bindCount=count;
        bind=(MYSQL_BIND*)malloc(bindCount*sizeof(MYSQL_BIND));
        memset(bind,0,sizeof(MYSQL_BIND)*count);
    }

}
void MyData::clearBindValue()
{
    BindValue* value;
    while(!bindValues.isEmpty())
    {
        value=bindValues.back();
        if(value->buf_len)
            free(value->szBuf);
        free(value);
        bindValues.pop_back();
    }
    columnValues.clear();
}
void MyData::mysql_timeToQString(const MYSQL_TIME* dt,QString& str)
{
    str=QString("%1-%2-%3 %4:%5:%6")
              .arg(dt->year).arg((int)dt->month).arg((int)dt->day)
              .arg((int)dt->hour).arg((int)dt->minute).arg((int)dt->second);
}
void MyData::mysql_timeToQDateTime(const MYSQL_TIME* dt,QDateTime& qdt)
{
    QDate date;
    QTime time;
    mysql_timeToQDate(dt,date);
    mysql_timeToQTime(dt,time);
    qdt=QDateTime(date,time);
}
void MyData::mysql_timeToQDate(const MYSQL_TIME* dt,QDate& date)
{
    date=QDate(dt->year,dt->month,dt->day);
}

void MyData::mysql_timeToQTime(const MYSQL_TIME* dt,QTime& time)
{
    time=QTime(dt->hour,dt->minute,dt->second);
}

void MyData::stringTomysql_time(MYSQL_TIME* dt,const char* s)
{
    stringTomysql_time(dt,QString(s));
}
void MyData::stringTomysql_time(MYSQL_TIME* dt,const QString& s)
{
    QDateTime date=QDateTime::fromString(s,datetime_format);
    daeTimeTomysql_time(dt,date);
}
void MyData::daeTimeTomysql_time(MYSQL_TIME* dt,const QDateTime& datetime)
{
    dateTomysql_time(dt,datetime.date());
    timeTomysql_time(dt,datetime.time());
}
void MyData::dateTomysql_time(MYSQL_TIME* dt,const QDate& date)
{
    dt->year=date.year();
    dt->month=date.month();
    dt->day=date.day();
}
void MyData::timeTomysql_time(MYSQL_TIME* dt,const QTime& time)
{
    dt->hour=time.hour();
    dt->minute=time.minute();
    dt->second=time.second();
}

void MyData::showError(int result)
{
    qDebug()<<"error:"<<result;
    dbgStmtError(stmt);
}
bool MyData::bind_result()
{
    lastRet=mysql_stmt_bind_result(stmt,bind);
    if(lastRet)
    {
        showError(lastRet);
        return false;
    }
    return true;
}
bool MyData::store_result()
{
    lastRet=mysql_stmt_store_result(stmt);
    if(lastRet)
    {
        showError(lastRet);
        return false;
    }
    return true;
}
bool MyData::nextRow()
{
    lastRet=mysql_stmt_fetch(stmt);
    if(lastRet&&lastRet!=MYSQL_DATA_TRUNCATED)
    {
        showError(lastRet);
        return false;
    }
    return true;
}

bool MyData::exec()
{
    lastRet=mysql_stmt_execute(stmt);
    if(lastRet)
    {
        showError(lastRet);
        return false;
    }
    return true;
}
bool MyData::execute_sql(const char* szSQL)
{
    mysql_real_query(_db->getDb(),szSQL,strlen(szSQL));
    return true;
}
bool MyData::bind_param()
{
    lastRet=mysql_stmt_bind_param(stmt, bind);
    if(lastRet)
    {
        showError(lastRet);
        return false;
    }
    return true;
}

BindValue* MyData::bind_param_int(int c,int ivalue)
{
    BindValue* value=bind_result_int(c);
    value->i32Value=ivalue;
    return value;
}

BindValue* MyData::bind_param_float(int c,float fvalue)
{
    BindValue* value=bind_result_float(c);
    value->fValue=fvalue;
    return value;
}

BindValue* MyData::bind_param_string(int c,int len,char* szValue)
{
    BindValue* value=bind_result_string(c,len);
    strcpy(value->szBuf,szValue);
    value->length=strlen(szValue);
    bind[c].length=&value->length;
    return value;
}

BindValue* MyData::bind_param_string(int c,int len,const QString& str)
{
    BindValue* value=bind_result_string(c,len);
    stringToBuf(str,value->szBuf,len);
    value->length=strlen(value->szBuf);//str.length();
    bind[c].length=&value->length;
    return value;

}
BindValue* MyData::bind_param_time(int c,const QDateTime& dtvalue )
{
    BindValue* value=bind_result_time(c);
    daeTimeTomysql_time(value->pTime,dtvalue);
    return value;
}

BindValue* MyData::bind_param_time(int c,const QDate& dtvalue )
{
    BindValue* value=bind_result_time(c);
    dateTomysql_time(value->pTime,dtvalue);
    return value;
}
void stringToBuf(const QString& in,char* buf,int buflen)
{
    std::string sIn=in.toStdString();
    if(sIn.length()>buflen-1)
    {
        strncpy(buf,sIn.c_str(),buflen-1);
        buf[buflen-1]=0;
    }
    else
    {
        strcpy(buf,sIn.c_str());
    }
}

void dbgStmtError(MYSQL_STMT* stmt)
{
    qDebug()<<"stmt error code:"<<mysql_stmt_errno(stmt)<<" msg:"<<mysql_stmt_error(stmt);
}
