#include "tablewidgetitemint.h"

#include "stockrecorditem.h"

TableWidgetItemInt::TableWidgetItemInt(int value, int type)
    :QTableWidgetItem(QString("%1").arg(value),type)
    ,iValue(value)
{
    //setData(LIST_DATA_RECORD,(uint64_t)pRecord);
}
TableWidgetItemInt::~TableWidgetItemInt()
{

}

bool TableWidgetItemInt::operator <(const QTableWidgetItem &other)const
{
    return iValue<((const TableWidgetItemInt*)&other)->getIntValue();
}
int TableWidgetItemInt::getIntValue()const
{
    return iValue;
}
