/*
 * 模拟交易Dock
*/
#ifndef SIMULATETRADEDOCKWIDGET_H
#define SIMULATETRADEDOCKWIDGET_H

#include <QDockWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
//#include "IMainWidgetQSS.h"
//#include "simulatetradeloader.h"
#include <QPushButton>
#include "widget/basedockwidget.h"
class StockListItem;

namespace simulate{
class SimulateTrade;

class SimulateTradeLoader;
class TradeEditDlg;

class SimulateTradeDockWidget : public BaseDockWidget//,public IMainWidgetQSS
{
    Q_OBJECT
public:
    explicit SimulateTradeDockWidget(const QString& title,QWidget *parent = nullptr);
    virtual ~SimulateTradeDockWidget();

    virtual void    getThemingWidget(QList<QWidget*>& wids);
    void setStockItem(StockListItem* pStock);

    const QList<SimulateTrade*>* getTrades()const;
    //virtual const char* qssName();
    //virtual bool reloadQss(const char* theme);
protected slots:
    void onRequest(SimulateTradeLoader*,bool);
    void onCurrentTradeChanged(int,int,int,int);
    void onEditTrade(int,int);

    void onTradeDelete(TradeEditDlg*,int row,SimulateTrade* trade);
    void onTradeSave(TradeEditDlg*,int row,SimulateTrade* trade);
    void onSaveTrades(bool);
protected:
    void onTradesUpdated(int start=0);
    void setTableRow(int row,SimulateTrade* trade);
    int loadTrades(const QJsonArray& arr);
    bool loadLocalTrades(QString& lastDate);
protected:
    void clearTrade();
    QTableWidget* tableWidget;
    QWidget* widgetMain;
    QVBoxLayout* layoutMain;
    QHBoxLayout* layoutBtn;
    QComboBox* cbxTradeType;
    QPushButton* btnSaveTrade;
    QList<SimulateTrade*> trades;

    SimulateTradeLoader* tradeLoader;
    TradeEditDlg* tradeEditDlg;

    //QDateTime lastDate;
    const StockListItem* pStock;
signals:
    void tradeChanged(SimulateTradeDockWidget*,SimulateTrade*);
    void loadSimulateTradeFinished(SimulateTradeDockWidget*);
};
}
#endif // SIMULATETRADEDOCKWIDGET_H
