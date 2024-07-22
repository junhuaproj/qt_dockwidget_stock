#ifndef CFGCHARTDLG_H
#define CFGCHARTDLG_H

#include <QDialog>
#include "cfgchart.h"
namespace Ui {
class CfgChartDlg;
}

class CfgChartDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CfgChartDlg(PCfgChartAll pCfg,QWidget *parent = nullptr);
    ~CfgChartDlg();

protected:
    PCfgChartAll pCfgAll;

    void updateCfg();
    void setCfgToUi();
protected slots:
    void onButtonTest();
    void onUpRadioVol(bool);
    void onUpRadioMacd2(bool);
    void onUpRadioHide(bool);

    void onDownRadioVol(bool);
    void onDownRadioMacd2(bool);
    void onDownRadioHide(bool);
signals:
    void cfgChanged(PCfgChartAll pCfg);
private:
    Ui::CfgChartDlg *ui;
};

#endif // CFGCHARTDLG_H
