/********************************************************************************
** Form generated from reading UI file 'chartthemedlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTTHEMEDLG_H
#define UI_CHARTTHEMEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChartThemeDlg
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButtonDark;
    QRadioButton *radioButtonLight;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QTableWidget *tableWidget;

    void setupUi(QDialog *ChartThemeDlg)
    {
        if (ChartThemeDlg->objectName().isEmpty())
            ChartThemeDlg->setObjectName(QString::fromUtf8("ChartThemeDlg"));
        ChartThemeDlg->resize(640, 480);
        buttonBox = new QDialogButtonBox(ChartThemeDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        verticalLayoutWidget = new QWidget(ChartThemeDlg);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 611, 411));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButtonDark = new QRadioButton(groupBox);
        radioButtonDark->setObjectName(QString::fromUtf8("radioButtonDark"));
        radioButtonDark->setChecked(true);

        horizontalLayout_2->addWidget(radioButtonDark);

        radioButtonLight = new QRadioButton(groupBox);
        radioButtonLight->setObjectName(QString::fromUtf8("radioButtonLight"));

        horizontalLayout_2->addWidget(radioButtonLight);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(verticalLayoutWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout->addWidget(listWidget);

        tableWidget = new QTableWidget(verticalLayoutWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        horizontalLayout->addWidget(tableWidget);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ChartThemeDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ChartThemeDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ChartThemeDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChartThemeDlg);
    } // setupUi

    void retranslateUi(QDialog *ChartThemeDlg)
    {
        ChartThemeDlg->setWindowTitle(QCoreApplication::translate("ChartThemeDlg", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ChartThemeDlg", "\344\270\273\351\242\230", nullptr));
        radioButtonDark->setText(QCoreApplication::translate("ChartThemeDlg", "dark", nullptr));
        radioButtonLight->setText(QCoreApplication::translate("ChartThemeDlg", "light", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChartThemeDlg: public Ui_ChartThemeDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTTHEMEDLG_H
