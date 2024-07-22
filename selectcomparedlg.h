/*
 * 选择比较股票的对话框
*/
#ifndef SELECTCOMPAREDLG_H
#define SELECTCOMPAREDLG_H

#include <QDialog>
#include <QList>
#include <QMdiSubWindow>

namespace Ui {
class SelectCompareDlg;
}

class StockListItem;

namespace compare{
enum CompareStockReplace{
    CompareWndReplace_Main,
    CompareWndReplace_Slave,
};

class SelectCompareDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelectCompareDlg(StockListItem* pStockItem,const QList<QMdiSubWindow*>& wnds,CompareStockReplace replaceType,QWidget *parent = nullptr);
    ~SelectCompareDlg();
    QMdiSubWindow* getSelectedWin()const;
    CompareStockReplace getReplaceMode()const;
    virtual void accept();
protected:
    StockListItem* pStockItem;
    QMdiSubWindow* destWnd;
    CompareStockReplace stockReplace;
private:
    Ui::SelectCompareDlg *ui;
};
}
#endif // SELECTCOMPAREDLG_H
