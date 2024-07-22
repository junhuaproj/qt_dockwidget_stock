/*
 * 实时股票信息，用于股票列表显示
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
