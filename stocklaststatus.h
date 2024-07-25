/*
 * 实时股票信息，用于股票列表显示
 * sohu
 * 编号，名称，价格，涨跌幅，涨跌额，成交量，现手，
 * 金额，换手，量比，最高，最低，市盈率，昨收，今开
 * 在股市进行时，现手显示当时最后一笔成交的成交量，
 * 红色代表正在挂买单的手数，绿色是挂卖单的收数。
 * 股市成交明细栏中“现手”红绿白，　　红的是外盘（B）：是按卖价成交，一般认为是主动买入，看涨；
 * 绿的是内盘（S）：是按买价成交，一般认为是主动卖出，看跌。
 * 白的是委托的价格合适，直接成交了，没有经过卖价、买价的挂单

 * 量比是衡量相对成交量的指标，
 * 它是指股市开市后平均每分钟的交易量与过去五个交易日平均每分钟的交易量之比
 * 市盈率（Price Earnings Ratio，简称P/E或PER），
 * 也称“本益比”、“股价收益比率”或“市价盈利比率（简称市盈率）”。
 * 市盈率是指股票价格除以每股收益(每股收益,EPS)的比率。
 * 或以公司市值除以年度股东应占溢利。
*/
#ifndef STOCKLASTSTATUS_H
#define STOCKLASTSTATUS_H
#include <QString>
#include <QList>
#include <QObject>
class StockListItem;
class StockLastStatus:public QObject
{
    Q_OBJECT
public:
    StockLastStatus();

    const StockListItem *getPStock() const;
    void setPStock(const StockListItem *newPStock);

    double getPrice() const;
    void setPrice(double newPrice);

    const QString& getChangedPer() const;
    void setChangedPer(const QString& newChangedPer);

    const QString& getChangedValue() const;
    void setChangedValue(const QString& newChangedValue);

    double getVolume() const;
    void setVolume(double newVolume);

    double getFund() const;
    void setFund(double newFund);

    const QString& getChange() const;
    void setChange(const QString&);

    const QString& getVolRate() const;
    void setVolRate(const QString& newVolRate);

    double getHigh() const;
    void setHigh(double newHigh);

    double getLow() const;
    void setLow(double newLow);

    double getPriceRate() const;
    void setPriceRate(double newPriceRate);

    double getPreClose() const;
    void setPreClose(double newPreClose);

    double getOpen() const;
    void setOpen(double newOpen);

    QString getCurVol() const;
    void setCurVol(const QString &newCurVol);

    void toString(QString& out)const;
protected:
    const StockListItem* pStock;
    double price;
    QString changedPer;
    QString changedValue;
    double volume;
    double fund;
    QString curVol;
    QString change;
    QString  volRate;
    double high;
    double low;
    double priceRate;
    double  preClose;
    double open;
};

void clearLastStatus(QList<StockLastStatus*>& lastStatus);
bool find_status_by_stock_func(StockLastStatus* p,const StockListItem* stock);
QList<StockLastStatus*>::const_iterator findLastStatus(const QList<StockLastStatus*>& sastStatus,const StockListItem*);
QList<StockLastStatus*>::const_pointer findLastStatus(const QList<StockLastStatus*>& status,const StockLastStatus* stock);

#endif // STOCKLASTSTATUS_H
