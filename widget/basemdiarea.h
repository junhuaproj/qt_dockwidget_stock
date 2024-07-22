/*
 * 为动态加载qss
*/
#ifndef BASEMDIAREA_H
#define BASEMDIAREA_H

#include <QMdiArea>

class BaseMdiArea : public QMdiArea
{
    Q_OBJECT
    Q_PROPERTY(QColor myBkColor READ myBkColor WRITE setMyBkColor NOTIFY myBkColorChanged FINAL)
public:
    BaseMdiArea(QWidget *parent = nullptr);

    QColor myBkColor()const;
    void setMyBkColor(QColor color);
protected:
    QColor mmyBkColor;

signals:
    void myBkColorChanged(QColor);
};

#endif // BASEMDIAREA_H
