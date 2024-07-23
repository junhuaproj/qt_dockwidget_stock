#include "dataloader.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <functional>
#include <memory>

#include "stockinformation.h"

#include <sqlite3.h>
#include "appconfig.h"
#include "mysqldata/mydb.h"
#include "mysqldata/mystocklistitem.h"
#include "mysqldata/mystockindex.h"
#include "mysqldata/mystockprop.h"
#include "data/parserjsondata.h"
#include "appconfig.h"

#include <QMessageBox>
using namespace std::placeholders;
namespace data{
//const char* data_root="F:\\stock";
DataLoader::DataLoader()
{
#ifdef DB_MYSQL
    mydb=new MyDB(this);
    QString user,pw,server,db;
    if(appConfig.readMysqlCfg(user,pw,server,db))
        mydb->open(user.toStdString().c_str(),
                   pw.toStdString().c_str(),
                   server.toStdString().c_str(),db.toStdString().c_str());
    else
    {
        QMessageBox::about(NULL,"db","read mysql cfg error /config/dbcfg.json");
    }

#endif
    beginXls();

}

DataLoader::~DataLoader()
{
#ifdef DB_MYSQL
    delete mydb;
#endif

    endXls();
    clear();
}

bool DataLoader::readJson(QByteArray& data,const QString& path)
{
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        data=f.readAll();
        f.close();
        return true;
    }
    return false;
}
bool DataLoader::write(const QJsonArray& arr,const QString& path)
{
    QJsonDocument doc(arr);
    QFile f(path);
    if(f.open(QFile::WriteOnly))
    {
        f.write(doc.toJson());
        f.close();
        return true;
    }
    return false;
}

bool DataLoader::loadTodayStocks(QStringList& stocks)
{
    QByteArray data;
    QString path;
    appConfig.getCfgTodayStock(path);
    if(readJson(data,path))
    {
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(!doc.isArray())
            return false;
        QJsonArray arr=doc.array();
        for(int i=0;i<arr.size();i++)
        {
            stocks.push_back(arr.at(i).toString());
        }
        return true;
    }
    return false;
}

bool DataLoader::saveTodayStocks(const QList<StockListItem*>& stocks)
{
    QJsonArray arr;
    QList<StockListItem*>::const_iterator it=stocks.begin(),end=stocks.end();
    for(;it!=end;it++)
    {
        arr.append(QJsonValue((*it)->getCode()));
    }
    QString path;
    appConfig.getCfgTodayStock(path);
    return write(arr,path);
}

bool DataLoader::loadDayChartDrawParam(QList<chart::StatisticIndexCfg*>& cfgs)
{
    QString path=QString("%1\\drawcfg.json").arg(appConfig.getStockRoot());
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        QJsonDocument doc=QJsonDocument::fromJson(f.readAll());
        f.close();
        QJsonObject obj= doc.object();
        QList<chart::StatisticIndexCfg*>::iterator it=cfgs.begin(),end=cfgs.end();
        QJsonValue val;
        for(;it!=end;it++)
        {
            val=obj.value((*it)->getIndexId());
            if(val.isUndefined())
            {
                (*it)->loadDefault();
            }
            else
            {
                QJsonObject obj=val.toObject();
                (*it)->loadCfg(&obj);
            }
        }
        return true;
    }
    return false;
}
void DataLoader::saveDayChartDrawParam(const QList<chart::StatisticIndexCfg*>& cfgs)
{
    QJsonObject root;
    QList<chart::StatisticIndexCfg*>::const_iterator it=cfgs.begin(),end=cfgs.end();
    for(;it!=end;it++)
    {
        QJsonObject index;
        (*it)->saveCfg(&index);
        root.insert((*it)->getIndexId(),index);
    }
    QJsonDocument doc(root);
    QString path=QString("%1\\drawcfg.json").arg(appConfig.getStockRoot());
    QFile f(path);
    if(f.open(QFile::WriteOnly))
    {
        f.write(doc.toJson());
        f.close();
    }
}

void DataLoader::clear()
{
    clearStockList();
}

void DataLoader::clearStockList()
{
    while(!stockList.isEmpty())
    {
        delete stockList.back();
        stockList.pop_back();
    }
    while(!indexList.isEmpty())
    {
        delete indexList.back();
        indexList.pop_back();
    }
    // while(!notes.isEmpty())
    // {
    //     delete notes.back();
    //     notes.pop_back();
    // }
}

QList<StockListItem*>* DataLoader::getStockList()
{
    return &stockList;
}
StockListItem* DataLoader::getStockIndex(uint32_t index)
{
    if(index<stockList.size())
        return stockList.at(index);
    return NULL;
}

void DataLoader::loadStockIndex()
{
    MyStockIndex* indexs=mydb->getStockIndexItem();
    indexs->gets(indexList);

}

bool DataLoader::loadStockInformation(const StockListItem* pItem,StockInformation &info)
{
    info.setName(pItem->getName());

    ParserJsonData parser;
    QString path=QString("%1\\stockinfo\\%2.json").arg(appConfig.getStockRoot(),pItem->getCode());
    parser.parserStockInfo(path,info);
    //informationLoader->loadInformation(pItem->getCode(),info);

    //MyStockProp prop(this->getDB());
    QList<StockProp*> indexes;
    QList<StockProp*> notions;
    QList<StockProp*> bussiness;
    getDB()->getProp(pItem->getId(),indexes,notions,bussiness);
    info.setIndex(indexes);
    info.setNotions(notions);
    info.setBussiness(bussiness);
    clearPropList(indexes);
    clearPropList(notions);
    clearPropList(bussiness);


    return true;
}
StockListItem* DataLoader::findStock(const QString& code)
{
    QList<StockListItem*>::iterator it=findStockByCode(&stockList,code);
    if(it!=stockList.end())
        return *it;
    return NULL;
}

void DataLoader::findStockList(const QStringList& codes,QList<StockListItem*>* stocks)
{
    QStringList::const_iterator it=codes.begin(),end=codes.end();
    for(;it!=end;it++)
    {
        stocks->append(findStock(*it));
    }
}

void DataLoader::beginXls()
{

}
void DataLoader::endXls()
{
#ifndef STOCKLIST_LOAD_FROM_SQLITE
    QSqlDatabase::removeDatabase("xlsx_connection");
#endif
}


void DataLoader::filterStock(QList<StockListItem*>* src,QList<StockListItem*>* dst,uint32_t flag,const QString& filter)
{
    QList<StockListItem*>::iterator it=src->begin(),end=src->end();
    for(;it!=end;it++)
    {
        //if(((*it)->getFlag()&flag)
        //    &&(filter.length()==0||(*it)->getCode().contains(filter)))
        if((*it)->filter(flag,filter))
            dst->append(*it);
    }
}

void DataLoader::filterStock(QList<StockListItem*>* stocks,uint32_t flag,const QString& filter)
{
    if((flag&(StockItemFlag_CN|StockItemFlag_SH))==(StockItemFlag_CN|StockItemFlag_SH))
    {
        filterStock(&stockList,stocks,StockItemFlag_SH,filter);
    }
    if((flag&(StockItemFlag_CN|StockItemFlag_SZ))==(StockItemFlag_CN|StockItemFlag_SZ))
    {
        filterStock(&stockList,stocks,StockItemFlag_SZ,filter);
    }
    if((flag&(StockItemFlag_CN|StockItemFlag_ZS))==(StockItemFlag_CN|StockItemFlag_ZS))
    {
        filterStock(&indexList,stocks,StockItemFlag_ZS,filter);
    }
}

#ifdef DB_MYSQL
MyDB* DataLoader::getDB()
{
    return mydb;
}
#endif
void DataLoader::loadStockList()
{
    MyStockListItem* stocks=mydb->getStockListItem();

    stocks->gets(stockList);
    /*while(!notes.isEmpty())
    {
        delete notes.back();
        notes.pop_back();
    }*/
    delete stocks;

}
QList<StockListItem*>::iterator DataLoader::findStockByCode(QList<StockListItem*>* stocks,const QString& code)
{
    return std::find_if(stocks->begin(),stocks->end(),std::bind(find_code_func,_1,code));
}
StockListItem* DataLoader::findStockById(int id)
{
    QList<StockListItem*>::iterator it= std::find_if(stockList.begin(),stockList.end(),std::bind(find_stockBy_id_func,_1,id));
    if(it!=stockList.end())
    {
        return *it;
    }
    return NULL;
}

StockListItem* DataLoader::findStockItemByCode(QList<StockListItem*>* stocks,const QString& code)
{
    QList<StockListItem*>::iterator it=findStockByCode(stocks,code);
    if(it==stocks->end())
        return NULL;
    return *it;
}

float* DataLoader::loadDatePrice(const QDate& date,const QString& prefix,int* outCount)
{
    QString path=QString("%1\\stockdaystatus\\%3%2.json").arg(appConfig.getStockRoot(),date.toString("yyyy-MM-dd"),prefix);
    QFile file(path);
    if(file.open(QFile::ReadOnly))
    {
        QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
        file.close();
        QJsonArray arr=doc.array();
        int count=arr.size();
        float* prices=(float*)malloc(sizeof(float)*count);
        for(int i=0;i<count;i++)
        {
            QJsonObject var=arr.at(i).toObject();
            prices[i]=var.value("pchg").toDouble();
        }
        *outCount=count;
        return prices;
    }
    return NULL;
}

bool find_code_func(StockListItem* p,const QString& code)
{
    return p->getCode().compare(code)==0;
}
bool find_stockBy_id_func(StockListItem* p,int id)
{
    return p->getId()==id;
}
}
