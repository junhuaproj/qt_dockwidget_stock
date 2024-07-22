/*
 * 分时线设置
*/
#ifndef DAYCHARTCFGDLG_H
#define DAYCHARTCFGDLG_H

#include <QDialog>
#include "cfgchart.h"
#include "StatisticIndex.h"
#include <QComboBox>
namespace Ui {
class DayChartCfgDlg;
}
namespace chart {
class DayChartCfgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DayChartCfgDlg(const QList<StatisticIndex*> *indexes,PChartViewCfg cfg,int cfgLen,QWidget *parent = nullptr);
    ~DayChartCfgDlg();

protected slots:
    void onAccepted();

protected:

    void initCbxType(QComboBox* pBox,StatisticIndex* pSelected);

    const QList<StatisticIndex*> *indexes;
    PChartViewCfg cfg;
    int cfgLen;
private:

    Ui::DayChartCfgDlg *ui;
};
}
#endif // DAYCHARTCFGDLG_H
