#include "basedockwidget.h"

BaseDockWidget::BaseDockWidget(const QString &title, QWidget *parent,
                               Qt::WindowFlags flags)
    :QDockWidget(title,parent,flags)
{

}

void    BaseDockWidget::getThemingWidget(QList<QWidget*>& wids)const
{

}
