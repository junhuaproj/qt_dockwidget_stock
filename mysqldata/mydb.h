#ifndef MYDB_H
#define MYDB_H

#include <mysql.h>
#include <QString>
#include <QStringList>
#include "recordlistsrc.h"
#include "dataloader.h"

namespace data{
    class DataLoader;
class StockRecordList;
    class StockRecordItem;
}

class MyStockRecordList;
class MyStockRecord;
class MyStockIndex;
class MyStockListItem;
class MyBlackListStock;

class MyDB
{
public:
    MyDB(data::DataLoader* pDataLoader);
    virtual ~MyDB();
    bool open(const char* user,const char* pw,const char* host,const char* db,int port=3306);
    void close();

    MyStockListItem* getStockListItem();
    MyStockIndex* getStockIndexItem();
    MyStockRecordList* getRecordList();
    MyStockRecord* getRecord();
    MyBlackListStock* getBlackList();

    bool addBlackList(const StockListItem* stock,const QDate& outDate);
    bool getStockRecords(int stockid,const QDateTime* date,const QString* key,
                         QList<data::StockRecordItem*>& items);
    bool updateStockNote(const StockListItem* stock);
    bool updateRecordList(const data::StockRecordList* record);
    void delRecordList(const data::StockRecordList* record);
    bool getProp(int stockid,QList<StockProp*>& indexes,QList<StockProp*>& notions,QList<StockProp*>& bussiness);

    void getStockIndex(int id,QList<StockProp*>& result);
    //void getBussiness(const QString& code,QList<StockProp*>& result);
    //void getNotions(const QString& code,QList<StockProp*>& result);
    void getNameByCode(const char* table,const QString& code,QStringList& result);
    void getNameByCode(const char* sql,QList<StockProp*>& result);
    bool updateRecord(const data::StockRecordItem* item);
    bool saveRecordListAndItem(data::StockRecordList* list);
    bool getRecordLists(const QString& where,QList<data::StockRecordList*>& list);
    void showInfo();

    void execute_sql(const char* sql);
    char* getSQLBuf(int* len);
    inline MYSQL* getDb(){
        return db;
    }
protected:
    char* szSQL;
    MYSQL* db;
    data::DataLoader* dataLoader;
};

#endif // MYDB_H
