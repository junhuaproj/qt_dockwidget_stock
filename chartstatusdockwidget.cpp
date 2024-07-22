#include "chartstatusdockwidget.h"

ChartStatusDockWidget::ChartStatusDockWidget(const QString &title,
                                             QWidget *parent,
                                             Qt::WindowFlags flags)
    : BaseDockWidget(title,parent,flags)
{
    textEdit=new QPlainTextEdit(this);
    this->setWidget(textEdit);
}

ChartStatusDockWidget::~ChartStatusDockWidget()
{

}
void ChartStatusDockWidget::setDayChartSelectRange(const QString& info)
{
    dayRangeinfo=info;
    updateChartInfo();
}
void ChartStatusDockWidget::setDayChartSelectItem(const QString& info)
{
    dayItemInfo=info;
    updateChartInfo();
}
void ChartStatusDockWidget::setMinuteChartSelectRange(const QString& info)
{
    minuteRangeInfo=info;
    updateChartInfo();
}
void ChartStatusDockWidget::setMinuteChartSelectItem(const QString& info)
{
    minuteItem=info;
    updateChartInfo();
}
void ChartStatusDockWidget::updateChartInfo()
{
    textEdit->clear();
    textEdit->appendHtml(dayRangeinfo);
    textEdit->appendHtml("<br />");
    textEdit->appendHtml(dayItemInfo);
    textEdit->appendHtml("<br />");
    textEdit->appendHtml(minuteRangeInfo);
    textEdit->appendHtml("<br />");
    textEdit->appendHtml(minuteItem);
    textEdit->appendHtml("<br />");
}
void ChartStatusDockWidget::getThemingWidget(QList<QWidget*>& wids)
{
    wids.push_back(textEdit);
}
