/********************************************************************************
** Form generated from reading UI file 'stockfilterdlg.ui'
**
** Created by: Qt User Interface Compiler version 6.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STOCKFILTERDLG_H
#define UI_STOCKFILTERDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StockFilterDlg
{
public:
    QVBoxLayout *verticalLayout_3;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *tabClipText;
    QVBoxLayout *verticalLayout;
    QPlainTextEdit *textEditJson;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBoxRemoveST;
    QPushButton *pushButtonOpenFile;
    QPushButton *pushButtonParse;
    QPushButton *pushButtonFromFile;
    QWidget *tabRecordList;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QDateEdit *dateEditEnd;
    QLineEdit *lineTitle;
    QLineEdit *lineContent;
    QCheckBox *checkBoxLevel;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QDateEdit *dateEditBegin;
    QTextEdit *textEdit_2;
    QLineEdit *lineKey;
    QCheckBox *checkBoxKey;
    QCheckBox *checkBoxDatetime;
    QCheckBox *checkBoxTitle;
    QLineEdit *lineLevel;
    QLineEdit *lineCondition;
    QCheckBox *checkBoxCondtion;
    QCheckBox *checkBoxContent;
    QHBoxLayout *horizontalLayout_btn;
    QRadioButton *radioList;
    QRadioButton *radioStock;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushSearch;
    QTableWidget *tableWidget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonDelete;
    QPushButton *pushFilter;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *StockFilterDlg)
    {
        if (StockFilterDlg->objectName().isEmpty())
            StockFilterDlg->setObjectName("StockFilterDlg");
        StockFilterDlg->resize(842, 606);
        verticalLayout_3 = new QVBoxLayout(StockFilterDlg);
        verticalLayout_3->setObjectName("verticalLayout_3");
        splitter = new QSplitter(StockFilterDlg);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName("tabWidget");
        tabClipText = new QWidget();
        tabClipText->setObjectName("tabClipText");
        verticalLayout = new QVBoxLayout(tabClipText);
        verticalLayout->setObjectName("verticalLayout");
        textEditJson = new QPlainTextEdit(tabClipText);
        textEditJson->setObjectName("textEditJson");

        verticalLayout->addWidget(textEditJson);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        checkBoxRemoveST = new QCheckBox(tabClipText);
        checkBoxRemoveST->setObjectName("checkBoxRemoveST");

        horizontalLayout_4->addWidget(checkBoxRemoveST);


        verticalLayout->addLayout(horizontalLayout_4);

        pushButtonOpenFile = new QPushButton(tabClipText);
        pushButtonOpenFile->setObjectName("pushButtonOpenFile");

        verticalLayout->addWidget(pushButtonOpenFile);

        pushButtonParse = new QPushButton(tabClipText);
        pushButtonParse->setObjectName("pushButtonParse");

        verticalLayout->addWidget(pushButtonParse);

        pushButtonFromFile = new QPushButton(tabClipText);
        pushButtonFromFile->setObjectName("pushButtonFromFile");

        verticalLayout->addWidget(pushButtonFromFile);

        tabWidget->addTab(tabClipText, QString());
        tabRecordList = new QWidget();
        tabRecordList->setObjectName("tabRecordList");
        gridLayout = new QGridLayout(tabRecordList);
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(tabRecordList);
        label_2->setObjectName("label_2");
        label_2->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_2->addWidget(label_2);

        dateEditEnd = new QDateEdit(tabRecordList);
        dateEditEnd->setObjectName("dateEditEnd");
        dateEditEnd->setMinimumSize(QSize(0, 20));

        horizontalLayout_2->addWidget(dateEditEnd);


        gridLayout->addLayout(horizontalLayout_2, 9, 1, 1, 1);

        lineTitle = new QLineEdit(tabRecordList);
        lineTitle->setObjectName("lineTitle");

        gridLayout->addWidget(lineTitle, 3, 1, 1, 1);

        lineContent = new QLineEdit(tabRecordList);
        lineContent->setObjectName("lineContent");

        gridLayout->addWidget(lineContent, 5, 1, 1, 1);

        checkBoxLevel = new QCheckBox(tabRecordList);
        checkBoxLevel->setObjectName("checkBoxLevel");

        gridLayout->addWidget(checkBoxLevel, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(tabRecordList);
        label->setObjectName("label");
        label->setMaximumSize(QSize(80, 20));

        horizontalLayout->addWidget(label);

        dateEditBegin = new QDateEdit(tabRecordList);
        dateEditBegin->setObjectName("dateEditBegin");
        dateEditBegin->setMinimumSize(QSize(0, 20));
        dateEditBegin->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(dateEditBegin);


        gridLayout->addLayout(horizontalLayout, 8, 1, 1, 1);

        textEdit_2 = new QTextEdit(tabRecordList);
        textEdit_2->setObjectName("textEdit_2");

        gridLayout->addWidget(textEdit_2, 10, 0, 1, 2);

        lineKey = new QLineEdit(tabRecordList);
        lineKey->setObjectName("lineKey");

        gridLayout->addWidget(lineKey, 6, 1, 1, 1);

        checkBoxKey = new QCheckBox(tabRecordList);
        checkBoxKey->setObjectName("checkBoxKey");

        gridLayout->addWidget(checkBoxKey, 6, 0, 1, 1);

        checkBoxDatetime = new QCheckBox(tabRecordList);
        checkBoxDatetime->setObjectName("checkBoxDatetime");

        gridLayout->addWidget(checkBoxDatetime, 8, 0, 2, 1);

        checkBoxTitle = new QCheckBox(tabRecordList);
        checkBoxTitle->setObjectName("checkBoxTitle");

        gridLayout->addWidget(checkBoxTitle, 3, 0, 1, 1);

        lineLevel = new QLineEdit(tabRecordList);
        lineLevel->setObjectName("lineLevel");

        gridLayout->addWidget(lineLevel, 4, 1, 1, 1);

        lineCondition = new QLineEdit(tabRecordList);
        lineCondition->setObjectName("lineCondition");

        gridLayout->addWidget(lineCondition, 7, 1, 1, 1);

        checkBoxCondtion = new QCheckBox(tabRecordList);
        checkBoxCondtion->setObjectName("checkBoxCondtion");

        gridLayout->addWidget(checkBoxCondtion, 7, 0, 1, 1);

        checkBoxContent = new QCheckBox(tabRecordList);
        checkBoxContent->setObjectName("checkBoxContent");

        gridLayout->addWidget(checkBoxContent, 5, 0, 1, 1);

        horizontalLayout_btn = new QHBoxLayout();
        horizontalLayout_btn->setObjectName("horizontalLayout_btn");
        radioList = new QRadioButton(tabRecordList);
        radioList->setObjectName("radioList");
        radioList->setChecked(true);

        horizontalLayout_btn->addWidget(radioList);

        radioStock = new QRadioButton(tabRecordList);
        radioStock->setObjectName("radioStock");

        horizontalLayout_btn->addWidget(radioStock);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_btn->addItem(horizontalSpacer_2);

        pushSearch = new QPushButton(tabRecordList);
        pushSearch->setObjectName("pushSearch");

        horizontalLayout_btn->addWidget(pushSearch);


        gridLayout->addLayout(horizontalLayout_btn, 1, 0, 1, 2);

        tabWidget->addTab(tabRecordList, QString());
        splitter->addWidget(tabWidget);
        tableWidget = new QTableWidget(splitter);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        splitter->addWidget(tableWidget);
        tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        tableWidget->verticalHeader()->setVisible(true);

        verticalLayout_3->addWidget(splitter);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        pushButtonDelete = new QPushButton(StockFilterDlg);
        pushButtonDelete->setObjectName("pushButtonDelete");

        horizontalLayout_3->addWidget(pushButtonDelete);

        pushFilter = new QPushButton(StockFilterDlg);
        pushFilter->setObjectName("pushFilter");

        horizontalLayout_3->addWidget(pushFilter);


        verticalLayout_3->addLayout(horizontalLayout_3);

        buttonBox = new QDialogButtonBox(StockFilterDlg);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setMinimumSize(QSize(80, 40));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(StockFilterDlg);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, StockFilterDlg, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, StockFilterDlg, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StockFilterDlg);
    } // setupUi

    void retranslateUi(QDialog *StockFilterDlg)
    {
        StockFilterDlg->setWindowTitle(QCoreApplication::translate("StockFilterDlg", "Dialog", nullptr));
        checkBoxRemoveST->setText(QCoreApplication::translate("StockFilterDlg", "\345\214\205\346\213\254ST", nullptr));
        pushButtonOpenFile->setText(QCoreApplication::translate("StockFilterDlg", "OpenFile", nullptr));
        pushButtonParse->setText(QCoreApplication::translate("StockFilterDlg", "Parse", nullptr));
        pushButtonFromFile->setText(QCoreApplication::translate("StockFilterDlg", "Parse Table", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabClipText), QCoreApplication::translate("StockFilterDlg", "\347\262\230\350\264\264\346\226\207\346\234\254", nullptr));
        label_2->setText(QCoreApplication::translate("StockFilterDlg", "\346\234\200\345\220\216", nullptr));
        checkBoxLevel->setText(QCoreApplication::translate("StockFilterDlg", "Level", nullptr));
        label->setText(QCoreApplication::translate("StockFilterDlg", "\346\234\200\346\227\251", nullptr));
        checkBoxKey->setText(QCoreApplication::translate("StockFilterDlg", "key", nullptr));
        checkBoxDatetime->setText(QCoreApplication::translate("StockFilterDlg", "Datetime", nullptr));
        checkBoxTitle->setText(QCoreApplication::translate("StockFilterDlg", "Title", nullptr));
        checkBoxCondtion->setText(QCoreApplication::translate("StockFilterDlg", "condtion", nullptr));
        checkBoxContent->setText(QCoreApplication::translate("StockFilterDlg", "Content", nullptr));
        radioList->setText(QCoreApplication::translate("StockFilterDlg", "List", nullptr));
        radioStock->setText(QCoreApplication::translate("StockFilterDlg", "Stock", nullptr));
        pushSearch->setText(QCoreApplication::translate("StockFilterDlg", "Search", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabRecordList), QCoreApplication::translate("StockFilterDlg", "\344\277\235\345\255\230\347\232\204\345\210\227\350\241\250", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("StockFilterDlg", "Delete", nullptr));
        pushFilter->setText(QCoreApplication::translate("StockFilterDlg", "Filter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StockFilterDlg: public Ui_StockFilterDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STOCKFILTERDLG_H
