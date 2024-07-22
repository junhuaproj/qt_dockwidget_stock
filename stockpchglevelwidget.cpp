#include "stockpchglevelwidget.h"
#include "appconfig.h"
#include <QProgressBar>
#include "stocklistitem.h"
//#include "AppConfigqss.h"
StockPChgLevelWidget::StockPChgLevelWidget(QWidget *parent)
    : BaseWidget(parent),levels(0)//,levelCount(0),levels(NULL),valueCount(0)
{
    layoutMain=new QVBoxLayout(this);
    tree=new QTreeWidget(this);
    layoutMain->addWidget(tree);
    setLayout(layoutMain);

    tree->setHeaderLabels({tr("level"),tr("count"),tr("percent")});
    tree->setColumnCount(3);
}
StockPChgLevelWidget::~StockPChgLevelWidget()
{
}
void StockPChgLevelWidget::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(tree);
}
void priceToLevel(const double* values,int vCount,
             const float* levelRange,int rangeCount,
             ValueLevelItem* levels,int levelCount)
{
    //计算0的位置,
    int levelZeroIndex=rangeCount/2;
    for(int i=0;i<vCount;i++)
    {
        //=0
        if(values[i]>-0.01&&values[i]<0.01)
        {
            levels[levelCount/2].count++;
            goto next;
        }

        //<0
        for(int l=0;l<=levelZeroIndex;l++)
        {
            if(values[i]<=levelRange[l])
            {
                levels[l].count++;
                goto next;
            }

        }

        //>0
        for(int l=rangeCount-1;l>=levelZeroIndex;l--)
        {
            if(values[i]>=levelRange[l])
            {
                levels[l+2].count++;
                goto next;
            }
        }
    next:
        ;
    }
}


void StockPChgLevelWidget::setStock(StockListItem* stock,const double* pchg,int count)
{
//#define LEVEL_COUNT 4
    //只定义正数部分
    //float range[LEVEL_COUNT]={1,2,3,5,7,9};
    int halfCount;
    float* range=appConfig.getPriceLevel(halfCount);

    tree->clear();
    //if(levels)
    //    delete []levels;
    levels.resetGroupData();

    levels.setRange(halfCount,range);
    levels.group(pchg,count);

    setTree();

}
void StockPChgLevelWidget::setTree()
{
    tree->clear();
    levels.setDataToTree(tree);

}


StockPChgLevelDockWidget::StockPChgLevelDockWidget(
    const QString &title, QWidget *parent,
    Qt::WindowFlags flags):BaseDockWidget(title,parent,flags)
{
    pchgLevel=new StockPChgLevelWidget(this);
    setWidget(pchgLevel);
}
void StockPChgLevelDockWidget::getThemingWidget(QList<QWidget*>& wids)
{
    wids.push_back(pchgLevel);
    pchgLevel->getThemingWidget(wids);
}

void StockPChgLevelDockWidget::setStock(StockListItem* stock,const double* pchg,int count)
{
    if(stock->getFlag()&StockItemFlag_ZS)
        return ;//指数不处理
    pchgLevel->setStock(stock,pchg,count);
}
