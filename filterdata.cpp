#include "filterdata.h"

PFilterItemPercent newFilterItemPercent()
{
    PFilterItemPercent p=(PFilterItemPercent)malloc(sizeof(FilterItemPercent));
    memset(p,0,sizeof(FilterItemPercent));
    return p;
}
void clearFilterItemPercent(std::vector<PFilterItemPercent>& items)
{
    while(!items.empty())
    {
        free(items.back());
        items.pop_back();
    }
}
