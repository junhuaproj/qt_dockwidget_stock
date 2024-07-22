/********************************************************************************
** Form generated from reading UI file 'daychartcfgdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAYCHARTCFGDLG_H
#define UI_DAYCHARTCFGDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DayChartCfgDlg
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QComboBox *comboMainType;
    QComboBox *comboView3Type;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *checkBoxMainVisible;
    QCheckBox *checkBoxView3Visible;
    QLabel *label_5;
    QComboBox *comboView2Type;
    QLabel *label;
    QCheckBox *checkBoxView2Visible;
    QLabel *label_2;
    QSpinBox *spinBoxView3Height;
    QSpinBox *spinBoxView2Height;
    QSpinBox *spinBoxMainHeight;

    void setupUi(QDialog *DayChartCfgDlg)
    {
        if (DayChartCfgDlg->objectName().isEmpty())
            DayChartCfgDlg->setObjectName(QString::fromUtf8("DayChartCfgDlg"));
        DayChartCfgDlg->resize(640, 480);
        buttonBox = new QDialogButtonBox(DayChartCfgDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(DayChartCfgDlg);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 80, 511, 271));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        comboMainType = new QComboBox(layoutWidget);
        comboMainType->setObjectName(QString::fromUtf8("comboMainType"));

        gridLayout->addWidget(comboMainType, 1, 2, 1, 1);

        comboView3Type = new QComboBox(layoutWidget);
        comboView3Type->setObjectName(QString::fromUtf8("comboView3Type"));

        gridLayout->addWidget(comboView3Type, 3, 2, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(16777215, 30));

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        checkBoxMainVisible = new QCheckBox(layoutWidget);
        checkBoxMainVisible->setObjectName(QString::fromUtf8("checkBoxMainVisible"));

        gridLayout->addWidget(checkBoxMainVisible, 1, 3, 1, 1);

        checkBoxView3Visible = new QCheckBox(layoutWidget);
        checkBoxView3Visible->setObjectName(QString::fromUtf8("checkBoxView3Visible"));

        gridLayout->addWidget(checkBoxView3Visible, 3, 3, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(16777215, 30));

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        comboView2Type = new QComboBox(layoutWidget);
        comboView2Type->setObjectName(QString::fromUtf8("comboView2Type"));

        gridLayout->addWidget(comboView2Type, 2, 2, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        checkBoxView2Visible = new QCheckBox(layoutWidget);
        checkBoxView2Visible->setObjectName(QString::fromUtf8("checkBoxView2Visible"));

        gridLayout->addWidget(checkBoxView2Visible, 2, 3, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        spinBoxView3Height = new QSpinBox(layoutWidget);
        spinBoxView3Height->setObjectName(QString::fromUtf8("spinBoxView3Height"));

        gridLayout->addWidget(spinBoxView3Height, 3, 1, 1, 1);

        spinBoxView2Height = new QSpinBox(layoutWidget);
        spinBoxView2Height->setObjectName(QString::fromUtf8("spinBoxView2Height"));

        gridLayout->addWidget(spinBoxView2Height, 2, 1, 1, 1);

        spinBoxMainHeight = new QSpinBox(layoutWidget);
        spinBoxMainHeight->setObjectName(QString::fromUtf8("spinBoxMainHeight"));

        gridLayout->addWidget(spinBoxMainHeight, 1, 1, 1, 1);


        retranslateUi(DayChartCfgDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), DayChartCfgDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DayChartCfgDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(DayChartCfgDlg);
    } // setupUi

    void retranslateUi(QDialog *DayChartCfgDlg)
    {
        DayChartCfgDlg->setWindowTitle(QCoreApplication::translate("DayChartCfgDlg", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("DayChartCfgDlg", "\345\233\276\350\241\250\344\270\211", nullptr));
        label_4->setText(QCoreApplication::translate("DayChartCfgDlg", "\351\253\230\345\272\246\346\257\224\344\276\213", nullptr));
        checkBoxMainVisible->setText(QCoreApplication::translate("DayChartCfgDlg", "\346\230\276\347\244\272", nullptr));
        checkBoxView3Visible->setText(QCoreApplication::translate("DayChartCfgDlg", "\346\230\276\347\244\272", nullptr));
        label_5->setText(QCoreApplication::translate("DayChartCfgDlg", "\345\233\276\350\241\250\347\261\273\345\236\213", nullptr));
        label->setText(QCoreApplication::translate("DayChartCfgDlg", "\344\270\273\345\233\276\350\241\250", nullptr));
        checkBoxView2Visible->setText(QCoreApplication::translate("DayChartCfgDlg", "\346\230\276\347\244\272", nullptr));
        label_2->setText(QCoreApplication::translate("DayChartCfgDlg", "\345\233\276\350\241\250\344\272\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DayChartCfgDlg: public Ui_DayChartCfgDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAYCHARTCFGDLG_H
