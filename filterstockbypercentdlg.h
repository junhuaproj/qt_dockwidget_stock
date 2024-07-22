#ifndef FILTERSTOCKBYPERCENTDLG_H
#define FILTERSTOCKBYPERCENTDLG_H

//#include <QDialog>
#include "widget/basedialog.h"

#include "appconfig.h"
namespace Ui {
class FilterStockByStockDlg;
}

class FilterStockByStockDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit FilterStockByStockDlg(int percent=5,FilterCompareType compareType=FilterCompareType_AbsGreatThan,QWidget *parent = nullptr);
    ~FilterStockByStockDlg();

    FilterCompareType getCompareType()const;
    int getPercent()const;
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected:
    FilterCompareType compareType;
    int percent;
protected slots:
    void onAccept();
private:
    Ui::FilterStockByStockDlg *ui;
};

#endif // FILTERSTOCKBYPERCENTDLG_H
