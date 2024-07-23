#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#include <QString>
/*
 * 用于加载数据和计算指标用的，后来使用talib后，多数已经删除
*/

#include <vector>

#include "dayitem.h"
#include "stockday.h"
#include "appconfig.h"

class Algorithm
{
public:
    static int loadStockDataFromSQLite(const char* dbPath, std::vector<PStockDayItem>& stockDays);
    static int loadStockDataFromSQLiteForCode(const QString& code, std::vector<PStockDayItem>& stockDays,const QString& type);
    static void clearStockDay(std::vector<PStockDayItem>& stockDays);

    static void ema(double* in,double* out,int len,int period);

    static float avgerate(float* values,int count);

    //static void calcMA(vector<PStockDayItem>& stockDays,int maIndex,int maSize);
    static void calcMA(const double* closes,double* ma,int maSize,uint32_t count);
};
//过滤选项，基本不用了，通过Python实现更加灵活，高效
namespace stockfilter{
typedef bool (*FilterCallback)(float v1,float v2);

FilterCallback getCallback(FilterCompareType compareType);
bool absLessThan(float v1,float v2);
bool absGreatThan(float v1,float v2);
bool equivalent(float v1,float v2);
bool lessThan(float v1,float v2);
bool greatThan(float v1,float v2);
}

#endif
