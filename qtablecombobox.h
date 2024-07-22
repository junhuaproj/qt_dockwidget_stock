/*
 * 用于QTableWidget的Combo
*/
#ifndef QTABLECOMBOBOX_H
#define QTABLECOMBOBOX_H

#include <QComboBox>
#include <QObject>

class QTableComboBox : public QComboBox
{
    Q_OBJECT
public:
    QTableComboBox(QWidget *parent = nullptr);
    void focusOutEvent(QFocusEvent *e) override;
    void leaveEvent(QEvent* e) override;

    uint64_t getData1() const;
    void setData1(uint64_t newData1);

    uint64_t getData2() const;
    void setData2(uint64_t newData2);

protected:
    uint64_t data1;
    uint64_t data2;
signals:
    void tableCbxKillFocus(QTableComboBox* cbx);
};

#endif // QTABLECOMBOBOX_H
