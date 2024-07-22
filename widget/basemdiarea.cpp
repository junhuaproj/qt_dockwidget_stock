#include "basemdiarea.h"

BaseMdiArea::BaseMdiArea(QWidget *parent)
    :QMdiArea(parent)
{

}

QColor BaseMdiArea::myBkColor()const
{
    return mmyBkColor;
}
void BaseMdiArea::setMyBkColor(QColor color)
{
    mmyBkColor=color;
    this->setBackground(mmyBkColor);
}
