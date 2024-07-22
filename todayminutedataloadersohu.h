#ifndef TODAYMINUTEDATALOADERSOHU_H
#define TODAYMINUTEDATALOADERSOHU_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "stocklistitem.h"
#include "stockchartview.h"
#include "idataloader.h"
namespace data{
class TodayMinuteDataLoaderSohu : public QObject
{
    Q_OBJECT
public:
    explicit TodayMinuteDataLoaderSohu(QObject *parent = nullptr);

    bool requestStock(StockChartView* chartView);
    void parseMinuteData(const QJsonArray& data);
    bool requestStockPrices(QList<StockListItem*>* stocks,TodayStocksPriceUpdate* priceUpdate);
protected:
    QNetworkReply *replyMinute;
    QNetworkReply *replyStocksPrice;
    QNetworkAccessManager networkManager;

    const StockListItem* pStockItem;

    //const QList<StockListItem*>* stocks;
    StockChartView* chartView;

    TodayStocksPriceUpdate* priceUpdate;
protected slots:
    void onStockPriceFinished();
    void onStockPriceReadyRead();

    void onDayFinished();
    void onDayReadyRead();
signals:
    void stockPriceUpdated(TodayStocksPriceUpdate* updated);
    void stockLoadFinished(StockChartView* chartView);
};
}
#endif // TODAYMINUTEDATALOADERSOHU_H
