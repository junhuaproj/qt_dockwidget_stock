/*
 * 股票、指数清单Widget，可用于查找，显示详情，从记录列表过滤
*/
#ifndef STOCKSEARCHWIDGET_H
#define STOCKSEARCHWIDGET_H

#include <QWidget>

#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QTimer>
#include <QTabWidget>
#include "dataloadernetsohu.h"
//#include "IMainWidgetQSS.h"
#include "widget/basewidget.h"
#include "bg/netthreadpool.h"
#include "data/TableStockMember.h"
class StockFilterDlg;
namespace data{
class DataLoader;

class StockRecordList;
class StockRecordItem;
// class StockNote;
}

class StockLastStatus;
class StockListItem;


class StockSearchWidget : public BaseWidget//,public INetResponse//,public IMainWidgetQSS
{
    Q_OBJECT
public:
    explicit StockSearchWidget(NetThreadPool* netThreadPool,data::DataLoader* p,QWidget *parent = nullptr);

    virtual ~StockSearchWidget();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    //virtual const char* qssName();
    //virtual bool reloadQss(const char* theme);

    //virtual void dataRecieved(NetRequest* request,const QByteArray& data);
    void createUI();
    TableStockMember* getTableCols(int* count,QStringList& titles);

    void setStockToList(const QList<StockListItem*>* stocks);
    void setRecordList(data::StockRecordList* recordList);
    void updateRecord(const StockListItem* pStock,data::StockRecordItem* pRecord);
protected slots:
    void onRecordInfo(bool);
    void onLastStatusChanged(const QList<StockLastStatus*>& status);
    void onSelectStock(StockListItem*);
    //void onTimerOut();
    //void onNetRequestFinished(NetRequest*,bool);
    void onActionRemoveStockFromList(bool);
    //void onCheckFilter(bool checked);
    void onRadioStock(bool);
    void onRadioIndex(bool);
    void onBtnSearch(bool);

    void onTableCellClicked(int row,int column);
    void onDoubleClickStock(QModelIndex index);
    void onActionStockNote(bool);
    void onActionBlack(bool);
    void onActionNewCompare(bool);
    void onStockContext(QPoint);
    void onActionAddTodayStock(bool);
    void onBtnOpenList(bool);
    void onFilterApply(StockFilterDlg* pdlg);


    void onActionChartNew(bool);
    void onOpenRecordList(data::StockRecordList* pRecordList);
    void onCurrentCellChanged(int,int,int,int);

    void onSectionClicked(int);
    void onExtendInfo(bool);
    void onUpdateLastPrice(bool);

    void onCloseList(bool);
    void onEditList(bool);
    void onDeleteList(bool);
public slots:
    void onFilterFinished(StockFilterDlg*);
protected:
    void resetTableRecordList();
    void onColumnChanged();
    void onRecordListStockChanged(data::StockRecordList* recordList);
    //void removeLastStatus(StockLastStatus*);
    //void requestLastStatus();

    void setTableRow(int row,const StockListItem* pStock);
    void setTableRow(int row,const data::StockRecordItem* pRecord);
    void setTableRow(int row,const StockLastStatus* lastStatus);
    // void setTableRow(int row,const data::StockNote* pNote);

    StockListItem* getCurrSelectStock();
    void getSelectedStocks(QList<StockListItem*>& stocks);
    void getStockListItem(const QList<QTableWidgetItem*>& items,QList<StockListItem*>& stocks);
protected:
    // datasohu::DataLoaderNetMinute* dataLoaderNet;
    QList<StockListItem*> stockItems;
    data::StockRecordList* recordList;
    //QStringList tableTitles;
    TableStockMember* columns;
    int columnCount;
    //
    void attachLastStatus(const QList<StockLastStatus*>& status);
    ////
    /// \brief sortColumn
    ///
    int sortColumn;
    Qt::SortOrder sortOrder;
    //////
    data::DataLoader* pDataLoader;
    NetThreadPool* pNetPool;
    //////
    StockFilterDlg* pFilterDlg;
    QTableWidget* tableStock;
    QVBoxLayout* dockMainLayout;

    QRadioButton* radioStock;
    QRadioButton* radioIndex;
    //QCheckBox* checkFilter;
    QPushButton* buttonSearch;
    //QPushButton* buttonAdvanceFilter;
    QLineEdit* lineEditKey;
    //QHBoxLayout* layoutStockType;
    //QHBoxLayout* layoutStockSearch;

    QPushButton* buttonOpenList;
    QPushButton* buttonCloseList;
    QPushButton* buttonEditList;
    QPushButton* buttonDeleteList;

    QCheckBox* chkExtendInfo;
    QCheckBox* chkUpdateLastPrice;
    QCheckBox* chkRecordList;
    QTabWidget* tabFilterCls;

    //QTimer timerUpdate;
signals:
    void addTodayStock(QList<StockListItem*> stocks);
    void stockClicked(StockListItem* pStock);
    void stockDbClicked(StockListItem* pStock);
    //void stockNoteAdd(StockListItem* pStock);
    void removeRecordFromList(int listid,int recordid);//StockListItem* pStock);
    void newWinCompare(StockListItem* p1,StockListItem* p2);

    void openRecordList(data::StockRecordList*);
    void newStockRecordList(QList<StockListItem*> stocks);

    void newStockWinCode(StockListItem* pStock);
    //void filterApply(StockFilterDlg*);
};

#endif // STOCKSEARCHWIDGET_H
