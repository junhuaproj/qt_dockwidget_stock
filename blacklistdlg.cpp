#include "blacklistdlg.h"
#include "ui_blacklistdlg.h"
#include "appconfig.h"
//#include "AppConfigqss.h"
BlackListDlg::BlackListDlg(QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::BlackListDlg)
{
    ui->setupUi(this);
    //changeWidgetTheme(this,"",3);
    appConfig.updateQss(this);
}

BlackListDlg::~BlackListDlg()
{
    delete ui;
}

void BlackListDlg::getOutDate(QDate& date)
{
    date=QDate::currentDate();
    if(ui->radioCalendar->isChecked())
    {
        date=ui->calendarWidget->selectedDate();
    }
    else if(ui->radioday10->isChecked())
    {
        date=date.addDays(10);
    }
    else if(ui->radioMonth->isChecked())
        date=date.addMonths(1);
    else
        date=date.addYears(1);
}
void BlackListDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(ui->calendarWidget);
    wids.push_back(ui->radioCalendar);
    wids.push_back(ui->radioday10);
    wids.push_back(ui->radioMonth);
    wids.push_back(ui->radioYear);
    getDlgBoxButtons(wids,ui->buttonBox);
}
