#ifndef STOCKINFODOCKWIDGET_H
#define STOCKINFODOCKWIDGET_H


/*
 * 股票信息Dock
*/
#include <QDockWidget>
#include <QTabWidget>
#include <QPlainTextEdit>
#include "widget/basedockwidget.h"
class RecordWidget;

class StockListItem;
namespace data {
    class StockRecordItem;
class DataLoader;
    class StockRecordList;
}


class StockInfoDockWidget:public BaseDockWidget
{
    Q_OBJECT
public:
    explicit StockInfoDockWidget(data::DataLoader* pDataLoader,const QString &title, QWidget *parent = nullptr,
                        Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~StockInfoDockWidget();

    void setStockItem(const StockListItem* pStock);
    void setRecordList(data::StockRecordList* recordList);
    void removeRecord(int parent);
    virtual void    getThemingWidget(QList<QWidget*>& wids);
protected:
    void createUI();

    data::DataLoader* pDataLoader;
    //data::StockRecordList* parentList;

    QTabWidget* tabWidget;
    QPlainTextEdit* textStockInfo;
    RecordWidget* recordWidget;
signals:
    void recordItemChanged(const StockListItem* pStock,data::StockRecordItem* item);
};

#endif // STOCKINFODOCKWIDGET_H
