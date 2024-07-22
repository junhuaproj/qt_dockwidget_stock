/*
 * qss用
*/
#ifndef ITHEMEWIDGET_H
#define ITHEMEWIDGET_H

#include <QWidget>
#include <QList>
class IThemeWidget{
public:
    virtual void    getThemingWidget(QList<QWidget*>& wids)const=0;
    virtual const char* qssName()const
    {
        return NULL;
    }
    virtual QWidget* getWidget(){
        return NULL;
    }
};

#endif // ITHEMEWIDGET_H
