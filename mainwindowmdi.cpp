#include "mainwindowmdi.h"
#include "ui_mainwindowmdi.h"
#include "userstockwnd.h"
//#include "mainwindow.h"

#include "usermainwnd.h"
#include "todayminutewnd.h"
#include "selectcomparedlg.h"
#include "dockwidgetdlg.h"
#include "appconfig.h"
#include "AppConfigqss.h"
using namespace compare;

MainWindowMdi::MainWindowMdi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowMdi)
    ,mainWindow(NULL)
    ,todayWnd(NULL)
{
    ui->setupUi(this);
#ifdef BASEMDIAREA_H
    mdiArea=new BaseMdiArea(this);
    setCentralWidget(mdiArea);
    connect(mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(onSubWindowActivated(QMdiSubWindow*)));
#else
    connect(ui->mdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(onSubWindowActivated(QMdiSubWindow*)));
    this->setCentralWidget(ui->mdiArea);
#endif
    connect(ui->actionmainwin,SIGNAL(triggered(bool)),this,SLOT(onMainWinTriggered(bool)));
    dataLoader.loadStockList();
    dataLoader.loadStockIndex();

    connect(ui->actionmainstock,SIGNAL(toggled(bool)),this,SLOT(onActionMainDockStock(bool)));

    connect(ui->actioncascadesubwin,SIGNAL(toggled(bool)),this,SLOT(onCascadeSubWin(bool)));
    connect(ui->actioncloseall,SIGNAL(toggled(bool)),this,SLOT(onCloseAllSubWin(bool)));
    connect(ui->actiontablemode,SIGNAL(toggled(bool)),this,SLOT(onTableSubWin(bool)));
    connect(ui->actiontilesubwin,SIGNAL(toggled(bool)),this,SLOT(onTileSubWin(bool)));

    connect(ui->actiondock,SIGNAL(triggered(bool)),this,SLOT(onActiveWndDock(bool)));

    connect(ui->actionupdateqss,SIGNAL(triggered(bool)),this,SLOT(onUpdateQSS(bool)));

    connect(ui->actiontodaywin,SIGNAL(triggered(bool)),this,SLOT(onActionTodayWin(bool)));

    this->setWindowTitle("statistic");

    netThreadPool=new NetThreadPool(&dataLoader);
}

MainWindowMdi::~MainWindowMdi()
{
    delete ui;
    delete netThreadPool;
}

void MainWindowMdi::onActionTodayWin(bool)
{
    TodayMinuteWnd* wnd=getTodayMinuteWnd();
    wnd->show();
}

void MainWindowMdi::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(ui->menubar);
#ifdef BASEMDIAREA_H
    wids.push_back(mdiArea);
#else
    wids.push_back(ui->mdiArea);
#endif
    wids.push_back(ui->statusbar);
}

const char* MainWindowMdi::qssName()const
{
    return "MainWindowMdi";
}
QWidget* MainWindowMdi::getWidget()
{
    return this;
}

void MainWindowMdi::onUpdateQSS(bool)
{
    QMdiSubWindow* subWidget=getMdiArea()->activeSubWindow();

    const AppConfigQSS* qss=appConfig.qss();
    IThemeWidget* themeWidget=NULL;
    if(subWidget==NULL)
    {

        themeWidget=this;
    }
    else
    {
        themeWidget=dynamic_cast<IThemeWidget*>(subWidget->widget());
    }
    qss->updateQss(themeWidget);
}

void MainWindowMdi::onSubWindowActivated(QMdiSubWindow* pWnd)
{
    if(pWnd!=NULL&&pWnd==mainWindow)
    {
        ui->menuviewmain->setEnabled(true);
        UserStartWnd* pStart=(UserStartWnd*)pWnd->widget();
        ui->actionmainstock->setChecked(pStart->isDockVisible(UserStartDock_Stock));
        ui->actionmainminute->setChecked(pStart->isDockVisible(UserStartDock_Minute));
        ui->actionmaintool->setChecked(pStart->isDockVisible(UserStartDock_tool));
    }
    else if(pWnd==todayWnd)
    {
        ui->menuviewmain->setEnabled(false);
    }
}

void MainWindowMdi::onDockVisibleChanged(UserStartDock flag,bool visible)
{
    if(flag&UserStartDock_Stock)
        ui->actionmainstock->setChecked(visible);
    if(flag&UserStartDock_Minute)
        ui->actionmainminute->setChecked(visible);
    if(flag&UserStartDock_tool)
        ui->actionmaintool->setChecked(visible);
}

void MainWindowMdi::onCurrentMessage(QString msg)
{
    this->statusBar()->showMessage(msg);
}
void MainWindowMdi::onMainWinTriggered(bool)
{
    if(mainWindow==NULL)
    {
        UserStartWnd* win=new UserStartWnd(netThreadPool,&dataLoader,this);
        mainWindow=getMdiArea()->addSubWindow(win);

        connect(win,SIGNAL(destroyed(QObject*)),this,SLOT(onMainWindowDestory(QObject*)));
        connect(win,SIGNAL(onNewWinCode(StockListItem*)),this,SLOT(onNewStockWinCode(StockListItem*)));
        connect(win,SIGNAL(onAddTodayStock(StockListItem*)),this,SLOT(onAddTodayStock(StockListItem*)));
        connect(win,SIGNAL(onAddTodayStocks(QList<StockListItem*>)),this,SLOT(onAddTodayStock(QList<StockListItem*>)));

        connect(win,SIGNAL(dockVisibleChanged(UserStartDock,bool)),this,SLOT(onDockVisibleChanged(UserStartDock,bool)));

        connect(win,SIGNAL(onNewWinCompare(StockListItem*,StockListItem*)),this,SLOT(onNewWinCompare(StockListItem*,StockListItem*)));
        connect(win,SIGNAL(currentMessage(QString)),this,SLOT(onCurrentMessage(QString)));

        win->show();
        onSubWindowActivated(mainWindow);
    }
    else
    {
        getMdiArea()->setActiveSubWindow(mainWindow);

    }
    ui->actionmainwin->setChecked(true);
}

void MainWindowMdi::onAddTodayStock(QList<StockListItem*> stocks)
{
    TodayMinuteWnd* wnd=getTodayMinuteWnd();
    wnd->addStocks(stocks);
    wnd->show();
}
TodayMinuteWnd* MainWindowMdi::getTodayMinuteWnd()
{
    if(todayWnd==NULL)
    {
        TodayMinuteWnd* wnd=new TodayMinuteWnd(netThreadPool,&dataLoader,this);
        connect(wnd,SIGNAL(destroyed(QObject*)),this,SLOT(onTodayWndDestory(QObject*)));
        connect(wnd,SIGNAL(currentMessage(QString)),this,SLOT(onCurrentMessage(QString)));
        todayWnd=getMdiArea()->addSubWindow(wnd);
        return wnd;
    }
    else
    {
        return (TodayMinuteWnd*)todayWnd->widget();
    }
}
void MainWindowMdi::onAddTodayStock(StockListItem* pItem)
{
    TodayMinuteWnd* wnd=getTodayMinuteWnd();
    wnd->addStock(pItem);
    wnd->show();
}
void MainWindowMdi::onTodayWndDestory(QObject*)
{
    todayWnd=NULL;
}
void MainWindowMdi::onNewStockWinCode(StockListItem* pItem)
{
    UserStockWnd* win=new UserStockWnd(pItem,netThreadPool,&dataLoader,this);
    getMdiArea()->addSubWindow(win);
    win->show();
}

void MainWindowMdi::onMainWindowDestory(QObject*)
{
    mainWindow=NULL;
    ui->actionmainwin->setChecked(false);
}
void MainWindowMdi::onActionMainDockStock(bool bCheck)
{
    if(getMdiArea()->activeSubWindow()==mainWindow)
    {
        UserStartWnd* pStart=(UserStartWnd*)mainWindow->widget();
        pStart->setDockStockVisible(UserStartDock_Stock,bCheck);
    }
}
void MainWindowMdi::onCloseAllSubWin(bool)
{
    getMdiArea()->closeAllSubWindows();
}
void MainWindowMdi::onTableSubWin(bool bCheck)
{
    if(bCheck)
    {
        getMdiArea()->setViewMode(QMdiArea::TabbedView);
        getMdiArea()->setTabsClosable(true);
        getMdiArea()->setEnabled(false);
    }
    else
    {
        getMdiArea()->setViewMode(QMdiArea::SubWindowView);
        getMdiArea()->setEnabled(true);
    }
}
void MainWindowMdi::onCascadeSubWin(bool)
{
    getMdiArea()->cascadeSubWindows();
}
void MainWindowMdi::onTileSubWin(bool)
{
    getMdiArea()->tileSubWindows();
}

void MainWindowMdi::onActiveWndDock(bool)
{
    QMdiSubWindow* subWidget=getMdiArea()->activeSubWindow();
    if(subWidget==NULL)
        return ;
    UserMainWnd* p= (UserMainWnd*)subWidget->widget();
    if(p==NULL)return ;
    QList<QDockWidget*> docks;
    p->getDockWidgets(docks);
    DockWidgetDlg* dlg=new DockWidgetDlg(docks,p,this);
    if(dlg->exec()==QDialog::Accepted)
    {

    }
    delete dlg;
}

void MainWindowMdi::onCompareWndDestory(QObject* obj)
{
    QList<QMdiSubWindow*>::iterator it= compareWndList.begin(),end=compareWndList.end();
    for(;it!=end;it++)
    {
        if((*it)->widget()==obj)
        {
            compareWndList.removeOne(*it);
            return ;
        }
    }
}
void MainWindowMdi::onNewWinCompare(StockListItem* p1,StockListItem* p2)
{
    if(compareWndList.size()==0||(p1&&p2))
    {
        newWin:
        UserStockCompareWnd* wnd=new UserStockCompareWnd(p1,netThreadPool,&dataLoader,p2,this);
        connect(wnd,SIGNAL(destroyed(QObject*)),this,SLOT(onCompareWndDestory(QObject*)));
        compareWndList.append(getMdiArea()->addSubWindow(wnd));
        wnd->show();
    }
    else if(p1!=NULL||p2!=NULL)
    {
        StockListItem* pStock=p1;
        if(p1==NULL)
            pStock=p2;
        SelectCompareDlg* dlg=new SelectCompareDlg(pStock,compareWndList,CompareWndReplace_Slave,this);
        if(dlg->exec()==QDialog::Accepted)
        {
            CompareStockReplace replace=dlg->getReplaceMode();
            QMdiSubWindow* pWnd=dlg->getSelectedWin();
            if(pWnd==NULL)
                goto newWin;
            UserStockCompareWnd* userWnd=(UserStockCompareWnd*)pWnd->widget();
            if(replace==CompareWndReplace_Main)
                userWnd->setMainStock(pStock);
            else if(replace==CompareWndReplace_Slave)
                userWnd->setSlaveStock(pStock);
        }
        delete dlg;
        //compareWndList
    }
}
