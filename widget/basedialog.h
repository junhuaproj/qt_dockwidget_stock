/*
 * Dialog基类，为方便动态加载qss
*/

#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include "widget/ithemewidget.h"

class QDialogButtonBox;
class BaseDialog : public QDialog,public IThemeWidget
{
public:
    BaseDialog(QWidget *parent = nullptr);
    virtual void getThemingWidget(QList<QWidget*>& wids)const;
    virtual const char* qssName()const;
    virtual QWidget* getWidget();
    void getDlgBoxButtons(QList<QWidget*>& wids,const QDialogButtonBox* box)const;
};

#endif // BASEDIALOG_H
