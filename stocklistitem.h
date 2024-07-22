/*
 * 股票列表项
**/
#ifndef __STOCKLISTITEM_H__
#define __STOCKLISTITEM_H__

#include <QString>
#include <QDate>

enum StockItemFlag{
    StockItemFlag_CN=0x01,
    StockItemFlag_SH=0x02,//shanghai
    StockItemFlag_SZ=0x04,//shenzhen
    StockItemFlag_ZS=0x08,//指数
};

class StockListItem
{
public:
    StockListItem();
    StockListItem(const StockListItem& item);
    StockListItem(const QString& code,const QString& name,uint32_t flag=0);
    const QString& getCode() const;
    bool isShanghaiZS()const;
    bool isShenzhenZS()const;
    void setCode(const QString &newCode);

    const QString& getName() const;
    void setName(const QString &newName);

    uint32_t getFlag() const;
    void setFlag(uint32_t newFlag);

    //inline StockListItem& operator=(StockListItem& src)
    void set(const StockListItem* src);

    bool isShanghai()const;
    bool isShenzhen()const;
    void toString(QString& str)const;

    bool filter(uint32_t flag,const QString& key);
    // double getLastPrice() const;
    // void setLastPrice(double newLastPrice);

#ifdef DB_MYSQL
    inline int getId()const{
        return id;
    }
    void setId(int newId);

#endif
    QString getComment() const;
    void setComment(const QString &newComment);

    QDate getCommentDate() const;
    void setCommentDate(const QDate &newCommentDate);

    int getCommentLevel() const;
    void setCommentLevel(int newCommentLevel);

protected:
#ifdef DB_MYSQL
    int id;
#endif
    QString code;//编号
    QString name;//名称
    uint32_t flag;//标志
    // double lastPrice;

    QString comment;//备注
    QDate commentDate;//备注日期
    int commentLevel;//备注分级
};
/*
class StockInfo
{
public:
    StockInfo(){}
    virtual ~StockInfo(){}
    QString code;
    QString name;
    QString keyword;
    QString scope;
    QString dashi;
    QString remind;
    QString profile;
    QString internel;
    QString summary;
};
*/
#endif // STOCKLISTITEM_H
