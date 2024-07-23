#include "mystocklistitem.h"

#include "stocklistitem.h"

#include <string>
#include <QDate>
#include <QTime>

#include "mysql_data_def.h"
#include "mydb.h"

#define STOCKLIST_FIELD_COUNT   7


MyStockListItem::MyStockListItem(MyDB* db)
    :MyData(db)
{

}

MyStockListItem::~MyStockListItem()
{

}

bool MyStockListItem::updateNote(int id,const QString& note,int level)
{
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"update t_stock set comment=?,lastdate=now(),commentlevel=%d where id=%d",
            level,id);
    if(!init_stmt(szSQL))
    {
        return 0;
    }
    resetBind(1);
    bind_param_string(0,STOCK_NOTE_LEN,note);

    bool ret=false;
    if(bind_param()&&exec())
    {
        ret=true;
    }
    deInit_stmt();
    return ret;
}


void MyStockListItem::get(int id,StockListItem& item)
{
    //char szSQL[128];
    int len;
    char* szSQL=_db->getSQLBuf(&len);
    sprintf(szSQL,"select code,market,name,level,comment,commentdate "
                   "from t_stock "
                   "where id=%d",id);

    if(!init_stmt(szSQL))
    {
        return ;
    }
    if (!exec())
    {
        return ;
    }
    resetBind(6);
    //BindValue* id=bind_result_int(0);
    BindValue* code=bind_result_string(0,STOCK_CODE_LEN);
    BindValue* market=bind_result_string(1,STOCK_MARKET_LEN);
    BindValue* name=bind_result_string(2,STOCK_NAME_LEN);
    BindValue* level=bind_result_int(3);
    BindValue* note=bind_result_string(4,STOCK_NOTE_LEN);
    BindValue* lastdate=bind_result_time(5);


    if(!bind_result())
    {
        return ;
    }
    if(!store_result())
    {
        return ;
    }

    if(nextRow())
    {
        //int flag=0;
        item.setId(id);

        item.setCode(code->szBuf);
        item.setName(name->szBuf);


        item.setFlag(getFlags(market->szBuf));
        if(!note->isnull)
        {
            QDate date;
            mysql_timeToQDate(lastdate->pTime,date);
            item.setCommentDate(date);
            item.setComment(note->szBuf);
            item.setCommentLevel(level->i32Value);
        }
    }
    deInit_stmt();
}
int MyStockListItem::getFlags(const char* market)
{
    if(strcmp(market,"sz")==0)
    {
        return StockItemFlag_SZ|StockItemFlag_CN;
    }
    else if(strcmp(market,"sh")==0)
    {
        return StockItemFlag_SH|StockItemFlag_CN;
    }
    return 0;
}
void MyStockListItem::gets(QList<StockListItem*>& items)
{

    const char* szSQL="select id,code,market,name,level,comment,commentdate "
                   "from t_stock";
                   //"where stockid=%d");
    //int ret;
    if(!init_stmt(szSQL))
    {
        return ;
    }


    if (!exec())
    {
        return ;
    }
    resetBind(STOCKLIST_FIELD_COUNT);
    BindValue* id=bind_result_int(0);
    BindValue* code=bind_result_string(1,STOCK_CODE_LEN);
    BindValue* market=bind_result_string(2,STOCK_MARKET_LEN);
    BindValue* name=bind_result_string(3,STOCK_NAME_LEN);
    BindValue* level=bind_result_int(4);
    BindValue* note=bind_result_string(5,STOCK_NOTE_LEN);
    BindValue* date=bind_result_time(6);

    qDebug()<<szSQL;
    if(!bind_result())
    {
        return ;
    }
    //ret=mysql_stmt_store_result(stmt);
    if(!store_result())
    {
        return ;
    }

    while(nextRow())
    {
        StockListItem* item=new StockListItem();

        item->setFlag(getFlags(market->szBuf));

        item->setId(id->i32Value);

        item->setCode(code->szBuf);
        item->setName(name->szBuf);

        if(!note->isnull)
        {
            QDate cdate;
            mysql_timeToQDate(date->pTime,cdate);
            item->setCommentDate(cdate);
            item->setComment(note->szBuf);
            item->setCommentLevel(level->i32Value);
        }
        else
            item->setCommentLevel(0);

        items.append(item);
    }
    error:
    deInit_stmt();
}
