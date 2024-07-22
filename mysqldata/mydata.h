/*
 * MYSQL数据操作的基类
*/
#ifndef MYDATA_H
#define MYDATA_H

#ifdef DB_MYSQL
//#include <mysql.h>

//#include <QList>
#include <QString>
#include <QDateTime>
#include "mybind_value.h"
#include <QList>
#include <QMap>
class MyDB;
class MyData
{
public:
    MyData(MyDB* db);
    virtual ~MyData();

    //virtual void initBind();
    //数据转换的一些方法
    void mysql_timeToQString(const MYSQL_TIME*,QString& );
    void mysql_timeToQDateTime(const MYSQL_TIME*,QDateTime& );
    void mysql_timeToQDate(const MYSQL_TIME*,QDate& );
    void mysql_timeToQTime(const MYSQL_TIME*,QTime& );

    void stringTomysql_time(MYSQL_TIME*,const char* s);
    void stringTomysql_time(MYSQL_TIME*,const QString& s);
    void daeTimeTomysql_time(MYSQL_TIME*,const QDateTime& );
    void dateTomysql_time(MYSQL_TIME*,const QDate& );
    void timeTomysql_time(MYSQL_TIME*,const QTime& );

    void showError(int result);

    void resetBind(int count=0);
    void clearBindValue();
    BindValue* bind_result_int(int c);
    BindValue* bind_result_float(int c);
    BindValue* bind_result_string(int c,int len);
    BindValue* bind_result_time(int c);
    BindValue* createBindValue(int c);

    BindValue* bind_param_int(int c,int value);
    BindValue* bind_param_float(int c,float value);
    BindValue* bind_param_string(int c,int len,char* szValue);
    BindValue* bind_param_string(int c,int len,const QString& str);
    BindValue* bind_param_time(int c,const QDateTime& value );
    BindValue* bind_param_time(int c,const QDate& value );

    bool init_stmt(const char* sql);
    void deInit_stmt();
    bool bind_result();
    bool store_result();
    bool nextRow();
    bool exec();
    bool bind_param();

    bool execute_sql(const char* szSQL);

protected:
    MyDB* _db;

    QMap<int,BindValue*> columnValues;
    int lastRet;
    MYSQL_STMT* stmt;
    int bindCount;
    MYSQL_BIND* bind;
    QList<BindValue*> bindValues;
    //bool* is_null;
    //unsigned long* length;
};
void dbgStmtError(MYSQL_STMT* stmt);
void stringToBuf(const QString&,char* buf,int buflen);
#endif
#endif // MYDATA_H
