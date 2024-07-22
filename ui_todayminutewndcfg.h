/********************************************************************************
** Form generated from reading UI file 'todayminutewndcfg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TODAYMINUTEWNDCFG_H
#define UI_TODAYMINUTEWNDCFG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TodayMinuteCfg
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSpinBox *spinBoxColumn;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *spinBoxRow;
    QLabel *label_3;
    QSpinBox *spinBoxUpdateTime;

    void setupUi(QDialog *TodayMinuteCfg)
    {
        if (TodayMinuteCfg->objectName().isEmpty())
            TodayMinuteCfg->setObjectName(QString::fromUtf8("TodayMinuteCfg"));
        TodayMinuteCfg->resize(640, 480);
        buttonBox = new QDialogButtonBox(TodayMinuteCfg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(TodayMinuteCfg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(40, 50, 431, 261));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        spinBoxColumn = new QSpinBox(gridLayoutWidget);
        spinBoxColumn->setObjectName(QString::fromUtf8("spinBoxColumn"));

        gridLayout->addWidget(spinBoxColumn, 1, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinBoxRow = new QSpinBox(gridLayoutWidget);
        spinBoxRow->setObjectName(QString::fromUtf8("spinBoxRow"));

        gridLayout->addWidget(spinBoxRow, 0, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        spinBoxUpdateTime = new QSpinBox(gridLayoutWidget);
        spinBoxUpdateTime->setObjectName(QString::fromUtf8("spinBoxUpdateTime"));

        gridLayout->addWidget(spinBoxUpdateTime, 2, 1, 1, 1);


        retranslateUi(TodayMinuteCfg);
        QObject::connect(buttonBox, SIGNAL(accepted()), TodayMinuteCfg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TodayMinuteCfg, SLOT(reject()));

        QMetaObject::connectSlotsByName(TodayMinuteCfg);
    } // setupUi

    void retranslateUi(QDialog *TodayMinuteCfg)
    {
        TodayMinuteCfg->setWindowTitle(QCoreApplication::translate("TodayMinuteCfg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("TodayMinuteCfg", "\345\233\276\350\241\250\350\241\214\346\225\260", nullptr));
        label_2->setText(QCoreApplication::translate("TodayMinuteCfg", "\345\233\276\350\241\250\345\210\227\346\225\260", nullptr));
        label_3->setText(QCoreApplication::translate("TodayMinuteCfg", "\350\207\252\345\212\250\346\233\264\346\226\260\346\227\266\351\227\264", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TodayMinuteCfg: public Ui_TodayMinuteCfg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TODAYMINUTEWNDCFG_H
