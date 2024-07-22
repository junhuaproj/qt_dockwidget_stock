/********************************************************************************
** Form generated from reading UI file 'stockrecorddlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCKRECORDDLG_H
#define UI_STOCKRECORDDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StockRecordDlg
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_3;
    QLineEdit *lineEditCondition;
    QLabel *label_4;
    QLineEdit *lineEditStock;
    QTextEdit *textEditContent;
    QLineEdit *lineEditDateTime;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEditKeyWord;
    QSpinBox *spinBoxLevel;
    QLabel *label_7;
    QLineEdit *lineEditListInfo;

    void setupUi(QDialog *StockRecordDlg)
    {
        if (StockRecordDlg->objectName().isEmpty())
            StockRecordDlg->setObjectName(QString::fromUtf8("StockRecordDlg"));
        StockRecordDlg->resize(640, 480);
        buttonBox = new QDialogButtonBox(StockRecordDlg);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 440, 621, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        gridLayoutWidget = new QWidget(StockRecordDlg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 20, 601, 401));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 1, 0, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 5, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 6, 0, 1, 1);

        lineEditCondition = new QLineEdit(gridLayoutWidget);
        lineEditCondition->setObjectName(QString::fromUtf8("lineEditCondition"));

        gridLayout->addWidget(lineEditCondition, 6, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        lineEditStock = new QLineEdit(gridLayoutWidget);
        lineEditStock->setObjectName(QString::fromUtf8("lineEditStock"));

        gridLayout->addWidget(lineEditStock, 1, 1, 1, 1);

        textEditContent = new QTextEdit(gridLayoutWidget);
        textEditContent->setObjectName(QString::fromUtf8("textEditContent"));

        gridLayout->addWidget(textEditContent, 4, 1, 1, 1);

        lineEditDateTime = new QLineEdit(gridLayoutWidget);
        lineEditDateTime->setObjectName(QString::fromUtf8("lineEditDateTime"));

        gridLayout->addWidget(lineEditDateTime, 5, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 4, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        lineEditKeyWord = new QLineEdit(gridLayoutWidget);
        lineEditKeyWord->setObjectName(QString::fromUtf8("lineEditKeyWord"));

        gridLayout->addWidget(lineEditKeyWord, 2, 1, 1, 1);

        spinBoxLevel = new QSpinBox(gridLayoutWidget);
        spinBoxLevel->setObjectName(QString::fromUtf8("spinBoxLevel"));

        gridLayout->addWidget(spinBoxLevel, 3, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 0, 0, 1, 1);

        lineEditListInfo = new QLineEdit(gridLayoutWidget);
        lineEditListInfo->setObjectName(QString::fromUtf8("lineEditListInfo"));

        gridLayout->addWidget(lineEditListInfo, 0, 1, 1, 1);


        retranslateUi(StockRecordDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), StockRecordDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StockRecordDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(StockRecordDlg);
    } // setupUi

    void retranslateUi(QDialog *StockRecordDlg)
    {
        StockRecordDlg->setWindowTitle(QCoreApplication::translate("StockRecordDlg", "Dialog", nullptr));
        label_5->setText(QCoreApplication::translate("StockRecordDlg", "code", nullptr));
        label_6->setText(QCoreApplication::translate("StockRecordDlg", "datetime", nullptr));
        label_3->setText(QCoreApplication::translate("StockRecordDlg", "condition", nullptr));
        label_4->setText(QCoreApplication::translate("StockRecordDlg", "level", nullptr));
        label->setText(QCoreApplication::translate("StockRecordDlg", "content", nullptr));
        label_2->setText(QCoreApplication::translate("StockRecordDlg", "key word", nullptr));
        label_7->setText(QCoreApplication::translate("StockRecordDlg", "list info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StockRecordDlg: public Ui_StockRecordDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCKRECORDDLG_H
