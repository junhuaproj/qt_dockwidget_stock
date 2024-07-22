#include "todayminutedataloadersohu.h"
#include <QDateTime>
#include <QTextCodec>
namespace data{
TodayMinuteDataLoaderSohu::TodayMinuteDataLoaderSohu(QObject *parent)
    : QObject(parent),replyMinute(NULL),replyStocksPrice(NULL),priceUpdate(NULL)
{

}
bool TodayMinuteDataLoaderSohu::requestStock(StockChartView* chartView)
{
    if(replyMinute)return false;
    this->pStockItem=chartView->getStockItem();
    this->chartView=chartView;
    chartView->clearPrices();
    chartView->setTitle(pStockItem->getCode());
    QDateTime curDate=QDateTime::currentDateTime();
    const QString codeHead=pStockItem->getCode().mid(3);
    QString url=QString("https://hq.stock.sohu.com/cn/%1/cn_%2-4.html?openinwebview_finance=false&t=%3&_=%3")
                      .arg(codeHead,pStockItem->getCode())
                      .arg(curDate.toMSecsSinceEpoch());


    replyMinute=networkManager.get(QNetworkRequest(url));
    connect(replyMinute,SIGNAL(finished()),this,SLOT(onDayFinished()));
    connect(replyMinute,SIGNAL(readyRead()),this,SLOT(onDayReadyRead()));
    return true;
}
void TodayMinuteDataLoaderSohu::onDayFinished()
{
    if(replyMinute)
    {
        replyMinute->deleteLater();
        replyMinute=Q_NULLPTR;
    }
}

void TodayMinuteDataLoaderSohu::onDayReadyRead()
{
    QByteArray bufs=replyMinute->readAll();
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    //QString jsonstr=QString::from(bufs);
    QString jsonstr=codec->toUnicode(bufs);
    jsonstr=jsonstr.mid(jsonstr.indexOf('(')+1);
    jsonstr=jsonstr.left(jsonstr.length()-1);
    jsonstr=jsonstr.replace("'","\"");

    QJsonDocument doc=QJsonDocument::fromJson(jsonstr.toUtf8());
    if(!doc.isArray())
    {
        qDebug()<<jsonstr;
        return;
    }
    QJsonArray root=doc.array();
    parseMinuteData(root);
}
void TodayMinuteDataLoaderSohu::parseMinuteData(const QJsonArray& data)
{
    QString hhmm;
    float price,avgPrice;
    int vol,fund;
    QJsonArray item=data.at(0).toArray();
    chartView->setPrices(item.at(0).toString().toFloat(),
                         item.at(1).toString().toFloat(),
                         item.at(2).toString().toFloat(),
                         item.at(3).toString().toFloat());

    for(qsizetype i=1;i<data.size();i++)
    {
        item=data.at(i).toArray();
        hhmm=item.at(0).toString();
        price=item.at(1).toString().toFloat();
        avgPrice=item.at(2).toString().toFloat();
        vol=item.at(3).toString().toFloat();
        fund=item.at(4).toString().toFloat();
        chartView->addStockPrice(hhmm,
                                   price,avgPrice,
                                   vol,fund);
    }

    emit stockLoadFinished(chartView);
    //StockChartView::addOneDayXLabel(chartView,0);
    //chartView->repaint();
}

bool TodayMinuteDataLoaderSohu::requestStockPrices(QList<StockListItem*>* stocks,TodayStocksPriceUpdate* priceUpdate)
{
    if(replyStocksPrice||stocks->size()==0)return false;
    this->priceUpdate=priceUpdate;
    //this->stocks=stocks;
    QString codes="";
    QList<StockListItem*>::iterator it=stocks->begin(),end=stocks->end();
    for(;it!=end;it++)
    {
        codes.append("cn_"+(*it)->getCode()+",");
    }
    QDateTime curDate=QDateTime::currentDateTime();
    codes=codes.left(codes.length()-1);
    QString url=QString("https://hqm.stock.sohu.com/getqjson?code=%1&cb=fortune_hq_diwei&_=%2").arg(codes).arg(curDate.toMSecsSinceEpoch());

    replyStocksPrice=networkManager.get(QNetworkRequest(url));
    connect(replyStocksPrice,SIGNAL(finished()),this,SLOT(onStockPriceFinished()));
    connect(replyStocksPrice,SIGNAL(readyRead()),this,SLOT(onStockPriceReadyRead()));
    return true;
}

void TodayMinuteDataLoaderSohu::onStockPriceFinished()
{
    if(replyStocksPrice)
    {
        replyStocksPrice->deleteLater();
        replyStocksPrice=Q_NULLPTR;
    }
}
void TodayMinuteDataLoaderSohu::onStockPriceReadyRead()
{
    QByteArray bufs=replyStocksPrice->readAll();
    QTextCodec *codec=QTextCodec::codecForName("GBK");
    //QString jsonstr=QString::from(bufs);
    QString jsonstr=codec->toUnicode(bufs);
    int index=jsonstr.indexOf('(');
    if(index<0)return;
    jsonstr=jsonstr.mid(index+1,jsonstr.length()-2-index-1);
    qDebug()<<jsonstr;
    QJsonDocument doc=QJsonDocument::fromJson(jsonstr.toUtf8());
    if(doc.isObject())
    {
        QJsonObject obj=doc.object();
        QStringList keys=obj.keys();

        //编号，名称，价格，涨跌幅，涨跌额，成交量，现手，
        //金额，换手，量比，最高，最低，市盈率，昨收，今开
        //QList<StockListItem*>::const_iterator it=stocks->begin(),end=stocks->end();
        /*for(;it!=end;it++)
        {
            QJsonArray stockInfos=obj["cn_"+(*it)->getCode()].toArray();
            for(int j=0;j<stockInfos.size();j++)
            {
                qDebug()<<stockInfos.at(j).toString();
            }
        }*/
        QStringList::iterator it=keys.begin(),end=keys.end();
        StockCurrInfo info;
        for(;it!=end;it++)
        {
            QJsonArray stockInfos=obj[*it].toArray();
            info.setCode(stockInfos.at(0).toString());
            info.setName(stockInfos.at(1).toString());
            info.setPrice(stockInfos.at(2).toString().toFloat());
            info.setChangedPercent(stockInfos.at(3).toString());
            info.setChangedValue(stockInfos.at(4).toString());
            info.setVolume(stockInfos.at(5).toString().toInt());
            info.setCurVol(stockInfos.at(6).toString().toInt());
            info.setFund(stockInfos.at(7).toString().toInt());
            info.setChange(stockInfos.at(8).toString());
            info.setVolRate(stockInfos.at(9).toString());
            info.setHigh(stockInfos.at(10).toString().toFloat());
            info.setLow(stockInfos.at(11).toString().toFloat());
            info.setPriceRate(stockInfos.at(12).toString().toFloat());
            info.setPreClose(stockInfos.at(13).toString().toFloat());
            info.setOpen(stockInfos.at(14).toString().toFloat());

            for(int j=0;j<stockInfos.size();j++)
            {
                qDebug()<<stockInfos.at(j).toString();
            }
            priceUpdate->PriceUpdated(&info);
        }
        emit stockPriceUpdated(priceUpdate);
    }

}
}
