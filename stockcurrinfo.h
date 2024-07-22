/*
 * 实时价格相关数据
*/
#ifndef STOCKCURRINFO_H
#define STOCKCURRINFO_H

#include <QString>
//sohu
//编号，名称，价格，涨跌幅，涨跌额，成交量，现手，
//金额，换手，量比，最高，最低，市盈率，昨收，今开
//在股市进行时，现手显示当时最后一笔成交的成交量，
//红色代表正在挂买单的手数，绿色是挂卖单的收数。
//股市成交明细栏中“现手”红绿白，　　红的是外盘（B）：是按卖价成交，一般认为是主动买入，看涨；
//绿的是内盘（S）：是按买价成交，一般认为是主动卖出，看跌。
//白的是委托的价格合适，直接成交了，没有经过卖价、买价的挂单

//量比是衡量相对成交量的指标，
//它是指股市开市后平均每分钟的交易量与过去五个交易日平均每分钟的交易量之比
//市盈率（Price Earnings Ratio，简称P/E或PER），
//也称“本益比”、“股价收益比率”或“市价盈利比率（简称市盈率）”。
//市盈率是指股票价格除以每股收益(每股收益,EPS)的比率。
//或以公司市值除以年度股东应占溢利。
class StockCurrInfo
{
public:
    StockCurrInfo();
    QString getCode() const;
    void setCode(const QString &newCode);

    QString getName() const;
    void setName(const QString &newName);

    float getPrice() const;
    void setPrice(float newPrice);

    const QString& getChangedPercent() const;
    void setChangedPercent(const QString& newChangedPercent);

    const QString& getChangedValue() const;
    void setChangedValue(const QString& newChangedValue);

    int getVolume() const;
    void setVolume(int newVolume);

    int getCurVol() const;
    void setCurVol(int newCurVol);

    int getFund() const;
    void setFund(int newFund);

    const QString& getChange() const;
    void setChange(const QString& newChange);

    const QString& getVolRate() const;
    void setVolRate(const QString& newVolRate);

    float getHigh() const;
    void setHigh(float newHigh);

    float getLow() const;
    void setLow(float newLow);

    float getPriceRate() const;
    void setPriceRate(float newPriceRate);

    float getPreClose() const;
    void setPreClose(float newPreClose);

    float getOpen() const;
    void setOpen(float newOpen);

    int getPriceStatus()const;
protected:
    QString code;
    QString name;
    float price;
    QString changedPercent;
    QString changedValue;
    int volume;
    int curVol;
    int fund;
    QString change;//换手
    QString volRate;
    float high;
    float low;
    float priceRate;//市盈率
    float preClose;
    float open;
};

#endif // STOCKCURRINFO_H
