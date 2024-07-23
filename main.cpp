//#include "mainwindow.h"
#include "mainwindowmdi.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include "AppConfigqss.h"
#include "appconfig.h"
#include "bg/netthreadpool.h"
//DataLoaderQSS qss;
//QTranslator  *translator=NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qss.doTranslator("lang_cn.qm");
    //指定翻译的语言
    appConfig.doTranslator("lang_cn.qm");

    MainWindowMdi w;
    w.show();
    return a.exec();
}
