/********************************************************************************
** Form generated from reading UI file 'selectcomparedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTCOMPAREDLG_H
#define UI_SELECTCOMPAREDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectCompareDlg
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QRadioButton *radioMain;
    QRadioButton *radioSlave;
    QLabel *label;
    QTableWidget *tableWidget;

    void setupUi(QDialog *SelectCompareDlg)
    {
        if (SelectCompareDlg->objectName().isEmpty())
            SelectCompareDlg->setObjectName(QString::fromUtf8("SelectCompareDlg"));
        SelectCompareDlg->resize(640, 480);
        buttonBox = new QDialogButtonBox(SelectCompareDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(SelectCompareDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(30, 340, 371, 80));
        radioMain = new QRadioButton(groupBox);
        radioMain->setObjectName(QString::fromUtf8("radioMain"));
        radioMain->setGeometry(QRect(20, 40, 97, 21));
        radioSlave = new QRadioButton(groupBox);
        radioSlave->setObjectName(QString::fromUtf8("radioSlave"));
        radioSlave->setGeometry(QRect(150, 40, 97, 21));
        label = new QLabel(SelectCompareDlg);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 161, 16));
        tableWidget = new QTableWidget(SelectCompareDlg);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(30, 50, 581, 281));

        retranslateUi(SelectCompareDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), SelectCompareDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SelectCompareDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(SelectCompareDlg);
    } // setupUi

    void retranslateUi(QDialog *SelectCompareDlg)
    {
        SelectCompareDlg->setWindowTitle(QCoreApplication::translate("SelectCompareDlg", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SelectCompareDlg", "replace", nullptr));
        radioMain->setText(QCoreApplication::translate("SelectCompareDlg", "main", nullptr));
        radioSlave->setText(QCoreApplication::translate("SelectCompareDlg", "slave", nullptr));
        label->setText(QCoreApplication::translate("SelectCompareDlg", "select window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectCompareDlg: public Ui_SelectCompareDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTCOMPAREDLG_H
