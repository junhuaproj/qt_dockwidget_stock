/*
 * 分时线Dock
*/
#ifndef MINUTECHARTDOCKWIDGET_H
#define MINUTECHARTDOCKWIDGET_H

#include <QDockWidget>
#include <QObject>
#include "stockchartview.h"
#include "netrequest.h"
#include <QHBoxLayout>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include "idataloaderminute.h"
#include "stockminutechart.h"
//#include "IMainWidgetQSS.h"
#include "widget/basedockwidget.h"
#include "bg/netthreadpool.h"
namespace datasohu{
class DataLoaderNetMinute;
}

#define DATALOADER_MINUTE_COUNT     2
class MinuteChartDockWidget : public BaseDockWidget//,public INetResponse//,public IMainWidgetQSS
{
    Q_OBJECT
public:
    MinuteChartDockWidget(NetThreadPool* netThreadPool,const QString& title,QWidget* parent=nullptr);

    //virtual const char* qssName();
    //virtual bool reloadQss(const char* theme);
    virtual void    getThemingWidget(QList<QWidget*>& wids);
    void createUI();
    virtual ~MinuteChartDockWidget();

    void setStock(const StockListItem* stock,const QDate& date,double preClose);
    void setStock(const StockListItem* stock,const QDate& start,const QDate& end,double preClose,bool today=false);
    //virtual void dataRecieved(NetRequest* request,const QByteArray& data);
    const minutechart::StockMinuteChart* getMinuteChart()const;

protected slots:
    void onDayMinuteUpdated(const StockListItem*,minutechart::StockDayMinute*);
    //void onRequestFinished(NetRequest*,bool);
    void onSelectItem(ChartWidget*,ChartPos*);
    void onSelectRange(ChartWidget*,ChartPos* start,ChartPos* end,bool changing);
    //void onMinCurrTime(const StockPriceItem*);
protected:
    datasohu::DataLoaderNetMinute* getMinuteRequest();
    bool todayIsTradeTime(const QDate& date)const;

protected:
    bool bTodayUpdate;
    NetThreadPool* pNetPool;
    IDataLoaderMinute* loader[DATALOADER_MINUTE_COUNT];
    const StockListItem* pStock;
    //datasohu::DataLoaderNetMinute* netRequest;
    QHBoxLayout * layoutStatus;
    QComboBox* cbxPriority;
    QLabel* lblStatus;
    QWidget* widgetMain;
    QVBoxLayout* layoutMain;
    //StockChartView *minChart;
    minutechart::StockMinuteChart* minChart;
signals:
    void currentMessage(const QString& msg);
};

#endif // MINUTECHARTDOCKWIDGET_H
