/*
 * 记录列表
*/
#ifndef STOCKRECORDLIST_H
#define STOCKRECORDLIST_H

#include <QObject>
#include <QList>
#include "stockrecorditem.h"
#include "mysqldata/mydb.h"
namespace data {
class StockRecordDb;

class StockRecordList : public QObject
{
    Q_OBJECT
public:
    explicit StockRecordList(QObject *parent = nullptr);

    virtual ~StockRecordList();
    void clear();

    int getId() const;
    void setId(int newId);

    const QString& getTitle() const;
    void setTitle(const QString &newTitle);

    const QDateTime& getDatetime() const;
    void setDatetime(const QDateTime &newDatetime);

    const QString& getContent() const;
    void setContent(const QString &newContent);

    int getLevel() const;
    void setLevel(int newLevel);

    void addItem(StockRecordItem * pItem);


    void removeItem(StockRecordItem* p);
    void addItems(const QList<StockRecordItem *> &newItems);
    const QList<StockRecordItem *>& getItems() const;
    QList<StockRecordItem *>& getDeepItems();
    void setItems(const QList<StockRecordItem *> &newItems);
    bool isRecordExist(const StockRecordItem* pItem);
#ifdef DB_MYSQL
    StockRecordItem* findRecord(const StockListItem* p);
#else
    StockRecordItem* findRecord(const QString& code);
void removeItem(const QString& code);
#endif
    void delItems(MyDB* db);
    // void addItems(MyDB* db);
    // bool save(data::StockRecordDb* db);
    void saveItems(MyDB* db);//data::StockRecordDb* db);
    void loadItems(MyDB* db);

    void updateSubItem();
protected:
    int id;
    QString title;
    QDateTime datetime;
    QString content;
    int level;
    QList<StockRecordItem*> items;
signals:
};
}
#endif // STOCKRECORDLIST_H
