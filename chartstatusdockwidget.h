/*
 * 用于显示图标信息，选中点，区间等
*/
#ifndef CHARTSTATUSDOCKWIDGET_H
#define CHARTSTATUSDOCKWIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QPlainTextEdit>
#include "widget/basedockwidget.h"

class ChartStatusDockWidget : public BaseDockWidget
{
    Q_OBJECT
public:
    explicit ChartStatusDockWidget(const QString &title,
                                   QWidget *parent = nullptr,
                                   Qt::WindowFlags flags = Qt::WindowFlags());
    virtual ~ChartStatusDockWidget();

    void setDayChartSelectRange(const QString& info);
    void setDayChartSelectItem(const QString& info);
    void setMinuteChartSelectRange(const QString& info);
    void setMinuteChartSelectItem(const QString& info);

    void updateChartInfo();
    virtual void    getThemingWidget(QList<QWidget*>& wids);
protected:
    QString dayRangeinfo;
    QString dayItemInfo;
    QString minuteRangeInfo;
    QString minuteItem;
protected:
    QPlainTextEdit* textEdit;
signals:
};

#endif // CHARTSTATUSDOCKWIDGET_H
