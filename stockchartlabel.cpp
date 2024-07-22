#include "stockchartlabel.h"

StockChartLabel::StockChartLabel() {}

int StockChartLabel::getValue() const
{
    return value;
}

void StockChartLabel::setValue(int newValue)
{
    value = newValue;
}

const QString&  StockChartLabel::getLabel() const
{
    return label;
}

void StockChartLabel::setLabel(const QString &newLabel)
{
    label = newLabel;
}
