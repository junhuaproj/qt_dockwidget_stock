/*
 * 包括K线图的基类
*/
#ifndef USERMAINWND_H
#define USERMAINWND_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QDockWidget>
#include <stockchartview.h>

#include <QToolBox>
#include <QCalendarWidget>
#include <dataloader.h>

#include "stockdaychart.h"
#include "StatisticIndex.h"
#include "appconfig.h"
#include "filterdata.h"
#include "tdxminutedata.h"
#include "minutechartdockwidget.h"
//#include "IMainWidgetQSS.h"
#include "widget/basemainwindow.h"
#include "widget/basedockwidget.h"

class StockPriceChgWidget;

class UserMainWnd : public BaseMainWindow//,public IMainWidgetQSS
{
    Q_OBJECT
public:
    explicit UserMainWnd(StockListItem* mainStock,NetThreadPool* netThreadPool,data::DataLoader* pDataLoader,QWidget *parent = nullptr);

    virtual ~UserMainWnd();

    virtual void getDockWidgets(QList<QDockWidget*>& docks);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    virtual const char* qssName()const;
    virtual QWidget* getWidget();

protected:

    virtual void loadUi();
    virtual void connectSignal();
    //void loadStock(const QString& code,loadStock* pStockDay,StockDayChartView *dayChart);
    void loadStock(StockListItem* pItem,StockDay* pStockDay,chart::StockDayChart *dayChart);
    void onShowStockMinuteChart(int prevIndex,const StockListItem* stock,const QDate& date);
    //void addOneDayXLabel(int day);

    //void loadChartCfg(StockDayChart* chart);

    void reCalcDayChartRange(chart::StockDayChart* view,int start,int count);

    void setPriceDate(const QDate& date);
    void createPriceChgDock();

    virtual chart::StockDayChart* getCurDayChart();

    void showDayChartContextMenu(chart::StockDayChart* chart);
protected:
    chart::StockDayChart *curDayChart;
    QWidget* mainWidget;
    chart::StockDayChart *dayChart;
    MinuteChartDockWidget* minuteDock;
    //StockChartView *minChart;
    BaseDockWidget *dockTool;
    QToolBox* toolBox;

    QCalendarWidget* calendarWidget;
    //QDockWidget *dockMin;
    //QWidget* dockMinWidget;
    //QVBoxLayout* dockMinLayout;
    QVBoxLayout* mainLayout;

    BaseDockWidget* dockPrice;
    StockPriceChgWidget* priceChgWidget;
    //QWidget* toolPage;
    //QVBoxLayout* toolLayout;

    //StockDayItems dayItems;

    //data::DataLoader* pDataLoader;

protected slots:
    void onDayChartChanged(chart::StockDayChart*,int start,int count);
    void onDayChartCurrStockDay(chart::StockDayChart*,int);
    void onCalendar(const QDate);

    void onDayChartContext(QPoint);
    void onActionConfig(bool);
    void onActionCalcParam(bool);

    void onActionTheme(bool);
    void onReCalcIndex(bool);

protected:
    void getAllCfg(QList<chart::StatisticIndexCfg*>& cfgs);
    //const StockListItem* pCurrStock;
    StockListItem* mainStock;
    StockDay stockDay;

    bool isStockValid()const;

    void loadFromSQLite(StockDay* stockDay,const QString& code,const QString& head);
    int filterStockPercent(StockDay* stockDay,FilterCompareType compareType,int percent,std::vector<PFilterItemPercent>& out);

    void loadFromSQLite(StockDay* stockDay,const StockListItem* p);
    void loadFromTdx(StockDay* stockDay,const StockListItem* p,tdx::TDXMinuteData *pTDXLoader);

    int findDate(StockDay* stockDay,const QDate& date);

    void clear();

    const QString& getCode()const;

    StockDay* getStockDay();
};



#endif // USERMAINWND_H
