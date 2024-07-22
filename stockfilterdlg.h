
#ifndef STOCKFILTERDLG_H
#define STOCKFILTERDLG_H

#include <QDialog>
//#include "IMainWidgetQSS.h"
#include "widget/basedialog.h"
namespace Ui {
class StockFilterDlg;
}

namespace data {
class DataLoader;
class StockRecordList;
class StockRecordItem;
}

class StockListItem;


class StockFilterDlg : public BaseDialog//,public IMainWidgetQSS
{
    Q_OBJECT

public:
    explicit StockFilterDlg(data::DataLoader* pDataLoad,QWidget *parent = nullptr);
    ~StockFilterDlg();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    //virtual const char* qssName();
    //virtual bool reloadQss(const char* theme);
    // void getResultList(QList<StockListItem*>& lists);
    void moveRecordItems(QList<data::StockRecordItem*>& out);
    void parseIndex(const QString& s,QStringList& names);
protected slots:
    void onOpenFile(bool);
    void onDelete(bool);
    void onFilter(bool);
    void onParse(bool);
    void onSearch(bool);
    void onParseTable(bool);
    void onSectionClicked(int index);
    void onTableDbClick(const QModelIndex &index);
protected:
    bool isList;//true:recordlist,false recorditem
    Qt::SortOrder sortOrder;
    int sortColumn;
    data::DataLoader* pDataLoad;

    QList<data::StockRecordList*> lists;
    QList<data::StockRecordItem*> items;
    void clearItemAndList();

    void setTabRecordItem();
#ifdef DB_MYSQL
    void setResultCodes(const QList<StockListItem*>& codes);
#else
    void setResultCodes(const QStringList& codes);
#endif
protected:
    void closeEvent(QCloseEvent *)override;
signals:
    void apply(StockFilterDlg*);
    void closed(StockFilterDlg*);
    void openRecordList(data::StockRecordList*);
    void selectStock(StockListItem*);
private:
    Ui::StockFilterDlg *ui;
};

#endif // STOCKFILTERDLG_H
