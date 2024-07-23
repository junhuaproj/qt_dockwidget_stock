/*
 * 指定股票涨跌百分比Widget
*/
#ifndef STOCKPCHGLEVELWIDGET_H
#define STOCKPCHGLEVELWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include <IMainWidgetQSS.h>
#include "valuelevelitem.h"
#include "widget/basewidget.h"
#include "widget/basedockwidget.h"

class StockListItem;


class StockPChgLevelWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit StockPChgLevelWidget(QWidget *parent = nullptr);
    virtual ~StockPChgLevelWidget();

    // virtual const char* qssName();
    // virtual bool reloadQss(const char* theme);
    void setStock(StockListItem* stock,const double* pchg,int count);
    virtual void    getThemingWidget(QList<QWidget*>& wids)const;
protected:
    void setTree();
    void addChildTreeItem(QTreeWidgetItem* parent,QList<ValueLevelItem*>* children );
    ValueItemLevelTree levels;
    //int levelCount;
    //ValueLevelItem* levels;
    //int valueCount;

    QVBoxLayout* layoutMain;

    QTreeWidget* tree;
signals:
};

class StockPChgLevelDockWidget : public BaseDockWidget//,public IMainWidgetQSS
{
    Q_OBJECT
public:
    explicit StockPChgLevelDockWidget(const QString &title, QWidget *parent = nullptr,
                                      Qt::WindowFlags flags = Qt::WindowFlags());

    void setStock(StockListItem* stock,const double* pchg,int count);
    virtual void    getThemingWidget(QList<QWidget*>& wids);
    // virtual const char* qssName();
    // virtual bool reloadQss(const char* theme);
protected:
    StockPChgLevelWidget* pchgLevel;
signals:
};
#endif // STOCKPCHGLEVELWIDGET_H
