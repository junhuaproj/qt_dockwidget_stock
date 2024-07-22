/*
 * 启动后的窗口，所有窗口的容器
*/
#ifndef MAINWINDOWMDI_H
#define MAINWINDOWMDI_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "dataloader.h"
#include "todayminutewnd.h"
#include "userstartwnd.h"
#include "userstockcomparewnd.h"
#include "bg/netthreadpool.h"
#include "widget/ithemewidget.h"

#include "widget/basemdiarea.h"

namespace Ui {
class MainWindowMdi;
}

class MainWindowMdi : public QMainWindow,public IThemeWidget
{
    Q_OBJECT

public:
    explicit MainWindowMdi(QWidget *parent = nullptr);
    ~MainWindowMdi();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    virtual const char* qssName()const;
    virtual QWidget* getWidget();
protected:
    QMdiSubWindow* mainWindow;
    QMdiSubWindow* todayWnd;

    TodayMinuteWnd* getTodayMinuteWnd();

    QList<QMdiSubWindow*> compareWndList;
protected slots:
    void onCompareWndDestory(QObject*);
    void onMainWinTriggered(bool);
    void onMainWindowDestory(QObject*);
    void onTodayWndDestory(QObject*);

    //void dockStockVisibleChanged(bool);
    void onNewStockWinCode(StockListItem* pItem);
    void onAddTodayStock(StockListItem* pItem);
    void onAddTodayStock(QList<StockListItem*>);

    void onNewWinCompare(StockListItem* p1,StockListItem* p2);

    void onSubWindowActivated(QMdiSubWindow*);

    void onDockVisibleChanged(UserStartDock,bool);
    void onActionMainDockStock(bool);

    void onCloseAllSubWin(bool);
    void onTableSubWin(bool);
    void onCascadeSubWin(bool);
    void onTileSubWin(bool);
    void onActiveWndDock(bool);

    void onCurrentMessage(QString msg);

    void onUpdateQSS(bool);
    void onActionTodayWin(bool);
    // void updateQss(QWidget* widget);

    inline QMdiArea* getMdiArea()
    {
#ifdef BASEMDIAREA_H
        return mdiArea;
#else
        return ui->mdiArea;
#endif
    }
protected:
    data::DataLoader dataLoader;
    NetThreadPool* netThreadPool;
private:
#ifdef BASEMDIAREA_H
    BaseMdiArea* mdiArea;
#endif
    Ui::MainWindowMdi *ui;
};
Q_DECLARE_METATYPE(MainWindowMdi);
#endif // MAINWINDOWMDI_H
