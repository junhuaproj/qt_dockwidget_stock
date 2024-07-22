#include "stockday.h"
#include <float.h>

StockDay::StockDay():count(0),

    dates(NULL),
    voturnovers(NULL),
    bases(NULL)
{
    memset(&dayData,0,sizeof(StockDayData));
}

StockDay::~StockDay()
{
    clear();
}

void StockDay::load(std::vector<PStockDayItem>& items)
{
    clear();
    std::vector<PStockDayItem>::iterator it=items.begin(),end=items.end();
    allowBaseBuf(items.size());

    for(int i=0;it!=end;it++,i++)
    {
        memcpy(dates[i].date,(*it)->date,STOCK_DATE_LEN);
        voturnovers[i]=(*it)->voturnover;
        //volRange.low= fmin(volRange.low,voturnovers[i]);
        //volRange.high= fmax(volRange.high,voturnovers[i]);
        bases[Stock_Open][i]=(*it)->open;
        bases[Stock_Close][i]=(*it)->close;
        bases[Stock_High][i]=(*it)->high;
        bases[Stock_Low][i]=(*it)->low;
        bases[Stock_Chg][i]=(*it)->chg;
        bases[Stock_Pchg][i]=(*it)->pchg;
        bases[Stock_Vaturnovers][i]=(*it)->vaturnover;
        bases[Stock_Turnovers][i]=(*it)->turnover;
    }
}

void StockDay::allowBaseBuf(uint32_t count)
{
    this->count=count;
    dates= (StockDate*)malloc(sizeof(StockDate)*count);
    memset(dates,0,sizeof(StockDate)*count);
    voturnovers=(StockFloat*)malloc(sizeof(StockFloat)*count);
    memset(voturnovers,0,sizeof(StockInt)*count);
    bases=(StockFloat**)malloc(sizeof(StockFloat*)*((int)Stock_Dbl));
    for(int i=0;i<(int)Stock_Dbl;i++)
    {
        bases[i]=(StockFloat*)malloc(sizeof(StockFloat)*count);
        memset(bases[i],0,sizeof(StockFloat)*count);
    }
}

int StockDay::findDate(const char* date)
{
    for(uint32_t i=0;i<count;i++)
    {
        if(strcmp(date,dates[i].date)==0)
            return i;
    }
    return -1;
}
int StockDay::findDate(const QString& date)
{
    return findDate(date.toStdString().c_str());
}

void StockDay::clear()
{

    if(dates)
    {
        free(dates);
        dates=NULL;
    }
    if(bases)
    {
        for(int i=0;i<(int)Stock_Dbl;i++)
        {
            free(bases[i]);
        }
        bases=NULL;
    }
    if(voturnovers)
    {
        free(voturnovers);
        voturnovers=NULL;
    }
    count=0;
}
PStockDate StockDay::getDates()
{

    return dates;
}

int32_t StockDay::getCount() const
{
    return count;
}

double *StockDay::getOpens() const
{
    return bases[Stock_Open];
}


double *StockDay::getCloses() const
{
    return bases[Stock_Close];
}

double *StockDay::getHighs() const
{
    return bases[Stock_High];
}

double *StockDay::getLows() const
{
    return bases[Stock_Low];
}


double *StockDay::getChgs() const
{
    return bases[Stock_Low];
}

StockFloat *StockDay::getVoturnovers() const
{
    return voturnovers;
}


double *StockDay::getVaturnovers() const
{
    return bases[Stock_Vaturnovers];
}

double *StockDay::getTurnovers() const
{
    return bases[Stock_Turnovers];
}


const StockDayData* StockDay::getStockData()
{
    dayData.vol=voturnovers;
    dayData.closes=getCloses();
    dayData.dates=dates;
    dayData.highs=getHighs();
    dayData.lows=getLows();
    dayData.opens=getOpens();
    dayData.count=count;

    return &dayData;
}
double *StockDay::getPchgs() const
{
    return bases[Stock_Pchg];
}

void calcFloatRange(const StockFloat* values,StockFloatRange* range,int start,int32_t count)
{
    range->high=FLT_MIN;
    range->low=FLT_MAX;
    int32_t end=start+count;
    for(;start<+end;start++)
    {
        range->high=fmax(range->high,values[start]);
        range->low=fmin(range->low,values[start]);
    }
}
