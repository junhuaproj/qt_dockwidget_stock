/*为动态加载qss
*/
#ifndef BASEMAINWINDOW_H
#define BASEMAINWINDOW_H

#include <QMainWindow>

#include <dataloader.h>
//#include "IMainWidgetQSS.h"
#include "ithemewidget.h"
#include "bg/netthreadpool.h"

class BaseMainWindow : public QMainWindow,public IThemeWidget
{
    Q_OBJECT
public:
    explicit BaseMainWindow(NetThreadPool* netThreadPool,
                            data::DataLoader* pDataLoader,
                            QWidget *parent = nullptr);

    //得到所有dock
    virtual void getDockWidgets(QList<QDockWidget*>& docks)=0;
    //virtual void getThemingWidget(QList<QWidget*>& wids);

    void getMainWindowTabBar(QList<QWidget*>& tabBars)const;
protected:
    data::DataLoader* pDataLoader;
    NetThreadPool* pNetPool;
signals:
    void currentMessage(const QString& msg);
};

#endif // BASEMAINWINDOW_H
