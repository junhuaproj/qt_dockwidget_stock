#include "sohudataparser.h"

#include "stockchartview.h"
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include "stockminutechart.h"
#include "dataloader.h"
#include "stocklaststatus.h"

namespace datasohu{
DataParser::DataParser():dataLoader(NULL) {

}


void DataParser::setDataLoader(data::DataLoader* dataLoader)
{
    this->dataLoader=dataLoader;
}

bool DataParser::parseMinuteData(const QByteArray& data,minutechart::StockDayMinute* day,bool bInited)
{
    QString jsonstr;
    if(!bInited)
    {
        QTextCodec *codec=QTextCodec::codecForName("GBK");
        //QString jsonstr=QString::from(bufs);
        jsonstr=codec->toUnicode(data);
        jsonstr=jsonstr.mid(jsonstr.indexOf('(')+1);
        jsonstr=jsonstr.left(jsonstr.length()-1);
        jsonstr=jsonstr.replace("'","\"");
    }
    else
    {
        jsonstr=QString(data);
    }
    QJsonDocument doc=QJsonDocument::fromJson(jsonstr.toUtf8());
    if(!doc.isArray())
    {
        qDebug()<<jsonstr;
        return false;
    }
    QJsonArray root=doc.array();
    return parseMinuteJsonData(root,day);
    //return parseJsonData(root,day);
}

bool DataParser::parseMinuteJsonData(const QJsonArray& data,minutechart::StockDayMinute* day)
{
    QString hhmm;
    float price,avgPrice;
    int vol,fund;
    QJsonArray item=data.at(0).toArray();

    day->setPrices(item.at(0).toString().toDouble(),
                         item.at(1).toString().toDouble(),
                         item.at(2).toString().toDouble(),
                         item.at(3).toString().toDouble());

    for(qsizetype i=1;i<data.size();i++)
    {
        item=data.at(i).toArray();
        hhmm=item.at(0).toString();
        price=item.at(1).toString().toDouble();
        avgPrice=item.at(2).toString().toDouble();
        vol=item.at(3).toString().toDouble();
        fund=item.at(4).toString().toDouble();
        if(day->isFillMode())
            day->setMin(i-1,hhmm,price,vol,fund,avgPrice);
        else
            day->addMin(hhmm,price,vol,fund,avgPrice);
    }
    return true;
}
bool DataParser::parsePriceData(const QByteArray& data,QList<StockLastStatus*>& stocks,Get_Last_Status_by_Stock_Callbback callback,void* p)
{
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    //QString jsonstr=QString::from(bufs);
    QString jsonstr=codec->toUnicode(data);
    int index=jsonstr.indexOf('(');
    if(index<0)return false;
    jsonstr=jsonstr.mid(index+1,jsonstr.length()-2-index-1);
    //jsonstr=jsonstr.replace('\'',"\"");
    QJsonDocument doc=QJsonDocument::fromJson(jsonstr.toUtf8());
    if(!doc.isObject())
        return false;
    return parsePriceJsonData(doc.object(),stocks,callback,p);
}


bool DataParser::parsePriceJsonData(const QJsonObject& obj,QList<StockLastStatus*>& stocks,Get_Last_Status_by_Stock_Callbback callback,void* p)
{
    QStringList keys=obj.keys();
    //编号，名称，价格，涨跌幅，涨跌额，成交量，现手，
    //金额，换手，量比，最高，最低，市盈率，昨收，今开

    QStringList::iterator it=keys.begin(),end=keys.end();
    //StockCurrInfo info;
    QString code;
    StockLastStatus* status;
    StockListItem* stock;
    //QList<StockLastStatus*>::iterator itFind;

    for(;it!=end;it++)
    {
        QJsonArray stockInfos=obj[*it].toArray();
        code=stockInfos.at(0).toString();
        code=code.mid(3);
        stock=dataLoader->findStock(code);
        if(callback)
        {//允许替换
            status=callback(stock,p);
        }
        else
            status=NULL;
        if(status==NULL)//不存在，重建
            status=new StockLastStatus();

        status->setPStock(stock);//dataLoader->findStock(code.mid(3)));

        status->setPrice(stockInfos.at(2).toString().toFloat());
        status->setChangedPer(stockInfos.at(3).toString());
        status->setChangedValue(stockInfos.at(4).toString());
        status->setVolume(stockInfos.at(5).toString().toInt());
        status->setCurVol(stockInfos.at(6).toString());
        status->setFund(stockInfos.at(7).toString().toInt());
        status->setChange(stockInfos.at(8).toString());
        status->setVolRate(stockInfos.at(9).toString());
        status->setHigh(stockInfos.at(10).toString().toFloat());
        status->setLow(stockInfos.at(11).toString().toFloat());
        status->setPriceRate(stockInfos.at(12).toString().toFloat());
        status->setPreClose(stockInfos.at(13).toString().toFloat());
        status->setOpen(stockInfos.at(14).toString().toFloat());
        if(!callback)
            stocks.push_back(status);
    }
    return false;
}

}
