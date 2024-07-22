#include "AppConfigqss.h"
//#include <QDir>

#include <QFile>
#include <QWidget>

AppConfigQSS::AppConfigQSS(const QString& root,QObject *parent)
    : QObject(parent),qssRoot(root)
{

}
AppConfigQSS::~AppConfigQSS()
{
    //delete translator;
}

void AppConfigQSS::updateQss(IThemeWidget* itheme)const
{
    QList<QWidget*> wids;
    const char* name=itheme->qssName();
    if(name)
        loadQSS(itheme->getWidget(),name);
    itheme->getThemingWidget(wids);
    updateQss(wids);
}

void AppConfigQSS::updateQss(QList<QWidget*>& wids)const
{
    QWidget* cur;
    QList<QWidget*> curWids;
    const char* curClass;

    while(!wids.isEmpty())
    {
        //找到同类的
        cur=wids.at(0);
        curWids.push_back(cur);
        curClass=cur->metaObject()->className();
        for(int i=1;i<wids.size();i++)
        {
            if(strcmp(wids.at(i)->metaObject()->className(),curClass)==0)
            {
                curWids.push_back(wids.at(i));
            }
        }
        loadQSSt(curWids,curClass);
        //更新后清除
        while(!curWids.isEmpty())
        {
            wids.removeOne(curWids.back());
            curWids.pop_back();
        }
    }
}

bool AppConfigQSS::loadQSS(QWidget* win,const QString& qssid)const
{
    QByteArray arr;
    if(loadQss(arr,qssid))
    {
        win->setStyleSheet(QString(arr));
        return true;
    }
    return false;
}
bool AppConfigQSS::loadQss(QByteArray& data,const QString& qssid)const
{
    //QFile f(dataroot+"\\qss\\"+qssid+".qss");
    QFile f(qssRoot+"\\"+qssid+".qss");
    if(f.open(QFile::ReadOnly))
    {
        data=f.readAll();
        f.close();

        return true;
    }
    return false;
}

