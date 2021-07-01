/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *codeLabel;
    QTextBrowser *showCode;
    QVBoxLayout *verticalLayout_4;
    QLabel *inputLable;
    QLineEdit *inputData;
    QVBoxLayout *verticalLayout_6;
    QLabel *currentVaribleLable;
    QTextBrowser *currentVarible_2;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QLabel *treeLable;
    QTextBrowser *showTree;
    QLabel *resultLable;
    QTextBrowser *showResult;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadButton_2;
    QPushButton *runbutton;
    QPushButton *clearButton;
    QPushButton *debug;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1004, 743);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        codeLabel = new QLabel(centralwidget);
        codeLabel->setObjectName(QString::fromUtf8("codeLabel"));

        verticalLayout->addWidget(codeLabel);

        showCode = new QTextBrowser(centralwidget);
        showCode->setObjectName(QString::fromUtf8("showCode"));

        verticalLayout->addWidget(showCode);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        inputLable = new QLabel(centralwidget);
        inputLable->setObjectName(QString::fromUtf8("inputLable"));

        verticalLayout_4->addWidget(inputLable);

        inputData = new QLineEdit(centralwidget);
        inputData->setObjectName(QString::fromUtf8("inputData"));

        verticalLayout_4->addWidget(inputData);


        gridLayout->addLayout(verticalLayout_4, 3, 0, 1, 2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        currentVaribleLable = new QLabel(centralwidget);
        currentVaribleLable->setObjectName(QString::fromUtf8("currentVaribleLable"));

        verticalLayout_6->addWidget(currentVaribleLable);

        currentVarible_2 = new QTextBrowser(centralwidget);
        currentVarible_2->setObjectName(QString::fromUtf8("currentVarible_2"));

        verticalLayout_6->addWidget(currentVarible_2);


        gridLayout->addLayout(verticalLayout_6, 1, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        treeLable = new QLabel(centralwidget);
        treeLable->setObjectName(QString::fromUtf8("treeLable"));

        verticalLayout_3->addWidget(treeLable);

        showTree = new QTextBrowser(centralwidget);
        showTree->setObjectName(QString::fromUtf8("showTree"));

        verticalLayout_3->addWidget(showTree);


        verticalLayout_2->addLayout(verticalLayout_3);

        resultLable = new QLabel(centralwidget);
        resultLable->setObjectName(QString::fromUtf8("resultLable"));

        verticalLayout_2->addWidget(resultLable);

        showResult = new QTextBrowser(centralwidget);
        showResult->setObjectName(QString::fromUtf8("showResult"));

        verticalLayout_2->addWidget(showResult);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 2, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        loadButton_2 = new QPushButton(centralwidget);
        loadButton_2->setObjectName(QString::fromUtf8("loadButton_2"));

        horizontalLayout->addWidget(loadButton_2);

        runbutton = new QPushButton(centralwidget);
        runbutton->setObjectName(QString::fromUtf8("runbutton"));

        horizontalLayout->addWidget(runbutton);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setAutoRepeatDelay(200);

        horizontalLayout->addWidget(clearButton);

        debug = new QPushButton(centralwidget);
        debug->setObjectName(QString::fromUtf8("debug"));

        horizontalLayout->addWidget(debug);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 2);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        codeLabel->setText(QCoreApplication::translate("MainWindow", "\344\273\243\347\240\201", nullptr));
        inputLable->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\344\273\243\347\240\201\350\241\214", nullptr));
        currentVaribleLable->setText(QCoreApplication::translate("MainWindow", "\345\275\223\345\211\215\345\217\230\351\207\217", nullptr));
        treeLable->setText(QCoreApplication::translate("MainWindow", "\350\257\255\345\217\245\344\270\216\350\257\255\346\263\225\346\240\221", nullptr));
        resultLable->setText(QCoreApplication::translate("MainWindow", "\350\277\220\350\241\214\347\273\223\346\236\234", nullptr));
        loadButton_2->setText(QCoreApplication::translate("MainWindow", "Load(\350\275\275\345\205\245\344\273\243\347\240\201)", nullptr));
        runbutton->setText(QCoreApplication::translate("MainWindow", "run(\346\211\247\350\241\214\344\273\243\347\240\201)", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "clear(\346\270\205\347\251\272\344\273\243\347\240\201)", nullptr));
        debug->setText(QCoreApplication::translate("MainWindow", "debug/step(\350\260\203\350\257\225/\345\215\225\346\255\245)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
