/*
 * 股票备注修改Dialog
*/
#ifndef STOCKNOTEDLG_H
#define STOCKNOTEDLG_H

#include <QDialog>
#include "widget/basedialog.h"

namespace Ui {
class StockNoteDlg;
}

// namespace data{
// class StockNote;
// }
class StockListItem;
class StockNoteDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit StockNoteDlg(const StockListItem* stock,QWidget *parent = nullptr);
    ~StockNoteDlg();

    virtual void accept();
    void getData(StockListItem* stock);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected:
    const StockListItem* pStock;
    // data::StockNote* stockNote;
private:
    Ui::StockNoteDlg *ui;
};

#endif // STOCKNOTEDLG_H
