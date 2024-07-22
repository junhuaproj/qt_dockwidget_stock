/*
 * 模拟交易详情
*/
#ifndef SIMULATETRADEEDITDLG_H
#define SIMULATETRADEEDITDLG_H

#include <QDialog>
#include "simulatetrade.h"
namespace Ui {
class SimulateTradeEditDlg;
}
namespace simulate{
class TradeEditDlg : public QDialog
{
    Q_OBJECT

public:
    explicit TradeEditDlg(QWidget *parent = nullptr);
    ~TradeEditDlg();

    void setTrade(int row,SimulateTrade* trade);
protected:
    int row;
    SimulateTrade* trade;
private:
    Ui::SimulateTradeEditDlg *ui;

protected slots:
    void onBtnDelete(bool);
    void onBtnSave(bool);
    void onBtnClose(bool);
    void onPriceChanged(double);
signals:
    void tradeDelete(TradeEditDlg*,int row,SimulateTrade* trade);
    void tradeSave(TradeEditDlg*,int row,SimulateTrade* trade);
};
}
#endif // SIMULATETRADEEDITDLG_H
