/*
 * 记录Dialog
 * 修改记录
*/
#ifndef STOCKRECORDDLG_H
#define STOCKRECORDDLG_H

//#include <QDialog>
#include "widget/basedialog.h"
namespace Ui {
class StockRecordDlg;
}

#include "stockrecorditem.h"
#include "stockrecordlist.h"
//namespace data {
class StockListItem;
//}

class StockRecordDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit StockRecordDlg(const StockListItem* pStock,data::StockRecordItem* record=nullptr,const data::StockRecordList* pList=nullptr,QWidget *parent = nullptr);
    ~StockRecordDlg();

    virtual void accept() override;
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected:
    const StockListItem* pStock;
    const data::StockRecordList* pList;
    data::StockRecordItem* pRecord;
private:
    Ui::StockRecordDlg *ui;
};

#endif // STOCKRECORDDLG_H
