/*
 * 模拟交易
*/
#ifndef SIMULATETRADE_H
#define SIMULATETRADE_H

#include <QString>
namespace simulate{
class SimulateTrade
{
public:
    SimulateTrade();
    virtual ~SimulateTrade();
    QString buyDate;
    double buyPrice;
    QString sellDate;
    double sellPrice;
    QString note;
    double getProfit();
};
}
#endif // SIMULATETRADE_H
