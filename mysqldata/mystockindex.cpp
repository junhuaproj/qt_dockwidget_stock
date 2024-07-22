#include "mystockindex.h"
#include "mysql_data_def.h"
#include "stocklistitem.h"

#define STOCKINDEX_FIELD_COUNT  4

MyStockIndex::MyStockIndex(MyDB* db)
:MyData(db)
{
}

MyStockIndex::~MyStockIndex()
{

}


bool MyStockIndex::gets(QList<StockListItem*>& items)
{
    const char* szSQL="select id,code,name,note "
                        "from t_indexs ";


    if(!init_stmt(szSQL))
        return false;

    BindValue* idValue,*nameValue,*codeValue,*noteValue;

    if (!exec())
    {
        //showError(ret);
        return false;
    }
    resetBind(STOCKINDEX_FIELD_COUNT);
    idValue=bind_result_int(0);
    codeValue=bind_result_string(1,STOCK_CODE_LEN);
    nameValue=bind_result_string(2,STOCK_NAME_LEN);

    noteValue=bind_result_string(3,STOCK_NOTE_LEN);

    if(!bind_result())
    {
        qDebug()<<szSQL;
        return false;
    }

    if(!store_result())
    {
        return false;
    }

    while(nextRow())
    {
        StockListItem* item=new StockListItem();


        item->setFlag(StockItemFlag_CN|StockItemFlag_ZS);

        item->setId(idValue->i32Value);

        item->setCode(codeValue->szBuf);
        item->setName(nameValue->szBuf);
        if(!noteValue->isnull)
        {

        }


        items.append(item);
    }
error:
    deInit_stmt();
    return true;
}
