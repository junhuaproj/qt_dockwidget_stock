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

    /**
     * 加载图表绘图配置
    */
    bool loadDayChartDrawParam(QList<chart::StatisticIndexCfg*>& cfgs);
    /**
     * 保存图表绘图配置
    */
    void saveDayChartDrawParam(const QList<chart::StatisticIndexCfg*>& cfgs);
    /**
     * json数组写入文件
    */
    bool write(const QJsonArray& arr,const QString& path);
    /**
     * 文件中读取json到数组
    */
    bool readJson(QByteArray& data,const QString& path);
    bool loadTodayStocks(QStringList& stocks);
    bool saveTodayStocks(const QList<StockListItem*>& stocks);

    void clear();
    /*
     * 清空股票
    */
    void clearStockList();

    /*
     * 上市公司简介
    */
    bool loadStockInformation(const StockListItem* pItem,StockInformation &info);

    /*
     * 通过编号查找股票
    */
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
    /*
     * 股票列表
    */
    QList<StockListItem*> stockList;
    /*
     * 指数列表
    */
    QList<StockListItem*> indexList;
};
bool find_code_func(StockListItem* p,const QString& code);
bool find_stockBy_id_func(StockListItem* p,int id);
}

#endif // DATALOADER_H
