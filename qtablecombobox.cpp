#include "qtablecombobox.h"
#include <QFocusEvent>
#include <QObjectList>
#include <QAbstractItemView>

QTableComboBox::QTableComboBox(QWidget *parent)
    :QComboBox(parent),data1(0),data2(0)
{
}


void QTableComboBox::focusOutEvent(QFocusEvent *e)
{
    QComboBox::focusOutEvent(e);
    //this->deleteLater();

    //if(e->lostFocus())
    {
        if(hasFocus()||view()->hasFocus())
        {}
        else    emit tableCbxKillFocus(this);
    }
}
void QTableComboBox::leaveEvent(QEvent* e)
{
}
uint64_t QTableComboBox::getData1() const
{
    return data1;
}

void QTableComboBox::setData1(uint64_t newData1)
{
    data1 = newData1;
}

uint64_t QTableComboBox::getData2() const
{
    return data2;
}

void QTableComboBox::setData2(uint64_t newData2)
{
    data2 = newData2;
}
