#include "basemainwindow.h"

#include <QTabBar>

BaseMainWindow::BaseMainWindow(NetThreadPool* net,
                               data::DataLoader* dataLoader,QWidget *parent)
    : QMainWindow(parent),pNetPool(net),pDataLoader(dataLoader)
{

}

//得到所有MainWindowTabBar
//这些TabBar是多个Dock组合后，自动建立的
void BaseMainWindow::getMainWindowTabBar(QList<QWidget*>& tabBars)const
{
    QList<QTabBar*> bar=this->findChildren<QTabBar*>(Qt::FindDirectChildrenOnly);
    QTabBar* cur;
    for(int i=0;i<bar.size();i++)
    {
        cur=bar.at(i);
        if(strcmp("QMainWindowTabBar",cur->metaObject()->className())==0)
            tabBars.push_back(cur);
        //qDebug()<<bar.at(i)->metaObject()->className();
    }
}
