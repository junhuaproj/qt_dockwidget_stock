/*
 * 黑名单UI
*/
#ifndef BLACKLISTDLG_H
#define BLACKLISTDLG_H

//#include <QDialog>
#include "widget/basedialog.h"

namespace Ui {
class BlackListDlg;
}

class BlackListDlg : public BaseDialog
{
    Q_OBJECT

public:
    explicit BlackListDlg(QWidget *parent = nullptr);
    ~BlackListDlg();

    void getOutDate(QDate& date);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
private:
    Ui::BlackListDlg *ui;
};

#endif // BLACKLISTDLG_H
