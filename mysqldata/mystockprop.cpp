#include "mystockprop.h"
#include "mydb.h"

MyStockProp::MyStockProp(MyDB* db)
    :MyData(db)
{
    resetBind(2);
    id=bind_result_int(0);
    name=bind_result_string(1,128);
}

MyStockProp::~MyStockProp()
{

}
bool MyStockProp::getProp(int stockid,QList<StockProp*>& indexes,QList<StockProp*>& notions,QList<StockProp*>& bussiness)
{
    getIndexes(stockid,indexes);
    getNotions(stockid,notions);
    getBussiness(stockid,bussiness);
    return true;
}
bool MyStockProp::getIndexes(int stockid,QList<StockProp*>& indexes)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select idx.id,idx.name "
                   "from t_indexs idx inner join t_stock_index si on idx.id=si.indexid"
                   " where stockid=%d",stockid);
    return getProp(szSQL,indexes);
}
bool MyStockProp::getNotions(int stockid,QList<StockProp*>& notions)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select n.id,n.name "
                   "from t_sinanotions n inner join t_sinanotion_detail d "
                   "on n.id=d.propid where stockid=%d",stockid);
    return getProp(szSQL,notions);
}
bool MyStockProp::getBussiness(int stockid,QList<StockProp*>& bussiness)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select b.id,b.name "
                   "from t_sinabussiness b inner join t_sinabussiness_detail d "
                   " on b.id=d.propid where stockid=%d",stockid);
    return getProp(szSQL,bussiness);
}
bool MyStockProp::getProp(const char* sql,QList<StockProp*>& props)
{
    StockProp* prop;
    bool ret=false;
    if(!init_stmt(sql))
        goto error;
    if (!exec())
    {
        goto error;
    }
    if(!bind_result())
    {
        qDebug()<<sql;
        goto error;
    }

    if(!store_result())
    {
        goto error;
    }

    while(nextRow())
    {
        prop=new StockProp(id->i32Value,name->szBuf);
        props.push_back(prop);
    }
    error:
    deInit_stmt();
    return ret;
}
