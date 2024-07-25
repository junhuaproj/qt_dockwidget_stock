/*
 * 日K线图设置，
 * 最多有3个图，可调整各图显示比例，或不显示
 * 选择各图显示的统计指标
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

    /*
     * 下拉列表中增加可选的指标
    **/
    void initCbxType(QComboBox* pBox,StatisticIndex* pSelected);

    const QList<StatisticIndex*> *indexes;
    PChartViewCfg cfg;
    int cfgLen;
private:

    Ui::DayChartCfgDlg *ui;
};
}
#endif // DAYCHARTCFGDLG_H
