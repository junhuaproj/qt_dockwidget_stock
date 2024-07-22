#ifndef STOCKCHARTLABEL_H
#define STOCKCHARTLABEL_H
#include <QString>

class StockChartLabel
{
public:
    StockChartLabel();

    int getValue() const;
    void setValue(int newValue);

    const QString& getLabel() const;
    void setLabel(const QString &newLabel);

protected:
    int value;
    QString label;
};

#endif // STOCKCHARTLABEL_H
