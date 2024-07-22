#include "stockpricechgwidget.h"
#include <QPieSeries>
#include "dataloader.h"
#include "appconfig.h"

StockPriceChgWidget::StockPriceChgWidget(data::DataLoader* loader,QWidget *parent)
    : BaseWidget(parent),pDataLoader(loader),levels(3)
{
    createUI();
}

StockPriceChgWidget::~StockPriceChgWidget()
{
    clear();
}

void StockPriceChgWidget::getThemingWidget(QList<QWidget*>& wids)const
{
    wids.push_back(tabWidget);
    wids.push_back(treeWidget);
    wids.push_back(chartView);
}

void StockPriceChgWidget::clear()
{
    while(!priceClasses.isEmpty())
    {
        delete priceClasses.back();
        priceClasses.pop_back();
    }
}

void StockPriceChgWidget::priceToLevel(const float* prices,int priceCount,
                                       const float* levelRange,int rangeCount,
                                       int* levels,int levelCount)
{
    //计算0的位置,
    int levelZeroIndex=rangeCount/2;
    for(int i=0;i<priceCount;i++)
    {
        //=0
        if(prices[i]>-0.01&&prices[i]<0.01)
        {
            levels[levelCount/2]++;
            goto next;
        }

        //<0
        for(int l=0;l<=levelZeroIndex;l++)
        {
            if(prices[i]<=levelRange[l])
            {
                levels[l]++;
                goto next;
            }

        }

        //>0
        for(int l=rangeCount-1;l>=levelZeroIndex;l--)
        {
            if(prices[i]>=levelRange[l])
            {
                levels[l+2]++;
                goto next;
            }
        }
    next:
        ;
    }
}

void StockPriceChgWidget::setDate(const QDate& date)
{
    clear();
    int countSZ=0,countSH=0;
    float* priceSZs=pDataLoader->loadDatePrice(date,"sz",&countSZ);
    float* priceSHs=pDataLoader->loadDatePrice(date,"sh",&countSH);
    if(priceSZs==NULL&&priceSHs==NULL)return;

    levels.resetGroupData();
    int rangeCount;
    float* ranges=appConfig.getPriceLevel(rangeCount);
    levels.setRange(rangeCount,ranges);

    levels.group(priceSZs,countSZ);
    levels.group(priceSHs,countSH);

    if(priceSZs)
        free(priceSZs);
    if(priceSHs)
        free(priceSHs);

    updatePrices();
}
void StockPriceChgWidget::updatePrices()
{
    treeWidget->clear();
    levels.setDataToTree(treeWidget);
    QPieSeries* pie=new QPieSeries();

    levels.setPie(pie);

    pie->setLabelsPosition(QPieSlice::LabelOutside);

    QChart* chart=new QChart();
    chart->legend()->setVisible(false);
    chart->addSeries(pie);

    chartView->setChart(chart);

}
void StockPriceChgWidget::createUI()
{
    tabWidget=new QTabWidget(this);
    //tabWidget->setTabPosition(QTabWidget::East);

    layoutMain=new QVBoxLayout(this);
    //listWidget=new QListWidget(this);
    treeWidget=new QTreeWidget(this);
    //layoutMain->addWidget(listWidget);

    chartView=new QChartView(this);
    //layoutMain->addWidget(chartView);

    chartView->setRenderHint(QPainter::Antialiasing);
    //listWidget->setVisible(false);
    tabWidget->addTab(treeWidget,tr("list"));
    tabWidget->addTab(chartView,tr("chart"));

    layoutMain->addWidget(tabWidget);
    setLayout(layoutMain);
    layoutMain->setSpacing(0);

    treeWidget->setHeaderLabels({tr("level"),tr("count"),tr("percent")});
    treeWidget->setColumnCount(3);
}
