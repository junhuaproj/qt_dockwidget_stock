/*
 * 记录
*/
#ifndef STOCKRECORDITEM_H
#define STOCKRECORDITEM_H
#include <QDateTime>

class StockListItem;
namespace data {

class StockRecordItem
{
public:
    StockRecordItem();


    const QDateTime& getDatetime() const;
    void setDatetime(const QDateTime &newDatetime);

    inline const QString& getText() const
    {
        return text;
    }
    void setText(const QString &newText);

    const QString& getKey() const;
    void setKey(const QString &newKey);

    const QString& getCondtion() const;
    void setCondtion(const QString &newCondtion);

    int getId() const;
    void setId(int newId);

    int getParent() const;
    void setParent(int newParent);

    int getLevel() const;
    void setLevel(int newLevel);

    inline const StockListItem* getStock()const
    {
        return pStock;
    }
    void setStock(const StockListItem* pStock);
    float getDownPercent() const;
    void setDownPercent(float newDownPercent);

    int getDownDay() const;
    void setDownDay(int newDownDay);

    float getLastKeyVal() const;
    void setLastKeyVal(float newLastKeyVal);

protected:
    const StockListItem* pStock;
    int id;
    int parent;
    int level;
    QDateTime datetime;
#ifdef DB_MYSQL
#else
    QString code;
#endif
    QString text;
    QString key;
    QString condtion;

    int downDay;
    float lastKeyVal;
    float downPercent;
};

//extern const char* datetime_format;
//extern const char* date_format;

char* StringToStr(const QString& s);
#ifdef DB_MYSQL
bool find_id_from_record_func(StockRecordItem* p,StockListItem* stock);
#else
bool find_code_from_record_func(StockRecordItem* p,const QString& code);
#endif
}
#endif // STOCKRECORDITEM_H
