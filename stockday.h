/*
 * 日线数据
*/
#ifndef STOCKDAY_H
#define STOCKDAY_H

#include <QObject>
#include <vector>
#include "dayitem.h"
#include "stockdaydata.h"


class StockDay
{
public:
    StockDay();
    virtual ~StockDay();

    void load(std::vector<PStockDayItem>& items);
    void clear();

    const StockDayData* getStockData();
    double *getOpens() const;
    //void setOpens(double *newOpens);

    double *getCloses() const;
    //void setCloses(double *newCloses);

    double *getHighs() const;
    //void setHighs(double *newHighs);

    double *getLows() const;
    //void setLows(double *newLows);

    double *getChgs() const;
    //void setChgs(double *newChg);

    StockFloat *getVoturnovers() const;
    //void setVoturnovers(int *newVoturnovers);

    double *getVaturnovers() const;
    //void setVaturnovers(double *newVaturnovers);

    double *getTurnovers() const;
    //void setTurnovers(double *newTurnovers);

    double *getPchgs() const;
    //void setPchgs(double *newPchgs);

    PStockDate getDates();

    int32_t getCount() const;

    int findDate(const char* date);
    int findDate(const QString& date);
    //void calcBiasRange(int start,int count);
protected:
    void allowBaseBuf(uint32_t count);
#if 0
    void clearMA();
#endif
protected:
    StockDayData dayData;
    int32_t count;

    PStockDate dates;
    StockFloat** bases;
    StockFloat*    voturnovers;

};



#endif // STOCKDAY_H
