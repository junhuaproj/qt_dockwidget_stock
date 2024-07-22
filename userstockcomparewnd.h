/*
 * 股票日线对比Widget
 * 2个K线图
*/

#ifndef USERSTOCKCOMPAREWND_H
#define USERSTOCKCOMPAREWND_H

#include "usermainwnd.h"
#include <QSplitter>
class UserStockCompareWnd : public UserMainWnd
{
    Q_OBJECT
public:
    UserStockCompareWnd(StockListItem* mainStock,NetThreadPool* netThreadPool,data::DataLoader* pDataLoader,StockListItem* slaveStock=nullptr,QWidget *parent = nullptr);

    virtual void loadUi() override;
    virtual void connectSignal() override;
    // virtual bool reloadQss(const char* theme);

    const StockListItem* getMainStock()const;
    const StockListItem* getSlaveStock()const;
    void setSlaveStock(StockListItem*);
    void setMainStock(StockListItem*);
protected:
    //StockListItem* mainStock;
    StockListItem* slaveStock;

    chart::StockDayChart* dayChart2;
    QSplitter* splitterChart;
    StockDay stockDay2;
protected slots:
    void onCalendarChart2(const QDate);
    void onDayChartChanged(chart::StockDayChart*,int,int);

    void onDayChart2Context(QPoint);

    void onDayChartCurrDay(int);
    void onDayChart2CurrDay(int);
};

#endif // USERSTOCKCOMPAREWND_H
