//#include "pch.h"
#include <stdio.h>
#include "Algorithm.h"
#include "sqlite3.h"
//#include "cJSON.h"
#include <QFile>
#include <QDirIterator>

#include <float.h>

#ifndef MAX_PATH
#define MAX_PATH    512
#endif

int Algorithm::loadStockDataFromSQLiteForCode(const QString& code, std::vector<PStockDayItem>& stockDays,const QString& type)
{
    char inpath[MAX_PATH];
    //sprintf_s(inpath, "F:\\stock\\sqlite\\cn_%s.db",
    sprintf_s(inpath, "F:\\stock\\sqlite\\%s_%s.db",
              type.toStdString().c_str(),code.toStdString().c_str());
    return loadStockDataFromSQLite(inpath,stockDays);
}
int Algorithm::loadStockDataFromSQLite(const char* dbPath, std::vector<PStockDayItem>& stockDays)
{
    sqlite3* db = NULL;
    int ret;
    //char path[MAX_PATH];
    //sprintf_s(path, "F:\\stock\\sqlite\\cn_%s.db", code);
    ret = sqlite3_open(dbPath, &db);
    if (ret != SQLITE_OK)
        return -1;

    sqlite3_stmt* stmt;
    //voturnover 交易数量
    ret = sqlite3_prepare(db, "select ymd,open,close,chg,pchg,low,high,voturnover,vaturnover,turnover from data", -1,
        &stmt, NULL);
    if (ret == SQLITE_OK)
    {
        while (SQLITE_ROW == sqlite3_step(stmt))
        {
            StockDayItem* p = (StockDayItem*)malloc(sizeof(StockDayItem));
            const char* date = (const char*)sqlite3_column_text(stmt, 0);
            memset(p,0,sizeof(StockDayItem));
            memcpy(p->date,date,10);
            //p->date[10]=0;
            //memcpy(p->date, date, 10);
            //for (int i = 0; i < 10; i++)
            //    p->date[i] = date[i];
            //p->date[10] = 0;
            p->open = (float)sqlite3_column_double(stmt, 1);
            p->close = (float)sqlite3_column_double(stmt, 2);
            p->chg = (float)sqlite3_column_double(stmt, 3);
            p->pchg = (float)sqlite3_column_double(stmt, 4);
            p->low = (float)sqlite3_column_double(stmt, 5);
            p->high = (float)sqlite3_column_double(stmt, 6);
            p->voturnover = sqlite3_column_int(stmt, 7);

            p->vaturnover = (float)sqlite3_column_double(stmt, 8);
            p->turnover = (float)sqlite3_column_double(stmt, 9);

            stockDays.push_back(p);
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}

void Algorithm::clearStockDay(std::vector<PStockDayItem>& stockDays)
{
    while (!stockDays.empty())
    {
        free(stockDays.back());
        stockDays.pop_back();
    }
}


void Algorithm::ema(double* in,double* out,int len,int period)
{
    out[0]=0;
    double curRate = (float)2 / (period + 1);
    double prevRate = ((float)period - 1) / (period + 1);
    for(int i=1;i<len;i++)
    {
        out[i] = in[i] * curRate + out[i-1]*prevRate;
    }
}

float Algorithm::avgerate(float* values,int count)
{
    float sum=0;
    for(int i=0;i<count;i++)
        sum+=values[i];
    return sum/count;
}
void floatMov(float* v1,float* v2,int len)
{
    for(int i=0;i<len;i++)
    {
        v1[i]=v2[i];
    }
}
void Algorithm::calcMA(const double* closes,double* ma,int maSize,uint32_t count)
{
    //vector<PStockDayItem>::iterator it = stockDays.begin(),
    //    end = stockDays.end();

    float* prices=new float[maSize];
    int valCount=0;
    for (uint32_t i=0;i<count;i++)
    {
        if(valCount<maSize)
        {
            prices[valCount]=closes[i];
            valCount++;
            ma[i]=avgerate(prices,valCount);
        }
        else
        {
            //memmove_s(prices,sizeof(float)*maSize,prices+1,sizeof(float)*(maSize-1));
            floatMov(prices,&prices[1],maSize-1);
            //memmove(prices+1,prices,sizeof(float)*(maSize-1));
            prices[maSize-1]=closes[i];
            ma[i]=avgerate(prices,maSize);
        }

        //(*it)->macd = 2 * ((*it)->diff - (*it)->dea);
    }
    delete []prices;
}

//typedef bool (*FilterCallback)(float v1,float v2);

namespace stockfilter{
bool greatThan(float v1,float v2)
{
    return v1>v2;
}

bool lessThan(float v1,float v2)
{
    return v1<v2;
}

bool equivalent(float v1,float v2)
{
    return v1==v2;
}
bool absGreatThan(float v1,float v2)
{
    return abs(v1)>abs(v2);
}

bool absLessThan(float v1,float v2)
{
    return abs(v1)<abs(v2);
}

FilterCallback getCallback(FilterCompareType compareType)
{
    FilterCallback p=NULL;
    switch(compareType)
    {
    case FilterCompareType_LessThan:
        p=lessThan;break;
    case FilterCompareType_Equivalent:
        p=equivalent;break;
    case FilterCompareType_GreatThan:
        p=greatThan; break;
    case FilterCompareType_AbsLessThan:
        p=absLessThan;break;
    case FilterCompareType_AbsGreatThan:
        p=absGreatThan; break;
    }
    return p;
}
}
