/*
 * 设置绘图参数
*/
#ifndef CHARTTHEMEDLG_H
#define CHARTTHEMEDLG_H

#include <QDialog>
#include "cfgchart.h"
#include "StatisticIndex.h"
namespace Ui {
class ChartThemeDlg;
}


namespace chart {
class ChartThemeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChartThemeDlg(QList<StatisticIndexCfg*>& cfgs,QWidget *parent = nullptr);
    ~ChartThemeDlg();

    //const DayChartParam* getParam()const;
protected:
    void setTableRow(int row,IndexCfgValue* cfg);

protected:
    //DayChartParam themes;
    //ChartThemeType curType;
    StatisticIndexCfg* pCurCfg;
protected slots:
    void onCellEdit(int,int);
    void onCellChanged(int,int);
    void onChartTypeChanged(int index);

    void onAccepted();
private:
    Ui::ChartThemeDlg *ui;
};
}
#endif // CHARTTHEMEDLG_H
