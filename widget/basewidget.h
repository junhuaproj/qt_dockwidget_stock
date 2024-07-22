/*为动态加载qss
*/
#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QWidget>
#include "ithemewidget.h"

class BaseWidget : public QWidget,public IThemeWidget
{
    Q_OBJECT
public:
    explicit BaseWidget(QWidget *parent = nullptr);
    virtual void getThemingWidget(QList<QWidget*>& wids);
signals:
};

#endif // BASEWIDGET_H
