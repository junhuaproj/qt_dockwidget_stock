#include "simulatetrade.h"

namespace simulate{
SimulateTrade::SimulateTrade() {}


SimulateTrade::~SimulateTrade()
{

}

double SimulateTrade::getProfit(){
    return sellPrice-buyPrice;
}

}
