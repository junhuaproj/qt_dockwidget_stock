/********************************************************************************
** Form generated from reading UI file 'stockrecordlistdlg.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCKRECORDLISTDLG_H
#define UI_STOCKRECORDLISTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StockRecordListDlg
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *spinBoxLevel;
    QTextEdit *textEditContent;
    QLineEdit *lineEditDateTime;
    QLineEdit *lineEditTitle;
    QTableWidget *tableWidget;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StockRecordListDlg)
    {
        if (StockRecordListDlg->objectName().isEmpty())
            StockRecordListDlg->setObjectName(QString::fromUtf8("StockRecordListDlg"));
        StockRecordListDlg->resize(640, 480);
        gridLayoutWidget = new QWidget(StockRecordListDlg);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 10, 621, 461));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinBoxLevel = new QSpinBox(gridLayoutWidget);
        spinBoxLevel->setObjectName(QString::fromUtf8("spinBoxLevel"));

        gridLayout->addWidget(spinBoxLevel, 2, 1, 1, 1);

        textEditContent = new QTextEdit(gridLayoutWidget);
        textEditContent->setObjectName(QString::fromUtf8("textEditContent"));

        gridLayout->addWidget(textEditContent, 3, 1, 1, 1);

        lineEditDateTime = new QLineEdit(gridLayoutWidget);
        lineEditDateTime->setObjectName(QString::fromUtf8("lineEditDateTime"));

        gridLayout->addWidget(lineEditDateTime, 1, 1, 1, 1);

        lineEditTitle = new QLineEdit(gridLayoutWidget);
        lineEditTitle->setObjectName(QString::fromUtf8("lineEditTitle"));

        gridLayout->addWidget(lineEditTitle, 0, 1, 1, 1);

        tableWidget = new QTableWidget(gridLayoutWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        gridLayout->addWidget(tableWidget, 4, 0, 1, 2);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        buttonBox = new QDialogButtonBox(gridLayoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 2);

        gridLayout->setRowStretch(5, 1);

        retranslateUi(StockRecordListDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), StockRecordListDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StockRecordListDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(StockRecordListDlg);
    } // setupUi

    void retranslateUi(QDialog *StockRecordListDlg)
    {
        StockRecordListDlg->setWindowTitle(QCoreApplication::translate("StockRecordListDlg", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("StockRecordListDlg", "Title", nullptr));
        label_2->setText(QCoreApplication::translate("StockRecordListDlg", "DateTime", nullptr));
        label_3->setText(QCoreApplication::translate("StockRecordListDlg", "Level", nullptr));
        label_4->setText(QCoreApplication::translate("StockRecordListDlg", "Content", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StockRecordListDlg: public Ui_StockRecordListDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCKRECORDLISTDLG_H
