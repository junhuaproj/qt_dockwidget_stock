/*
 * qss用的接口，用于动态加载QSS，
*/
#ifndef ITHEMEWIDGET_H
#define ITHEMEWIDGET_H

#include <QWidget>
#include <QList>
class IThemeWidget{
public:
    /**
     * 获得需要加载QSS的所有widget,包括QPushButton,QTableWidget等
     * 根据Widget名称自动从qss目录下读取
    */
    virtual void    getThemingWidget(QList<QWidget*>& wids)const=0;
    /*
     * 当前窗口的qss名称
    */
    virtual const char* qssName()const
    {
        return NULL;
    }
    /*
     * 需要应用qss的Widget,主要是指QMainWindow,QDialog的子类
    */
    virtual QWidget* getWidget(){
        return NULL;
    }
};

#endif // ITHEMEWIDGET_H
