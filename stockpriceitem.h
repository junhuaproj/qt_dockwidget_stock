//分钟数据
#ifndef STOCKPRICEITEM_H
#define STOCKPRICEITEM_H

#include <QObject>

class StockPriceItem
{
public:
    StockPriceItem();

    const QString& getHhmm() const;
    void setHhmm(const QString &newHhmm);

    float getPrice() const;
    void setPrice(float newPrice);

    float getAvgPrice() const;
    void setAvgPrice(float newAvgPrice);

    int32_t getVolume() const;
    void setVolume(int32_t newVolume);

    int32_t getFund() const;
    void setFund(int32_t newFund);

    bool isHour()const;
    bool is0900Or1300()const;
protected:
    QString hhmm;
    float price;
    float avgPrice;
    int32_t volume;
    int32_t fund;
};

#endif // STOCKPRICEITEM_H
