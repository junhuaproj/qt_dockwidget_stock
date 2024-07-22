#include "daychartreferlinedlg.h"
#include "ui_daychartreferlinedlg.h"
#include "StatisticIndex.h"
#include "cfgchart.h"
#include <QColorDialog>
#include <QColor>
#include "appconfig.h"
#include "AppConfigqss.h"
namespace chart
{
#define TABLE_DATA_REFER_LINE   (Qt::UserRole+3)
DayChartReferLineDlg::DayChartReferLineDlg(int col,float yvalue,DayChartType view,PChartViewCfg cfgs,int c,QWidget *parent)
    : BaseDialog(parent)
    , ui(new Ui::DayChartReferLineDlg)
    ,pCfgs(cfgs),count(c)
    ,curLine(NULL),colIndex(col),value(yvalue),viewIndex(view)
{
    ui->setupUi(this);
    QString text;
    for(int i=0;i<count;i++)
    {
        text=QString(tr("view %1->")).arg(i+1);
        if(pCfgs[i].pIndex)
            text.append(pCfgs[i].pIndex->getTitle());
        ui->comboBoxView->addItem(text,QVariant((uint64_t)(&pCfgs[i].lines)));
    }

    ui->comboBoxView->setCurrentIndex(0);

    connect(ui->pushButtonAdd,SIGNAL(clicked(bool)),this,SLOT(onAddClick(bool)));
    connect(ui->pushButtonClear,SIGNAL(clicked(bool)),this,SLOT(onClearClick(bool)));
    connect(ui->pushButtonColor,SIGNAL(clicked(bool)),this,SLOT(onColorClick(bool)));
    connect(ui->pushButtonDelete,SIGNAL(clicked(bool)),this,SLOT(onDeleteClick(bool)));
    connect(ui->radioButtonHorizonal,SIGNAL(clicked(bool)),this,SLOT(onHorizonChecked(bool)));
    connect(ui->radioButtonVertical,SIGNAL(clicked(bool)),this,SLOT(onVerticalChecked(bool)));
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),
            this,SLOT(onCurrCellChanged(int,int,int,int)));
    connect(ui->comboBoxView,SIGNAL(currentIndexChanged(int)),this,SLOT(onViewChanged(int)));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(onSaveClick(bool)));
    if(viewIndex<count)
    {
        ui->comboBoxView->setCurrentIndex(viewIndex);
        //onViewChanged(viewIndex);
        onAddClick(true);
        ui->spinBoxIndex->setValue(colIndex);
        ui->spinBoxValue->setValue(value);
    }
    else
        onViewChanged(0);
    appConfig.qss()->updateQss(this);
}

DayChartReferLineDlg::~DayChartReferLineDlg()
{
    delete ui;
}

void DayChartReferLineDlg::getThemingWidget(QList<QWidget*>& wids)const
{
    BaseDialog::getThemingWidget(wids);
    wids.push_back(ui->tableWidget);
    wids.push_back(ui->comboBoxView);
    wids.push_back(ui->lineEditColor);
    wids.push_back(ui->pushButtonAdd);
    wids.push_back(ui->pushButtonClear);
    wids.push_back(ui->pushButtonColor);
    wids.push_back(ui->pushButtonDelete);
    wids.push_back(ui->pushButtonSave);
    getDlgBoxButtons(wids,ui->buttonBox);
}

void DayChartReferLineDlg::onSaveClick(bool)
{
    int index=ui->comboBoxView->currentIndex();
    if(index<0)return;
    if(curLine==NULL)
    {
        ui->tableWidget->setRowCount(0);
        ReferLines* lines=(ReferLines*)qvariant_cast<uint64_t>(ui->comboBoxView->itemData(index));
        curLine=getNewReferLine(lines);
    }
    curLine->color=ui->lineEditColor->text().toUInt(nullptr,16);
    curLine->lineWidth=ui->spinBoxWidth->value();
    if(ui->radioButtonHorizonal->isChecked())
    {
        curLine->flag=ReferLine_Horizontal;
        curLine->value=ui->spinBoxValue->value();
    }
    else
    {
        curLine->index=ui->spinBoxIndex->value();
        curLine->flag=ReferLine_Vertical;
    }
    onViewChanged(ui->comboBoxView->currentIndex());
    emit referUpdated((DayChartType)index);
}

void DayChartReferLineDlg::onHorizonChecked(bool bCheck)
{
    ui->spinBoxValue->setVisible(true);
    ui->spinBoxIndex->setVisible(false);
}

void DayChartReferLineDlg::onVerticalChecked(bool)
{
    ui->spinBoxValue->setVisible(false);
    ui->spinBoxIndex->setVisible(true);
}
void DayChartReferLineDlg::onCurrCellChanged(int currentRow,int,int,int)
{
    QTableWidgetItem* pCur=ui->tableWidget->item(currentRow,1);
    if(pCur==NULL)return ;
    QVariant var=pCur->data(TABLE_DATA_REFER_LINE);
    curLine=(PReferLine)qvariant_cast<uint64_t>(var);

    ui->lineEditColor->setText(QString(tr("%1")).arg(curLine->color,8,16));

    ui->spinBoxWidth->setValue(curLine->lineWidth);
    if(curLine->flag&ReferLine_Horizontal)
    {
        ui->radioButtonHorizonal->setChecked(true);
        ui->spinBoxValue->setValue(curLine->value);
        onHorizonChecked(true);
    }
    else if(curLine->flag&ReferLine_Vertical)
    {
        ui->radioButtonVertical->setChecked(true);
        ui->spinBoxIndex->setValue(curLine->index);
        onVerticalChecked(true);
    }
}

void DayChartReferLineDlg::onAddClick(bool)
{
    ui->lineEditColor->setText(tr("7F7F7F"));
    if(ui->radioButtonHorizonal->isChecked())
    {
        onHorizonChecked(true);
    }
    else
    {
        onVerticalChecked(false);
    }
    ui->spinBoxIndex->setValue(1);
    ui->spinBoxValue->setValue(0);
    ui->spinBoxWidth->setValue(0.9);
    curLine=NULL;
}

void DayChartReferLineDlg::onClearClick(bool)
{
    int index=ui->comboBoxView->currentIndex();
    if(index<0)return;
    ui->tableWidget->setRowCount(0);
    ReferLines* lines=(ReferLines*)qvariant_cast<uint64_t>(ui->comboBoxView->itemData(index));
    if(lines->lines)
    {
        free(lines->lines);
        memset(lines,0,sizeof(ReferLines));
    }
    onAddClick(false);
}
void DayChartReferLineDlg::onColorClick(bool)
{
    bool ok=false;
    uint32_t color=ui->lineEditColor->text().toInt(&ok,16);
    if(!ok)
        color=0x7f7f7f;
    QColorDialog* dlg=new QColorDialog(QColor(color),this);
    if(dlg->exec()==QDialog::Accepted)
    {
        QColor clr=dlg->currentColor();
        color=(clr.red()<<16)|(clr.green()<<8)|clr.blue();
        QString text=QString("%1").arg(color,6,16,QChar('0'));
        ui->lineEditColor->setText(text);
    }
    delete dlg;
}
void DayChartReferLineDlg::onDeleteClick(bool)
{
    int index=ui->comboBoxView->currentIndex();
    if(index<0)return;
    int count=ui->tableWidget->rowCount();
    if(count==0)return;//没有数据，不处理
    QTableWidgetItem* item;

    for(count=count-1;count>=0;count--)
    {
        item=ui->tableWidget->item(count,1);
        if(item->isSelected())
        {
            ui->tableWidget->removeRow(count);
        }
    }
    int lastCount=ui->tableWidget->rowCount();
    if(lastCount==count)return;//没有删除

    ReferLines* lines=(ReferLines*)qvariant_cast<uint64_t>(ui->comboBoxView->itemData(index));
    QVariant var;
    PReferLine pLine;
    if(lastCount==0)
    {//全部删除
        free(lines->lines);
        memset(lines,0,sizeof(ReferLines));
    }
    PReferLine pOldLine=lines->lines;
    lines->count=lastCount;
    lines->lines=(PReferLine)malloc(sizeof(ReferLine)*lastCount);
    for(int i=0;i<lastCount;i++)
    {
        item=ui->tableWidget->item(i,1);
        if(item->isSelected())
        {
            var=item->data(TABLE_DATA_REFER_LINE);
            pLine=(PReferLine)qvariant_cast<uint64_t>(var);
            memcpy(lines->lines+i,pLine,sizeof(ReferLine));
        }
    }
    free(pOldLine);
    onAddClick(false);
}

void DayChartReferLineDlg::onViewChanged(int index)
{
    ReferLines* lines=(ReferLines*)qvariant_cast<uint64_t>(ui->comboBoxView->itemData(index));
    if(lines==NULL)return;
    PReferLine pLine=lines->lines;
    ui->tableWidget->setRowCount(lines->count);
    QTableWidgetItem* item;
    for(int i=0;i<lines->count;i++,pLine++)
    {
        //1
        item=new QTableWidgetItem(QString(tr("%1")).arg(pLine->color,8,16));
        item->setData(TABLE_DATA_REFER_LINE,QVariant((uint64_t)pLine));
        ui->tableWidget->setItem(i,1,item);

        //3
        item=new QTableWidgetItem(QString(tr("%1")).arg(pLine->lineWidth,0,'f',2));
        ui->tableWidget->setItem(i,3,item);
        if(pLine->flag&ReferLine_Horizontal)
        {
            item=new QTableWidgetItem(tr("horizontal"));
            ui->tableWidget->setItem(i,0,item);
            item=new QTableWidgetItem(QString(tr("%1")).arg(pLine->value,0,'f',2));
            ui->tableWidget->setItem(i,2,item);
        }
        else if(pLine->flag&ReferLine_Vertical)
        {
            item=new QTableWidgetItem(tr("vertical"));
            ui->tableWidget->setItem(i,0,item);
            item=new QTableWidgetItem(QString(tr("%1")).arg(pLine->index));
            ui->tableWidget->setItem(i,2,item);
        }
    }
}
}
