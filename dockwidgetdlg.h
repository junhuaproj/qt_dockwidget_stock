/**
 * Dock 显示、隐藏设置
*/
#ifndef DOCKWIDGETDLG_H
#define DOCKWIDGETDLG_H

//#include <QDialog>
#include "widget/basedialog.h"
namespace Ui {
class DockWidgetDlg;
}

#include <QList>
#include <QDockWidget>

class QTableComboBox;
class UserMainWnd;
class DockWidgetDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit DockWidgetDlg(QList<QDockWidget*>& docks,UserMainWnd* pUserWnd,QWidget *parent = nullptr);
    ~DockWidgetDlg();

    virtual void getThemingWidget(QList<QWidget*>& wids)const;
protected slots:
    void onCellChanged(int,int);
    void onCellDoubleClick(int,int);
    void onCurrentDockChanged(int);
    void onTableCbxKillFocus(QTableComboBox*);
private:
    QTableComboBox* oldCbx;
    UserMainWnd* pUserWnd;
    Ui::DockWidgetDlg *ui;
};

#endif // DOCKWIDGETDLG_H
