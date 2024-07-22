#include "appconfig.h"
#include <QWidget>
//#include "IMainWidgetQSS.h"
#include "AppConfigqss.h"
#include <QDir>
#include <QApplication>
#include "data/parserjsondata.h"
const char* datetime_format="yyyy-MM-dd hh:mm:ss";
const char* date_format="yyyy-MM-dd";

AppConfig appConfig;

AppConfig::AppConfig(QObject *parent)
    : QObject(parent),cfgQss(NULL)
{
    stockRoot="f:\\stock";
    dataRoot=QDir::currentPath();
    dataRoot=dataRoot.replace("/","\\");
    dataRoot=dataRoot.replace("\\debug","");

    qssRoot=dataRoot+"\\qss";
    cfgRoot=dataRoot+"\\config";
    cfgQss=new AppConfigQSS(qssRoot);
    translator=new QTranslator();


    priceLevelCount=10;
    priceLevelRange=(float*)malloc(sizeof(float)*priceLevelCount);
    for(int i=0;i<priceLevelCount;i++)
        priceLevelRange[i]=i;

    tradeAddress.push_back(new HttpAddressItem(0,tr("kdj"),tr("/index/kdj/testtrade/%1/0.0/100.0")));
    tradeAddress.push_back(new HttpAddressItem(0,tr("macd"),tr("/index/macd/testtrade/%1")));
}
AppConfig::~AppConfig()
{
    delete translator;
    delete cfgQss;
}
bool AppConfig::readMysqlCfg(QString& user,QString& pw,QString& server,QString& db)
{
    QString path=QString("%1\\config\\dbcfg.json").arg(dataRoot);
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        QByteArray data=f.readAll();
        f.close();
        QJsonDocument doc=QJsonDocument::fromJson(data);
        if(!doc.isObject())
        {
            return false;
        }
        QJsonObject obj=doc.object();
        user=obj.value("user").toString();
        pw=obj.value("pw").toString();
        server=obj.value("server").toString();
        db=obj.value("db").toString();
        return true;
    }
    return false;

}

const QList<HttpAddressItem*>* AppConfig::getTradeAddress()const
{
    return &tradeAddress;
}
void AppConfig::getSimulateTradePath(const QString& code,QString& path)const
{
    path=QString("%1\\%2\\%3.json").arg(stockRoot,"simulatetrade",code);
}

float* AppConfig::getPriceLevel(int& count)
{
    count=priceLevelCount;
    return priceLevelRange;
}
const QString& AppConfig::cfgPath()const
{
    return cfgRoot;
}

void AppConfig::getCfgTodayStock(QString& path)const
{
    path= cfgRoot+"\\todaywndcfg.json";
}

bool AppConfig::doTranslator(const QString& file)
{
    QString path=dataRoot+"\\"+file;
    bool bok=translator->load(path);
    if(bok)
    {
        return QApplication::installTranslator(translator);
    }
    return false;
}

void AppConfig::updateQss(IThemeWidget* itheme)const
{
    cfgQss->updateQss(itheme);
}


void AppConfig::getTableStockColumnNames(const TableStockMember* columns,int col_count,QStringList& titles)
{
    ParserJsonData parser;
    QString path=QString("%1\\TableStockMember.json").arg(cfgRoot);
    QStringList names;
    parser.parserTableStockProp(path,names);
    for(int i=0;i<col_count;i++)
    {
        titles.push_back(names.at(columns[i]));
    }
}

TodayMinuteCfg::TodayMinuteCfg()
{
    loadDefault();
}
TodayMinuteCfg::~TodayMinuteCfg()
{

}
int TodayMinuteCfg::getRows() const
{
    return rows;
}

void TodayMinuteCfg::setRows(int newRows)
{
    rows = newRows;
}

int TodayMinuteCfg::getColumns() const
{
    return columns;
}

void TodayMinuteCfg::setColumns(int newColumns)
{
    columns = newColumns;
}
void TodayMinuteCfg::loadDefault()
{
    rows=2;
    columns=2;
    updateTimeout=2;
}

int TodayMinuteCfg::getUpdateTimeout() const
{
    return updateTimeout;
}

void TodayMinuteCfg::setUpdateTimeout(int newUpdateTimeout)
{
    updateTimeout = newUpdateTimeout;
}
#if 0
void changeWidgetTheme(QWidget* widget,const char* theme,int level)
{
#define NO_QSS_LEN  13
    const char noqss[NO_QSS_LEN][24]={"QScrollBar","QHeaderView","QToolButton","QCheckBox","QRadioButton",
                                        "QScrollArea","QToolBoxButton","QDockWidgetTitleButton","QLabel","QSplitterHandle",
                                        "QLineEdit","QScrollBar","QToolButton"};
    QObjectList objs= widget->children();
    IMainWidgetQSS* qsswidget;
    QWidget* wid;
    const AppConfigQSS* qss=appConfig.qss();
    for(auto obj:objs)
    {
        if(!obj->isWidgetType())
            continue;

        const QMetaObject* meta= obj->metaObject();

        qDebug()<<meta->className();
        wid=(QWidget*)obj;
        for(int i=0;i<NO_QSS_LEN;i++)
        {
            if(strcmp(meta->className(),noqss[i])==0)
                goto loopfinish;
//                goto next;
        }
        qsswidget=dynamic_cast<IMainWidgetQSS*>(wid);
        if(qsswidget)
        {
            //qDebug()<<"qss theme:"<<meta->className();
            qss->loadQSS(wid,qsswidget->qssName());
            qsswidget->reloadQss(theme);
            continue;
        }
        else if(strcmp(meta->className(),"QWidget")==0||!qss->loadQSS(wid,meta->className()))
        {
            qDebug()<<"can't find theme:"<<meta->className();
        }
        //else continue;
    next:
        if(level<5)
            changeWidgetTheme(wid,theme,level+1);
    loopfinish:
        ;
    }
}
#endif
