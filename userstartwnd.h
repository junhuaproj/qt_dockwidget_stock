/*
 * 分析的主窗口
 * 股票清单列表Dock，
 * 一个日K线图，
 * 分时线图Dock
 * 股票信息Dock
 * 模拟交易列表Dock
 * 指定股票的历史涨跌百分比分级
 * 指定日期的所有股票涨跌百分比分级
*/

#ifndef USERSTARTWND_H
#define USERSTARTWND_H

#include <QListWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QCheckBox>
#include <QPlainTextEdit>
#include "simulatetradedockwidget.h"
//#include "stockpricechgwidget.h"
#include "stockpchglevelwidget.h"

#include "usermainwnd.h"
#include <chartstatusdockwidget.h>
//#include "stocklistdockwidget.h"

enum UserStartDock
{
    UserStartDock_Stock=0x01,
    UserStartDock_Minute=0x02,
    UserStartDock_tool=0x04,
};

namespace data{
class StockRecordList;
}
class StockFilterDlg;

class RecordListWidget;
class RecordWidget;
class StockSearchWidget;
namespace simulate {
class SimulateTrade;
}

class StockInfoDockWidget;

using namespace simulate;

class UserStartWnd : public UserMainWnd
{
    Q_OBJECT
public:
    UserStartWnd(NetThreadPool* netThreadPool,data::DataLoader* p,QWidget *parent = nullptr);
    virtual ~UserStartWnd();

    //virtual bool reloadQss(const char* theme);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;

    bool isDockVisible(UserStartDock flag);
    void setDockStockVisible(UserStartDock flag,bool visible);

    virtual void getDockWidgets(QList<QDockWidget*>& docks);
protected:
    virtual void loadUi() override;

    //void setStockToList(QList<StockListItem*>* stocks);


protected slots:
    void onMinuteChartSelectItem(ChartWidget*,ChartPos*);
    void onMinuteChartSelectRange(ChartWidget*,ChartPos*,ChartPos*,bool);

    void onDayChartMainSelectItem(ChartWidget*,ChartPos*);
    void onDayChartMainSelectedRange(ChartWidget*,ChartPos*,ChartPos*,bool);

    void onRecordItemChanged(const StockListItem*,data::StockRecordItem*);
    void onStockClicked(StockListItem*);

    void dockStockVisibleChanged(bool);

    //void getStockListItem(const QList<QTableWidgetItem*>& items,QList<StockListItem*>& stocks);
    //void getSelectedStocks(QList<StockListItem*>& stocks);

    // void onOpenRecordList(data::StockRecordList*);

    void onNewStockRecordList(QList<StockListItem*> stocks);
    void onStockDbClick(StockListItem*);
    //void onActionBlack(bool);

    void recordListChanged(RecordListWidget*,data::StockRecordList*);
    void onRemoveRecordFromList(int listid,int recordid);//StockListItem*);

    void onSimulateTradeChanged(SimulateTradeDockWidget*,SimulateTrade*);
    void onLoadTradeFinished(SimulateTradeDockWidget*);

    // void onFilterApply(StockFilterDlg*);
protected:
    //dock stock
    //StockFilterDlg* pFilterDlg;
    QDockWidget* dockStock;
    //QTableWidget* tableStock;

    //QListWidget* listStockRecord;
    //QDockWidget* dockStockInfo;
    //QPlainTextEdit* textStockInfo;
    StockInfoDockWidget* dockStockInfo;

    //QDockWidget* dockTable;
    simulate::SimulateTradeDockWidget* tradeDockWidget;
    //QTableWidget* multiFuncTable;
    //QWidget* widgetTable;
    //QVBoxLayout* dockTableLayout;

    // RecordListWidget* recordListWidget;
    ChartStatusDockWidget* chartStatusDockWidget;

    //RecordWidget* recordWidget;

    StockSearchWidget* stockWidget;
    StockPChgLevelDockWidget* pchgLevelDockWidget;
    void loadDockStock();

    //StockListDockWidget* listDockWidget;

protected:

    //StockRecordList* curRecordList;
signals:
    void dockVisibleChanged(UserStartDock flag,bool visible);
    void onNewWinCode(StockListItem* p);
    void onAddTodayStock(StockListItem* p);
    void onAddTodayStocks(QList<StockListItem*>);

    void onNewWinCompare(StockListItem* p1,StockListItem* p2);
};



#endif // USERSTARTWND_H
