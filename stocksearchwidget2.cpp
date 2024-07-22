#include "stocksearchwidget.h"
#include <QHeaderView>
#include "appconfig.h"
#include <QMenu>
#include "stockfilterdlg.h"

void StockSearchWidget::createUI()
{
    tableStock=new QTableWidget(this);
    dockMainLayout=new QVBoxLayout(this);

    tabFilterCls=new QTabWidget(this);


    ///股票、指数
    QWidget* stock_index=new QWidget(this);

    QHBoxLayout* layoutStockType=new QHBoxLayout(stock_index);

    radioStock=new QRadioButton(tr("stock"),this);
    radioIndex=new QRadioButton(tr("index"),this);
    layoutStockType->addWidget(radioStock);
    layoutStockType->addWidget(radioIndex);

    stock_index->setLayout(layoutStockType);
    tabFilterCls->addTab(stock_index,tr("stock&index"));


    //查询
    QWidget* searchWnd=new QWidget(this);
    QHBoxLayout* layoutSearch=new QHBoxLayout(searchWnd);
    lineEditKey=new QLineEdit(this);
    buttonSearch=new QPushButton(tr("Search"),this);
    layoutSearch->addWidget(lineEditKey);
    layoutSearch->addWidget(buttonSearch);

    searchWnd->setLayout(layoutSearch);
    tabFilterCls->addTab(searchWnd,tr("search"));
    //list
    QWidget* listWnd=new QWidget(this);
    buttonOpenList=new QPushButton(tr("open"),listWnd);;
    buttonCloseList=new QPushButton(tr("close"),listWnd);
    buttonEditList=new QPushButton(tr("edit"),listWnd);
    buttonDeleteList=new QPushButton(tr("delete"),listWnd);
    connect(buttonDeleteList,SIGNAL(clicked(bool)),this,SLOT(onDeleteList(bool)));
    connect(buttonEditList,SIGNAL(clicked(bool)),this,SLOT(onEditList(bool)));
    connect(buttonCloseList,SIGNAL(clicked(bool)),SLOT(onCloseList(bool)));

    QHBoxLayout* layoutlist=new QHBoxLayout(searchWnd);
    layoutlist->addWidget(buttonOpenList);
    layoutlist->addWidget(buttonEditList);
    layoutlist->addWidget(buttonCloseList);
    layoutlist->addWidget(buttonDeleteList);

    listWnd->setLayout(layoutlist);
    tabFilterCls->addTab(listWnd,tr("list"));
    //cfg
    QWidget* cfgWnd=new QWidget(this);
    chkExtendInfo=new QCheckBox(tr("extend info"),cfgWnd);
    chkUpdateLastPrice=new QCheckBox(tr("update last price"),cfgWnd);
    chkRecordList=new QCheckBox(tr("record info"),cfgWnd);

    QHBoxLayout* layoutcfg=new QHBoxLayout(cfgWnd);
    layoutcfg->addWidget(chkExtendInfo);
    layoutcfg->addWidget(chkUpdateLastPrice);
    layoutcfg->addWidget(chkRecordList);

    cfgWnd->setLayout(layoutcfg);
    tabFilterCls->addTab(cfgWnd,tr("cfg"));
    connect(chkExtendInfo,SIGNAL(clicked(bool)),this,SLOT(onExtendInfo(bool)));
    connect(chkUpdateLastPrice,SIGNAL(clicked(bool)),this,SLOT(onUpdateLastPrice(bool)));
    connect(chkRecordList,SIGNAL(clicked(bool)),this,SLOT(onRecordInfo(bool)));

    dockMainLayout->addWidget(tabFilterCls);
    dockMainLayout->setStretch(0,0);


    tableStock->setSelectionBehavior(QTableWidget::SelectRows);

    dockMainLayout->addWidget(tableStock);
    dockMainLayout->setStretch(1,1);
    dockMainLayout->setSpacing(1);
    setLayout(dockMainLayout);

    radioStock->setChecked(true);

    connect(radioStock,SIGNAL(clicked(bool)),this,SLOT(onRadioStock(bool)));
    connect(radioIndex,SIGNAL(clicked(bool)),this,SLOT(onRadioIndex(bool)));
    //connect(checkFilter,SIGNAL(clicked(bool)),this,SLOT(onCheckFilter(bool)));
    connect(buttonSearch,SIGNAL(clicked(bool)),this,SLOT(onBtnSearch(bool)));
    connect(buttonOpenList,SIGNAL(clicked(bool)),this,SLOT(onBtnOpenList(bool)));

    connect(tableStock,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(onDoubleClickStock(QModelIndex)));
    tableStock->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tableStock,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onStockContext(QPoint)));

    connect(tableStock,SIGNAL(cellClicked(int,int)),this,SLOT(onTableCellClicked(int,int)));
    connect(tableStock,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(onCurrentCellChanged(int,int,int,int)));

    connect(tableStock->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(onSectionClicked(int)));

    QStringList tableHeader;

    tableHeader<<tr("code")<<tr("name")<<tr("level")<<tr("lastdate")<<tr("percent")<<tr("pricestatus");

    tableStock->setColumnCount(tableHeader.size());

    tableStock->setHorizontalHeaderLabels(tableHeader);

    for(int i=0;i<tableHeader.size();i++)
        tableStock->setColumnWidth(i,50);//setSectionResizeMode(i,QHeaderView::ResizeToContents);

    //tableStock->verticalHeader()->setVisible(false);
    tableStock->setEditTriggers(QTableWidget::NoEditTriggers);
    //onCheckFilter(false);
    //qss.loadQSS(tableStock,"tableview");
}
void StockSearchWidget::onStockContext(QPoint)
{
    //QAction* actionChart1=new QAction("图表1");
    //QAction* actionChart2=new QAction("图表2");
    QAction* actionNewWin=new QAction("new Win");
    QAction* actionAddToDay=new QAction("Add To Today");
    QAction* actionNewWinCompare=new QAction("New Win Compare");
    QAction* actionStockNote=new QAction(tr("stock note"));;
    QAction* actionStockBlack=new QAction(tr("black list"));;

    QAction* actionRemoveFromList=NULL;
    //if(checkFilter->isChecked())
    {
        actionRemoveFromList=new QAction(tr("remove from list"));
        connect(actionRemoveFromList,SIGNAL(triggered(bool)),this,SLOT(onActionRemoveStockFromList(bool)));
    }
    QMenu* menu=new QMenu();
    connect(actionStockNote,SIGNAL(triggered(bool)),this,SLOT(onActionStockNote(bool)));

    connect(actionNewWin,SIGNAL(triggered(bool)),this,SLOT(onActionChartNew(bool)));
    connect(actionAddToDay,SIGNAL(triggered(bool)),this,SLOT(onActionAddTodayStock(bool)));
    connect(actionNewWinCompare,SIGNAL(triggered(bool)),this,SLOT(onActionNewCompare(bool)));

    connect(actionStockBlack,SIGNAL(triggered(bool)),this,SLOT(onActionBlack(bool)));

    menu->addAction(actionNewWin);
    menu->addAction(actionAddToDay);
    menu->addAction(actionNewWinCompare);
    menu->addAction(actionStockNote);
    menu->addAction(actionStockBlack);
    //if(checkFilter->isChecked())
    {
        menu->addAction(actionRemoveFromList);
    }
    menu->exec(QCursor::pos());
}
void StockSearchWidget::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(tableStock);
    wids.push_back(radioStock);
    wids.push_back(radioIndex);
    wids.push_back(buttonOpenList);
    wids.push_back(buttonSearch);
    wids.push_back(buttonCloseList);
    wids.push_back(buttonEditList);
    wids.push_back(buttonDeleteList);
    wids.push_back(lineEditKey);
    wids.push_back(tabFilterCls);
}

void StockSearchWidget::onSectionClicked(int index)
{
    if(index==sortColumn)
    {
        if(sortOrder==Qt::AscendingOrder)
            sortOrder=Qt::DescendingOrder;
        else
            sortOrder=Qt::AscendingOrder;
    }
    tableStock->sortByColumn(index,sortOrder);
    sortColumn=index;
}

void StockSearchWidget::onColumnChanged()
{
    QStringList titles;
    if(columns)
        free(columns);
    columns=this->getTableCols(&columnCount,titles);
    tableStock->setColumnCount(columnCount);
    tableStock->setHorizontalHeaderLabels(titles);
}

void StockSearchWidget::onBtnOpenList(bool)
{
    if(pFilterDlg==NULL)
    {
        pFilterDlg=new StockFilterDlg(pDataLoader,this);
        connect(pFilterDlg,SIGNAL(closed(StockFilterDlg*)),this,SLOT(onFilterFinished(StockFilterDlg*)));
        connect(pFilterDlg,SIGNAL(apply(StockFilterDlg*)),this,SLOT(onFilterApply(StockFilterDlg*)));
        connect(pFilterDlg,
                SIGNAL(openRecordList(data::StockRecordList*)),
                this,SLOT(onOpenRecordList(data::StockRecordList*)));// SIGNAL(openRecordList(StockRecordList*)));

        connect(pFilterDlg,SIGNAL(selectStock(StockListItem*)),this,SLOT(onSelectStock(StockListItem*)));

    }
    pFilterDlg->show();
}
