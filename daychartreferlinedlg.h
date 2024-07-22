/*
 * 参考线设置
*/
#ifndef DAYCHARTREFERLINEDLG_H
#define DAYCHARTREFERLINEDLG_H

#include <QDialog>
#include "widget/basedialog.h"

namespace Ui {
class DayChartReferLineDlg;
}

#include "cfgchart.h"
namespace chart
{
class DayChartReferLineDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit DayChartReferLineDlg(int colIndex,float value,DayChartType viewIndex,
                                  PChartViewCfg cfgs,int count,QWidget *parent = nullptr);
    ~DayChartReferLineDlg();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected slots:
    void onViewChanged(int index);
    void onAddClick(bool);
    void onClearClick(bool);
    void onColorClick(bool);
    void onDeleteClick(bool);
    void onCurrCellChanged(int,int,int,int);
    void onHorizonChecked(bool);
    void onVerticalChecked(bool);
    void onSaveClick(bool);
protected:
    PReferLine curLine;
    PChartViewCfg pCfgs;
    const int count;

    int colIndex;
    float value;
    DayChartType viewIndex;
private:
    Ui::DayChartReferLineDlg *ui;
signals:
    void referUpdated(DayChartType chartType);
};
}
#endif // DAYCHARTREFERLINEDLG_H
