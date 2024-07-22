/**
 * 一天的分钟线
*/
#ifndef STOCKDAYMINUTE_H
#define STOCKDAYMINUTE_H

#include <QString>
#include "stockpriceitem.h"
#include <QPointF>
#include <QDate>

class QPainter;
class QPointF;

namespace minutechart
{

#define SOHU_FILL_COUNT 242
class StockDayMinute
{
public:
    StockDayMinute(void* pCreater,int fillCount=0);
    virtual ~StockDayMinute();
    void reset();//清空数据，释放内存
    /**
     * 增加分钟数据
    */
    void addMin(const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice=0);
    void addMin(uint16_t hour,uint16_t minute,float price,int32_t volume,int32_t fund,float avgPrice=0);
    /**
     * 获得价格区间
    */
    void getPriceRange(float& min,float& max)const;
    /**
     * 获得成交量区间
    */
    void getVolumeRange(int32_t& min,int32_t& max)const;
    /**
     * 获得价格折线图的点
    */
    int getPricePoints(QPointF* pts,qreal itemWidth,float lineWidth,float rate,const QPointF& start,float startValue)const;
    /**
     * 画交易量柱形图
    */
    int paintVolume(QPainter* painter,qreal itemWidth,float lineWidth,float rate,const QPointF& start,int32_t startValue)const;
    /**
     * 设置分钟数据
    */
    bool setMin(int index,const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice=0);
    /**
     * 设置分钟数据
    */
    void setPriceItem(StockPriceItem* p,const QString& hhmm,float price,int32_t volume,int32_t fund,float avgPrice=0);
    /**
     * 覆盖空数据
    */
    void fillEmpty(int count=SOHU_FILL_COUNT);

    inline int count()
    {
        return minute.size();
    }
    void setPrices(float preClosePrice,float open,float high,float low){
        preClose=preClosePrice;
    }
    float getPreClose()const;
    QDate date;
    QList<StockPriceItem*> minute;
    inline bool isFillMode(){return bFilled;}
    void reCalc();
    double getTotalVolume() const;

    double getTotalFund() const;

    inline bool isCreate(void* p)
    {
        return p==pCreate;
    }
protected:
    void* pCreate;//指向创建这个对象的指针，由创建者删除
protected:
    float preClose;
    double totalVolume;
    double totalFund;
    bool bFilled;
};
}
#endif // STOCKDAYMINUTE_H
