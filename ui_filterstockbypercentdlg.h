/********************************************************************************
** Form generated from reading UI file 'filterstockbypercentdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERSTOCKBYPERCENTDLG_H
#define UI_FILTERSTOCKBYPERCENTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilterStockByStockDlg
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBoxType;
    QSpinBox *spinBoxPercent;

    void setupUi(QDialog *FilterStockByStockDlg)
    {
        if (FilterStockByStockDlg->objectName().isEmpty())
            FilterStockByStockDlg->setObjectName(QString::fromUtf8("FilterStockByStockDlg"));
        FilterStockByStockDlg->resize(504, 325);
        buttonBox = new QDialogButtonBox(FilterStockByStockDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 270, 471, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalLayoutWidget = new QWidget(FilterStockByStockDlg);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(100, 70, 321, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBoxType = new QComboBox(horizontalLayoutWidget);
        comboBoxType->setObjectName(QString::fromUtf8("comboBoxType"));
        comboBoxType->setMaxCount(9);

        horizontalLayout->addWidget(comboBoxType);

        spinBoxPercent = new QSpinBox(horizontalLayoutWidget);
        spinBoxPercent->setObjectName(QString::fromUtf8("spinBoxPercent"));

        horizontalLayout->addWidget(spinBoxPercent);


        retranslateUi(FilterStockByStockDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), FilterStockByStockDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FilterStockByStockDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(FilterStockByStockDlg);
    } // setupUi

    void retranslateUi(QDialog *FilterStockByStockDlg)
    {
        FilterStockByStockDlg->setWindowTitle(QCoreApplication::translate("FilterStockByStockDlg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FilterStockByStockDlg", "\346\257\224\344\276\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FilterStockByStockDlg: public Ui_FilterStockByStockDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERSTOCKBYPERCENTDLG_H
