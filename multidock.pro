QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += sql network

QT      += charts
greaterThan(QT_MAJOR_VERSION, 5): QT +=  core5compat

CONFIG += c++17


INCLUDEPATH += C:\Program Files\OpenSSL-Win64\include

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


DEFINES += DB_MYSQL=1

SOURCES += \
    Algorithm.cpp \
    AppConfigqss.cpp \
    RecordWidget.cpp \
    appconfig.cpp \
    bbandsindex.cpp \
    bg/bgrunnable.cpp \
    bg/managerrunnable.cpp \
    bg/netthreadpool.cpp \
    bg/netthreadrequest.cpp \
    bg/todayminuterunnable.cpp \
    bg/todaypricerunnable.cpp \
    biasstatisticindex.cpp \
    biasstatisticindexcfg.cpp \
    blacklistdlg.cpp \
    cfgchart.cpp \
    chartstatusdockwidget.cpp \
    chartthemedlg.cpp \
    chartwidget.cpp \
    config/httpaddressitem.cpp \
    data/parserjsondata.cpp \
    data/parserrecordlist.cpp \
    data/stockdayminute.cpp \
    data/stockprop.cpp \
    dataloader.cpp \
    dataloaderlocalsohu.cpp \
    dataloadernetsohu.cpp \
    daychartcfgdlg.cpp \
    daychartreferlinedlg.cpp \
    daystockchartcfg.cpp \
    dmiindex.cpp \
    dockwidgetdlg.cpp \
    filterdata.cpp \
    filterstockbypercentdlg.cpp \
    kdjindex.cpp \
    kdjindexcfg.cpp \
    macdindex.cpp \
    main.cpp \
    mainwindowmdi.cpp \
    minutechartdockwidget.cpp \
    mysqldata/myblackliststock.cpp \
    mysqldata/mydata.cpp \
    mysqldata/mydb.cpp \
    mysqldata/mystockindex.cpp \
    mysqldata/mystocklistitem.cpp \
    mysqldata/mystockprop.cpp \
    mysqldata/mystockrecord.cpp \
    mysqldata/mystockrecordlist.cpp \
    net/netbaserequest.cpp \
    netrequest.cpp \
    qtablecombobox.cpp \
    selectcomparedlg.cpp \
    simulatetrade.cpp \
    simulatetradedockwidget.cpp \
    simulatetradeeditdlg.cpp \
    simulatetradeloader.cpp \
    sohudataparser.cpp \
    stockbasedata.cpp \
    stockchartdraw.cpp \
    stockchartlabel.cpp \
    stockchartview.cpp \
    stockcurrinfo.cpp \
    stockday.cpp \
    stockdaychart.cpp \
    stockfilterdlg.cpp \
    stockinfodockwidget.cpp \
    stockinformation.cpp \
    stocklaststatus.cpp \
    stocklistitem.cpp \
    stockmacdcfg.cpp \
    stockminutechart.cpp \
    stocknotedlg.cpp \
    stockpchglevelwidget.cpp \
    stockpricechgwidget.cpp \
    stockpriceitem.cpp \
    stockrecorddlg.cpp \
    stockrecorditem.cpp \
    stockrecordlist.cpp \
    stockrecordlistdlg.cpp \
    stocksearchwidget.cpp \
    stocksearchwidget2.cpp \
    tablewidgetitemint.cpp \
    tdxminutedata.cpp \
    todayminutecfgdlg.cpp \
    todayminutedataloadersohu.cpp \
    todayminutewnd.cpp \
    usermainwnd.cpp \
    userstartwnd.cpp \
    userstockcomparewnd.cpp \
    userstockwnd.cpp \
    valuelevelitem.cpp \
    volumeindex.cpp \
    widget/basedialog.cpp \
    widget/basedockwidget.cpp \
    widget/basemainwindow.cpp \
    widget/basemdiarea.cpp \
    widget/basewidget.cpp

HEADERS += \
    Algorithm.h \
    AppConfigqss.h \
    RecordWidget.h \
    StatisticIndex.h \
    appconfig.h \
    bbandsindex.h \
    bg/bgrunnable.h \
    bg/managerrunnable.h \
    bg/netthreadpool.h \
    bg/netthreadrequest.h \
    bg/todayminuterunnable.h \
    bg/todaypricerunnable.h \
    biasstatisticindex.h \
    biasstatisticindexcfg.h \
    blacklistdlg.h \
    cfgchart.h \
    chartstatusdockwidget.h \
    chartthemedlg.h \
    chartwidget.h \
    config/httpaddressitem.h \
    data/TableStockMember.h \
    data/parserjsondata.h \
    data/parserrecordlist.h \
    data/stockdayminute.h \
    data/stockprop.h \
    dataloader.h \
    dataloaderlocalsohu.h \
    dataloadernetsohu.h \
    daychartcfgdlg.h \
    daychartreferlinedlg.h \
    daystockchartcfg.h \
    dmiindex.h \
    dockwidgetdlg.h \
    filterdata.h \
    filterstockbypercentdlg.h \
    idataloader.h \
    idataloaderminute.h \
    kdjindex.h \
    kdjindexcfg.h \
    macdindex.h \
    mainwindowmdi.h \
    minutechartdockwidget.h \
    mysqldata/mybind_value.h \
    mysqldata/myblackliststock.h \
    mysqldata/mydata.h \
    mysqldata/mydb.h \
    mysqldata/mysql_data_def.h \
    mysqldata/mystockindex.h \
    mysqldata/mystocklistitem.h \
    mysqldata/mystockprop.h \
    mysqldata/mystockrecord.h \
    mysqldata/mystockrecordlist.h \
    net/netbaserequest.h \
    netrequest.h \
    qtablecombobox.h \
    selectcomparedlg.h \
    simulatetrade.h \
    simulatetradedockwidget.h \
    simulatetradeeditdlg.h \
    simulatetradeloader.h \
    sohudataparser.h \
    stockbasedata.h \
    stockchartdraw.h \
    stockchartlabel.h \
    stockchartview.h \
    stockcurrinfo.h \
    stockday.h \
    stockdaychart.h \
    stockdaydata.h \
    stockfilterdlg.h \
    stockinfodockwidget.h \
    stockinformation.h \
    stocklaststatus.h \
    stocklistitem.h \
    stockmacdcfg.h \
    stockminutechart.h \
    stocknotedlg.h \
    stockpchglevelwidget.h \
    stockpricechgwidget.h \
    stockpriceitem.h \
    stockrecorddlg.h \
    stockrecorditem.h \
    stockrecordlist.h \
    stockrecordlistdlg.h \
    stocksearchwidget.h \
    tablewidgetitemint.h \
    tdxminutedata.h \
    todayminutecfgdlg.h \
    todayminutedataloadersohu.h \
    todayminutewnd.h \
    usermainwnd.h \
    userstartwnd.h \
    userstockcomparewnd.h \
    userstockwnd.h \
    valuelevelitem.h \
    volumeindex.h \
    widget/basedialog.h \
    widget/basedockwidget.h \
    widget/basemainwindow.h \
    widget/basemdiarea.h \
    widget/basewidget.h \
    widget/ithemewidget.h

FORMS += \
    blacklistdlg.ui \
    chartthemedlg.ui \
    daychartcfgdlg.ui \
    daychartreferlinedlg.ui \
    dockwidgetdlg.ui \
    filterstockbypercentdlg.ui \
    mainwindowmdi.ui \
    selectcomparedlg.ui \
    simulatetradeeditdlg.ui \
    stockfilterdlg.ui \
    stocknotedlg.ui \
    stockrecorddlg.ui \
    stockrecordlistdlg.ui \
    todayminutewndcfg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    default.qrc

TRANSLATIONS = lang_cn.ts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../ta-lib/c/lib/ -lta_func_cdr -lta_abstract_cdr -lta_common_cdr -lta_libc_cdr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../ta-lib/c/lib/x64 -lta_func_cdd  -lta_abstract_cdd -lta_common_cdd -lta_libc_cdd

INCLUDEPATH += $$PWD/../../../ta-lib/c/include
DEPENDPATH += $$PWD/../../../ta-lib/c/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../sqlite3forqt/x64/release/ -lsqlite3forqt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../sqlite3forqt/x64/debug/ -lsqlite3forqt

INCLUDEPATH += $$PWD/../sqlite3forqt
DEPENDPATH += $$PWD/../sqlite3forqt


INCLUDEPATH += $$PWD/../mysql-8.4.0-winx64/include
DEPENDPATH += $$PWD/../mysql-8.4.0-winx64/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../mysql-8.4.0-winx64/lib/ -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../mysql-8.4.0-winx64/lib/ -llibmysql
