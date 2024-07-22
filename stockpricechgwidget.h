/*
 * 日所有股票涨跌百分比分级
*/
#ifndef STOCKPRICECHGWIDGET_H
#define STOCKPRICECHGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <dataloader.h>
#include <QList>
#include <QChartView>
#include <QTabWidget>
#include <valuelevelitem.h>
#include "widget/basewidget.h"

namespace data {
class DataLoader;
}
class PriceItem{
public:
    PriceItem(const QString& tag,int per)
        :percent(per),title(tag)
    {
        //title=QString("%1").arg(tag).arg(per);
    }
//protected:
    QString title;
    int percent;
};

class StockPriceChgWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit StockPriceChgWidget(data::DataLoader* loader, QWidget *parent = nullptr);

    virtual ~StockPriceChgWidget();
    void createUI();
    void clear();
    void setDate(const QDate& date);

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected:
    void updatePrices();
    void priceToLevel(const float* prices,int priceCount,const float* levelRange,int rangeCount,int* levels,int levelCount);
    data::DataLoader* pDataLoader;
    QVBoxLayout* layoutMain;
    QTreeWidget* treeWidget;
    //QListWidget* listWidget;
    QTabWidget* tabWidget;

    QChartView* chartView;

    int stockCount;
    QList<PriceItem*> priceClasses;
    ValueItemLevelThree levels;
signals:
};

#endif // STOCKPRICECHGWIDGET_H
