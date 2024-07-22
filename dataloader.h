/*
 * 数据加载器，这里保存用于全局的所有股票清单，指数清单，
 * 使用中的股票StockListItem指针最终指向这里，退出时才清空
*/
#ifndef DATALOADER_H
#define DATALOADER_H
#include <QObject>
#include <QString>
#include "stocklistitem.h"
//#include "todaystock.h"
#include "idataloader.h"

#include <QList>
#include <QMap>

#include "todayminutedataloadersohu.h"
#include "idataloader.h"

#include "cfgchart.h"
#include "StatisticIndex.h"

#include "stockrecorditem.h"


class MyDB;
#define STOCKLIST_LOAD_FROM_SQLITE

class StockInformation;
namespace data{

//class StockInformationLoader;
class StockRecordDb;
class DataLoader:public QObject
{
    Q_OBJECT
public:
    DataLoader();
    virtual ~DataLoader();
    void loadStockList();

#ifdef DB_MYSQL
    MyDB* getDB();
#endif
    void loadStockIndex();

    bool loadDayChartDrawParam(QList<chart::StatisticIndexCfg*>& cfgs);
    void saveDayChartDrawParam(const QList<chart::StatisticIndexCfg*>& cfgs);

    bool write(const QJsonArray& arr,const QString& path);
    bool readJson(QByteArray& data,const QString& path);
    bool loadTodayStocks(QStringList& stocks);
    bool saveTodayStocks(const QList<StockListItem*>& stocks);

    void clear();
    void clearStockList();

    bool loadStockInformation(const StockListItem* pItem,StockInformation &info);

    StockListItem* findStock(const QString& code);
    StockListItem* findStockById(int id);
    void findStockList(const QStringList& codes,QList<StockListItem*>* stocks);
    QList<StockListItem*>* getStockList();
    StockListItem* getStockIndex(uint32_t index);

    void filterStock(QList<StockListItem*>* stocks,uint32_t flag,const QString& filter="");
    void filterStock(QList<StockListItem*>* src,QList<StockListItem*>* dst,uint32_t flag,const QString& filter="");


    static QList<StockListItem*>::iterator findStockByCode(QList<StockListItem*>* stocks,const QString& code);
    static StockListItem* findStockItemByCode(QList<StockListItem*>* stocks,const QString& code);


    float* loadDatePrice(const QDate& date,const QString& prefix,int* outCount);
protected:
#ifdef DB_MYSQL
    MyDB* mydb;
    // QList<StockNote*> notes;
#endif

protected:
    void beginXls();
    void endXls();
protected:
    QStringList blackList;
    QList<StockListItem*> stockList;
    QList<StockListItem*> indexList;
};
bool find_code_func(StockListItem* p,const QString& code);
bool find_stockBy_id_func(StockListItem* p,int id);
}

#endif // DATALOADER_H
