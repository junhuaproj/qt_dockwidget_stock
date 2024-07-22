/*
 * 记录
*/
#pragma once

#include <qwidget.h>
#include <qlistwidget.h>
#include <qboxlayout.h>
#include <qlist.h>
#include "widget/basewidget.h"

class StockListItem;
class MyDB;
namespace data{
class StockRecordDb;
class StockRecordList;
class StockRecordItem;
}
class RecordWidget :public BaseWidget
{
	Q_OBJECT
public:
    RecordWidget(MyDB* recordDB,QWidget* parent = nullptr);
	virtual ~RecordWidget();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    void setStockItem(const StockListItem* pStock);
    void setRecordList(data::StockRecordList* recordList);
    void addRecord(data::StockRecordItem* pItem);
    void removeRecord(int parent);
	void clear();
protected slots:
	void onListDbClick(QModelIndex);
signals:
    void recordItemChanged(const StockListItem* pStock,data::StockRecordItem* item);
protected:
    void addRecordItem(data::StockRecordItem* pItem);
protected:
    QList<data::StockRecordItem*> items;
    MyDB* mydb;
    data::StockRecordList* parentList;
    const StockListItem* pStock;
	QVBoxLayout* layout;
	QListWidget* listRecord;
};

