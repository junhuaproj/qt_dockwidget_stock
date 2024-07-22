/*
 * qss加载
*/
#ifndef APPCONFIGQSS_H
#define APPCONFIGQSS_H

#include <QObject>

#include <QDockWidget>

#include "appconfig.h"
#include "widget/ithemewidget.h"

class AppConfigQSS : public QObject
{
    Q_OBJECT
public:
    explicit AppConfigQSS(const QString& qssRoot,QObject *parent = nullptr);
    virtual ~AppConfigQSS();

    bool loadQSS(QWidget* win,const QString& qssid)const;
    template<typename T>
    inline bool loadQSSt(QList<T>& wins,const QString& qssid)const
    {
        QByteArray arr;
        if(loadQss(arr,qssid))
        {
            QString qss=QString(arr);
            auto it=wins.begin(),end=wins.end();
            for(;it!=end;it++)
                (*it)->setStyleSheet(qss);
            return true;
        }
        return false;

    }
    void updateQss(IThemeWidget* itheme)const;
    void updateQss(QList<QWidget*>& wids)const;
    //bool loadQSS(QList<QDockWidget*>& wins);

    //bool doTranslator(const QString& file);

    bool loadQss(QByteArray& data,const QString& qssid)const;
protected:
    QString qssRoot;

signals:
};
//extern DataLoaderQSS qss;
#endif // APPCONFIGQSS_H
