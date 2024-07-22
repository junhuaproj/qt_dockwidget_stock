
#ifndef USERSTOCKWND_H
#define USERSTOCKWND_H

#include <QPushButton>
#include <QTableWidget>

#include "usermainwnd.h"

class UserStockWnd : public UserMainWnd
{
    Q_OBJECT
public:
    UserStockWnd(StockListItem* p,NetThreadPool* netThreadPool,data::DataLoader* pDataLoader,QWidget *parent = nullptr);

    // virtual bool reloadQss(const char* theme);
protected:
    virtual void loadUi();
    virtual void connectSignal();
protected:
    //StockListItem* pStockItem;
protected slots:
    void onBtnPercentChanged(bool);
    void onCellActivated(int row,int column);

    void onDayChartChanged(chart::StockDayChart*,int start,int count);
protected:
    QWidget* calcPage1;
    QTableWidget* tableWidgetResult;
    QGridLayout* calcPage1Layout;
    QPushButton* btnPercentChanged;
};

#endif // USERSTOCKWND_H
