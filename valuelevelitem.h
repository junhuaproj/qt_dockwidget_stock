#ifndef VALUELEVELITEM_H
#define VALUELEVELITEM_H

#include <QString>
#include <QList>
#include <QTreeWidget>
#include <QPieSeries>

class ValueLevelItem
{
public:
    ValueLevelItem();
    ValueLevelItem(float level,int compare);
    virtual ~ValueLevelItem();
    void getTitle(QString& title);
    //QString title;
    bool testValue(int index,float v);
    void setRange(int count,float* range,int compare);

    int count;
    QList<int> indexes;

    QList<ValueLevelItem*>* children;
    void clear();
    void resetData();
    inline int getCompare()const
    {
        return compare;
    }
protected:
    float level;
    int compare;
};

class ValueItemLevelThree
{
public:
    ValueItemLevelThree(float center);
    virtual ~ValueItemLevelThree();

    void setRange(int count,float* range);
    void group(const float* values,int count);
    void group(const double* values,int count);
    void resetGroupData();
    void setDataToTree(QTreeWidget* tree);
    void setPie(QPieSeries* pie);
    void addChildTreeItem(QTreeWidget* tree,QTreeWidgetItem* parent,
                                               QList<ValueLevelItem*>* children );
protected:
    float levelCenter;
    int dataCount;
    int levelCount;
    ValueLevelItem* items;
};

#endif // VALUELEVELITEM_H
