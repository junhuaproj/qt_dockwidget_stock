/*
 * 用于QTableWidget整数值时的排序
*/
#ifndef TABLEWIDGETITEMINT_H
#define TABLEWIDGETITEMINT_H

#include <QObject>
#include <QTableWidgetItem>
class StockRecordItem;


class TableWidgetItemInt:public QTableWidgetItem
{
    //Q_OBJECT
public:
    explicit TableWidgetItemInt(int iValue, int type = Type);
    virtual ~TableWidgetItemInt();

    bool operator <(const QTableWidgetItem &other)const;
    int getIntValue()const;
protected:
    int iValue;
};

#endif // TABLEWIDGETITEMINT_H
