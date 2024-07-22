#ifndef TODAYMINUTECFGDLG_H
#define TODAYMINUTECFGDLG_H

#include <QDialog>

#include "appconfig.h"
#include "widget/basedialog.h"

namespace Ui {
class TodayMinuteCfg;
}

class TodayMinuteCfgDlg : public BaseDialog
{
    Q_OBJECT
public:
    explicit TodayMinuteCfgDlg(TodayMinuteCfg* pCfg,QWidget *parent = nullptr);

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected slots:
    void onCfgAccept();
protected:
    TodayMinuteCfg* pMinuteCfg;
    Ui::TodayMinuteCfg* ui;
signals:
};

#endif // TODAYMINUTECFGDLG_H
