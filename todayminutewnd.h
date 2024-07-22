#ifndef TODAYMINUTEWND_H
#define TODAYMINUTEWND_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QList>
#include <stocklistitem.h>
#include <QGridLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <dataloader.h>
/*
 * 实时报价Widget
 * 列表显示报价
 * 显示多个股票分时图
*/
//#include "stockchartview.h"
#include "stockminutechart.h"
#include "dataloadernetsohu.h"
#include "appconfig.h"
#include "netrequest.h"
//#include "IMainWidgetQSS.h"
#include "widget/basemainwindow.h"

class TodayMinuteWnd : public BaseMainWindow   
{
    Q_OBJECT
public:
    explicit TodayMinuteWnd(NetThreadPool* netThreadPool,data::DataLoader* dataLoader=NULL,QWidget *parent = nullptr);

    virtual ~TodayMinuteWnd();
    void removeStockCode(StockListItem* pItem);
    void addStock(StockListItem* pItem);//,bool checkUpdate=true);
    void addStocks(QList<StockListItem*>& stocks);
    bool isStockExist(StockListItem* pItem);

    void loadChart(StockChartView* pChart,StockListItem* pItem);

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    virtual void getDockWidgets(QList<QDockWidget*>& docks);

    virtual const char* qssName()const;
    virtual QWidget* getWidget();

    void loadStocks();
protected:
    void setTableStatus(int row,StockLastStatus*);
protected:
    QSplitter* splitter;
    QTableWidget* tableWidget;
    QWidget* chartWidget;
    //QHBoxLayout* mainLayout;

    QList<minutechart::StockMinuteChart*> minuteCharts;
    QVBoxLayout* vBoxLayout;
    QGridLayout* gridLayout;

    QHBoxLayout* hBoxBottomLayout;
    QPushButton* btnNextPage;
    QPushButton* btnPrevPage;
    QPushButton* btnCfgPage;
    QPushButton* btnSave;
    QLabel* lblSummary;

    //QTimer timerUpdate;
    TodayMinuteCfg todayCfg;

    int chartStockIndex;//第一个图表显示的StockIndex
    int updateChartIndex;

protected slots:

    void onBtnPrevPage(bool);
    void onBtnNextPage(bool);
    void onBtnCfgPage(bool);
    void onBtnSave(bool);

    void onStockChanged();
    void onStockContext(QPoint);

    void onActionRemoveStock(bool);
    void onStockRemoved(StockListItem* pItem);
    void onTableCellDoubleClicked(int row,int col);

    void onSelectItem(ChartWidget*,ChartPos*);
    void onSelectRange(ChartWidget*,ChartPos*,ChartPos*,bool);

    void onDayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*);
    void onLastStatusChanged(const QList<StockLastStatus*>&);
protected:
    void loadUI();
    void loadUIRight();

    void clearChartsFromLayout();
    void onCfgChanged();
    void showStockChart(int firstIndex);

    minutechart::StockMinuteChart* findChart(const StockListItem* pItem);
    minutechart::StockMinuteChart* findEmptyChart();

    StockListItem* getStockFromTableItem(QTableWidgetItem* pItem);
protected:
    QList<StockListItem*> stocks;
};
#endif // TODAYMINUTEWND_H
