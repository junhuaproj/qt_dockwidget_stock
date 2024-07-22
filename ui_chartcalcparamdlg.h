/********************************************************************************
** Form generated from reading UI file 'chartcalcparamdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTCALCPARAMDLG_H
#define UI_CHARTCALCPARAMDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartCalcParamDlg
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tabKLine;
    QWidget *tabVolume;
    QWidget *tabMacd;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QSpinBox *spinMacdSlow;
    QSpinBox *spinMacdFast;
    QSpinBox *spinMacdDea;
    QWidget *tabKdj;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *spinKdjK;
    QSpinBox *spinKdjD;
    QSpinBox *spinKdjJ;

    void setupUi(QDialog *ChartCalcParamDlg)
    {
        if (ChartCalcParamDlg->objectName().isEmpty())
            ChartCalcParamDlg->setObjectName(QString::fromUtf8("ChartCalcParamDlg"));
        ChartCalcParamDlg->resize(640, 480);
        buttonBox = new QDialogButtonBox(ChartCalcParamDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(ChartCalcParamDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 30, 601, 381));
        tabKLine = new QWidget();
        tabKLine->setObjectName(QString::fromUtf8("tabKLine"));
        tabWidget->addTab(tabKLine, QString());
        tabVolume = new QWidget();
        tabVolume->setObjectName(QString::fromUtf8("tabVolume"));
        tabWidget->addTab(tabVolume, QString());
        tabMacd = new QWidget();
        tabMacd->setObjectName(QString::fromUtf8("tabMacd"));
        gridLayoutWidget = new QWidget(tabMacd);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 20, 551, 201));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinMacdSlow = new QSpinBox(gridLayoutWidget);
        spinMacdSlow->setObjectName(QString::fromUtf8("spinMacdSlow"));

        gridLayout->addWidget(spinMacdSlow, 0, 1, 1, 1);

        spinMacdFast = new QSpinBox(gridLayoutWidget);
        spinMacdFast->setObjectName(QString::fromUtf8("spinMacdFast"));

        gridLayout->addWidget(spinMacdFast, 1, 1, 1, 1);

        spinMacdDea = new QSpinBox(gridLayoutWidget);
        spinMacdDea->setObjectName(QString::fromUtf8("spinMacdDea"));

        gridLayout->addWidget(spinMacdDea, 2, 1, 1, 1);

        tabWidget->addTab(tabMacd, QString());
        tabKdj = new QWidget();
        tabKdj->setObjectName(QString::fromUtf8("tabKdj"));
        gridLayoutWidget_2 = new QWidget(tabKdj);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(20, 20, 551, 201));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 2, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        spinKdjK = new QSpinBox(gridLayoutWidget_2);
        spinKdjK->setObjectName(QString::fromUtf8("spinKdjK"));

        gridLayout_2->addWidget(spinKdjK, 0, 1, 1, 1);

        spinKdjD = new QSpinBox(gridLayoutWidget_2);
        spinKdjD->setObjectName(QString::fromUtf8("spinKdjD"));

        gridLayout_2->addWidget(spinKdjD, 1, 1, 1, 1);

        spinKdjJ = new QSpinBox(gridLayoutWidget_2);
        spinKdjJ->setObjectName(QString::fromUtf8("spinKdjJ"));

        gridLayout_2->addWidget(spinKdjJ, 2, 1, 1, 1);

        tabWidget->addTab(tabKdj, QString());

        retranslateUi(ChartCalcParamDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ChartCalcParamDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ChartCalcParamDlg, SLOT(reject()));

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(ChartCalcParamDlg);
    } // setupUi

    void retranslateUi(QDialog *ChartCalcParamDlg)
    {
        ChartCalcParamDlg->setWindowTitle(QCoreApplication::translate("ChartCalcParamDlg", "Dialog", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabKLine), QCoreApplication::translate("ChartCalcParamDlg", "KLine", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabVolume), QCoreApplication::translate("ChartCalcParamDlg", "Volume", nullptr));
        label->setText(QCoreApplication::translate("ChartCalcParamDlg", "slow", nullptr));
        label_3->setText(QCoreApplication::translate("ChartCalcParamDlg", "dea", nullptr));
        label_2->setText(QCoreApplication::translate("ChartCalcParamDlg", "fast", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMacd), QCoreApplication::translate("ChartCalcParamDlg", "MACD", nullptr));
        label_4->setText(QCoreApplication::translate("ChartCalcParamDlg", "slow", nullptr));
        label_5->setText(QCoreApplication::translate("ChartCalcParamDlg", "dea", nullptr));
        label_6->setText(QCoreApplication::translate("ChartCalcParamDlg", "fast", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabKdj), QCoreApplication::translate("ChartCalcParamDlg", "KDJ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartCalcParamDlg: public Ui_ChartCalcParamDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTCALCPARAMDLG_H
