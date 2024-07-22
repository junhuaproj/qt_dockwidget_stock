/*Dock基类，为动态加载qss
*/
#ifndef BASEDOCKWIDGET_H
#define BASEDOCKWIDGET_H

#include <QDockWidget>
#include "ithemewidget.h"
class BaseDockWidget : public QDockWidget,public IThemeWidget
{
public:
    BaseDockWidget(const QString &title, QWidget *parent = nullptr,
                   Qt::WindowFlags flags = Qt::WindowFlags());

    virtual void    getThemingWidget(QList<QWidget*>& wids)const;
};

#endif // BASEDOCKWIDGET_H
