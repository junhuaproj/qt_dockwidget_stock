#include "basedialog.h"
#include <QDialogButtonBox>
#include <QAbstractButton>
class QAbstractButton;
BaseDialog::BaseDialog(QWidget *parent)
    :QDialog(parent)
{

}
void BaseDialog::getThemingWidget(QList<QWidget*>& wids)const
{
    //wids.push_back(this);
}
const char* BaseDialog::qssName()const
{
    return this->metaObject()->className();
}
QWidget* BaseDialog::getWidget()
{
    return this;
}
void BaseDialog::getDlgBoxButtons(QList<QWidget*>& wids,const QDialogButtonBox* box)const
{
    QList<QAbstractButton*> btns=box->buttons();
    QList<QAbstractButton*>::const_iterator it=btns.constBegin(),end=btns.constEnd();
    for(;it!=end;it++)
        wids.append(*it);
}
