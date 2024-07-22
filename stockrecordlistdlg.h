/*
 * 记录列表对话框
*/
#ifndef STOCKRECORDLISTDLG_H
#define STOCKRECORDLISTDLG_H

//#include <QDialog>
#include "widget/basedialog.h"

namespace Ui {
class StockRecordListDlg;
}

class StockListItem;
namespace data{
class StockRecordList;
}
//using namespace data;

class StockRecordListDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit StockRecordListDlg(QWidget *parent = nullptr);
    ~StockRecordListDlg();

    virtual void accept() override;
    void setStocks(const QList<StockListItem*>& stocks);
    void setRecordList(const data::StockRecordList* list);
    void getRecordList(data::StockRecordList* list);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
private:
    QList<StockListItem*> stocks;
    Ui::StockRecordListDlg *ui;
};

#endif // STOCKRECORDLISTDLG_H
