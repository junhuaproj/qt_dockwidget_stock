#ifndef FILTERDATA_H
#define FILTERDATA_H
#include <vector>

#include "dayitem.h"
//using namespace std;

struct FilterItemPercent
{
    int prev;
    int curr;
    int percent;
};

typedef struct FilterItemPercent *PFilterItemPercent;

PFilterItemPercent newFilterItemPercent();
void clearFilterItemPercent(std::vector<PFilterItemPercent>& items);

#endif // FILTERDATA_H
