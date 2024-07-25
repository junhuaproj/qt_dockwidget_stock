/**
 * 全局的配置，包括qss等
 */
#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QObject>

#define TABLE_DATA_STOCK     Qt::UserRole+1
#include "data/TableStockMember.h"

enum FilterCompareType{
    FilterCompareType_LessThan=-1,//小于
    FilterCompareType_Equivalent=0,//相等
    FilterCompareType_GreatThan=1,//大于
    FilterCompareType_AbsLessThan=2,//绝对值小于
    FilterCompareType_AbsGreatThan=3,//绝对值大于
};

#include <QTranslator>
#include "net/httpaddressitem.h"
#include "widget/ithemewidget.h"
class QJsonArray;
class AppConfigQSS;
class AppConfig : public QObject
{
    Q_OBJECT
public:
    explicit AppConfig(QObject *parent = nullptr);
    virtual ~AppConfig();

    const QString& cfgPath()const;
    bool readMysqlCfg(QString& user,QString& pw,QString& server,QString& db);
    void getCfgTodayStock(QString& path)const;
    bool doTranslator(const QString& file);
    inline const AppConfigQSS* qss(){return cfgQss;}

    float* getPriceLevel(int& count);
    void getSimulateTradePath(const QString& code,QString& path)const;
    const QList<HttpAddressItem*>* getTradeAddress()const;
    void updateQss(IThemeWidget* itheme)const;

    void getTableStockColumnNames(const TableStockMember* columns,int col_count,QStringList& titles);
    const QString& getStockRoot()const;

    inline const QString& getDateFormat(){
        return dateFormat;
    }
    inline const QString& getDateTimeFormat()
    {
        return datetimeFormat;
    }
    const QString& getChartFont()const;
    inline const QString& getTdxPath()const
    {
        return stockTdx;
    }
protected:
    bool loadCfg();
//涨跌幅分级
    int priceLevelCount;
    float* priceLevelRange;

    QList<HttpAddressItem*> tradeAddress;

    AppConfigQSS* cfgQss;
    QTranslator  *translator;

    QString qssRoot;//qss文件目录
    QString cfgRoot;//其他配置文件
    QString dataRoot;//应用程序目录
    QString stockRoot;//数据
    QString stockTdx;//通达信数据目录

    QString dateFormat;//日期格式
    QString datetimeFormat;//日期时间格式
    QString chartFont;//图表字体
signals:
};
/*
 * 日实时图中的配置
 * 同时显示的行数、列数
 * 更新间隔
 * */
class TodayMinuteCfg
{
public:
    TodayMinuteCfg();
    virtual ~TodayMinuteCfg();

    int getRows() const;
    void setRows(int newRows);

    int getColumns() const;
    void setColumns(int newColumns);

    void loadDefault();
    int getUpdateTimeout() const;
    void setUpdateTimeout(int newUpdateTimeout);

protected:
    int updateTimeout;
    int rows;
    int columns;
};
//全局的配置
extern AppConfig appConfig;
//extern const char* datetime_format;
//extern const char* date_format;
//void changeWidgetTheme(QWidget* widget,const char* theme,int level);
#endif // APPCONFIG_H
