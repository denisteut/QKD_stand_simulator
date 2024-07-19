/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QCustomPlot *widget;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QLineEdit *scanStepLine;
    QLineEdit *lineEditSecondPlateAcc;
    QLineEdit *BaseAngle_1;
    QPushButton *TestButton;
    QRadioButton *Alt_Method;
    QPushButton *zeroingAngles;
    QLineEdit *BaseAngle_3;
    QLineEdit *lineEditPlatesSteps;
    QLabel *label_3;
    QPushButton *OpenFile;
    QLabel *label_2;
    QLineEdit *numberOfIter;
    QLineEdit *lineEditSecondPlateBS;
    QPushButton *initButton;
    QComboBox *PlateNumber;
    QLineEdit *BaseAngle_2;
    QPushButton *testInit;
    QLabel *label_6;
    QLineEdit *maxRotateStep;
    QLabel *label;
    QLabel *label_5;
    QLineEdit *lineEditNumOfMeasures;
    QLineEdit *accuracyLine;
    QComboBox *init_ComboBox;
    QLabel *label_4;
    QPushButton *randButton;
    QLineEdit *BaseAngle_4;
    QProgressBar *progressTestBar;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1212, 738);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        widget = new QCustomPlot(centralwidget);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 981, 291));
        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(10, 310, 961, 335));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(gridLayoutWidget_2);
        pushButton->setObjectName("pushButton");

        gridLayout_2->addWidget(pushButton, 3, 0, 1, 1);

        scanStepLine = new QLineEdit(gridLayoutWidget_2);
        scanStepLine->setObjectName("scanStepLine");

        gridLayout_2->addWidget(scanStepLine, 1, 0, 1, 1);

        lineEditSecondPlateAcc = new QLineEdit(gridLayoutWidget_2);
        lineEditSecondPlateAcc->setObjectName("lineEditSecondPlateAcc");

        gridLayout_2->addWidget(lineEditSecondPlateAcc, 1, 3, 1, 1);

        BaseAngle_1 = new QLineEdit(gridLayoutWidget_2);
        BaseAngle_1->setObjectName("BaseAngle_1");

        gridLayout_2->addWidget(BaseAngle_1, 5, 1, 1, 1);

        TestButton = new QPushButton(gridLayoutWidget_2);
        TestButton->setObjectName("TestButton");
        TestButton->setEnabled(false);

        gridLayout_2->addWidget(TestButton, 9, 2, 1, 1);

        Alt_Method = new QRadioButton(gridLayoutWidget_2);
        Alt_Method->setObjectName("Alt_Method");

        gridLayout_2->addWidget(Alt_Method, 3, 1, 1, 1);

        zeroingAngles = new QPushButton(gridLayoutWidget_2);
        zeroingAngles->setObjectName("zeroingAngles");

        gridLayout_2->addWidget(zeroingAngles, 1, 2, 1, 1);

        BaseAngle_3 = new QLineEdit(gridLayoutWidget_2);
        BaseAngle_3->setObjectName("BaseAngle_3");

        gridLayout_2->addWidget(BaseAngle_3, 7, 1, 1, 1);

        lineEditPlatesSteps = new QLineEdit(gridLayoutWidget_2);
        lineEditPlatesSteps->setObjectName("lineEditPlatesSteps");

        gridLayout_2->addWidget(lineEditPlatesSteps, 9, 3, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_2);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 9, 0, 1, 1);

        OpenFile = new QPushButton(gridLayoutWidget_2);
        OpenFile->setObjectName("OpenFile");

        gridLayout_2->addWidget(OpenFile, 7, 2, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 6, 0, 1, 1);

        numberOfIter = new QLineEdit(gridLayoutWidget_2);
        numberOfIter->setObjectName("numberOfIter");

        gridLayout_2->addWidget(numberOfIter, 0, 3, 1, 1);

        lineEditSecondPlateBS = new QLineEdit(gridLayoutWidget_2);
        lineEditSecondPlateBS->setObjectName("lineEditSecondPlateBS");

        gridLayout_2->addWidget(lineEditSecondPlateBS, 3, 3, 1, 1);

        initButton = new QPushButton(gridLayoutWidget_2);
        initButton->setObjectName("initButton");

        gridLayout_2->addWidget(initButton, 3, 2, 1, 1);

        PlateNumber = new QComboBox(gridLayoutWidget_2);
        PlateNumber->addItem(QString());
        PlateNumber->addItem(QString());
        PlateNumber->addItem(QString());
        PlateNumber->addItem(QString());
        PlateNumber->setObjectName("PlateNumber");
        PlateNumber->setEditable(false);

        gridLayout_2->addWidget(PlateNumber, 0, 0, 1, 1);

        BaseAngle_2 = new QLineEdit(gridLayoutWidget_2);
        BaseAngle_2->setObjectName("BaseAngle_2");

        gridLayout_2->addWidget(BaseAngle_2, 6, 1, 1, 1);

        testInit = new QPushButton(gridLayoutWidget_2);
        testInit->setObjectName("testInit");

        gridLayout_2->addWidget(testInit, 6, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_2);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 7, 3, 1, 1);

        maxRotateStep = new QLineEdit(gridLayoutWidget_2);
        maxRotateStep->setObjectName("maxRotateStep");

        gridLayout_2->addWidget(maxRotateStep, 1, 1, 1, 1);

        label = new QLabel(gridLayoutWidget_2);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 5, 0, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_2);
        label_5->setObjectName("label_5");

        gridLayout_2->addWidget(label_5, 5, 3, 1, 1);

        lineEditNumOfMeasures = new QLineEdit(gridLayoutWidget_2);
        lineEditNumOfMeasures->setObjectName("lineEditNumOfMeasures");
        lineEditNumOfMeasures->setReadOnly(true);

        gridLayout_2->addWidget(lineEditNumOfMeasures, 6, 3, 1, 1);

        accuracyLine = new QLineEdit(gridLayoutWidget_2);
        accuracyLine->setObjectName("accuracyLine");

        gridLayout_2->addWidget(accuracyLine, 0, 1, 1, 1);

        init_ComboBox = new QComboBox(gridLayoutWidget_2);
        init_ComboBox->addItem(QString());
        init_ComboBox->addItem(QString());
        init_ComboBox->addItem(QString());
        init_ComboBox->addItem(QString());
        init_ComboBox->addItem(QString());
        init_ComboBox->setObjectName("init_ComboBox");

        gridLayout_2->addWidget(init_ComboBox, 5, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 7, 0, 1, 1);

        randButton = new QPushButton(gridLayoutWidget_2);
        randButton->setObjectName("randButton");

        gridLayout_2->addWidget(randButton, 0, 2, 1, 1);

        BaseAngle_4 = new QLineEdit(gridLayoutWidget_2);
        BaseAngle_4->setObjectName("BaseAngle_4");

        gridLayout_2->addWidget(BaseAngle_4, 9, 1, 1, 1);

        progressTestBar = new QProgressBar(gridLayoutWidget_2);
        progressTestBar->setObjectName("progressTestBar");
        progressTestBar->setValue(0);

        gridLayout_2->addWidget(progressTestBar, 10, 0, 1, 4);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1212, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "QKD Stand Sim", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Scan", nullptr));
        scanStepLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "Scan Step", nullptr));
        lineEditSecondPlateAcc->setPlaceholderText(QCoreApplication::translate("MainWindow", "2nd plate accuracy", nullptr));
        BaseAngle_1->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        TestButton->setText(QCoreApplication::translate("MainWindow", "Testbuttion", nullptr));
        Alt_Method->setText(QCoreApplication::translate("MainWindow", "Alternative Method", nullptr));
        zeroingAngles->setText(QCoreApplication::translate("MainWindow", "Set Angles to Zero", nullptr));
        BaseAngle_3->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Plate4", nullptr));
        OpenFile->setText(QCoreApplication::translate("MainWindow", "Open Results File", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Plate2", nullptr));
        numberOfIter->setPlaceholderText(QCoreApplication::translate("MainWindow", "Number of iterations", nullptr));
        lineEditSecondPlateBS->setPlaceholderText(QCoreApplication::translate("MainWindow", "2nd plate basestep", nullptr));
        initButton->setText(QCoreApplication::translate("MainWindow", "Init", nullptr));
        PlateNumber->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        PlateNumber->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        PlateNumber->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        PlateNumber->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));

        PlateNumber->setPlaceholderText(QCoreApplication::translate("MainWindow", "Plate", nullptr));
        BaseAngle_2->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        testInit->setText(QCoreApplication::translate("MainWindow", "Test Init", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "plates steps", nullptr));
        maxRotateStep->setText(QString());
        maxRotateStep->setPlaceholderText(QCoreApplication::translate("MainWindow", "Scan for", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Plate1", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "number of measures", nullptr));
        lineEditNumOfMeasures->setPlaceholderText(QCoreApplication::translate("MainWindow", "number of measures", nullptr));
        accuracyLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "Scan accuracy", nullptr));
        init_ComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Init1", nullptr));
        init_ComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Init2", nullptr));
        init_ComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Init3", nullptr));
        init_ComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Init4", nullptr));
        init_ComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Init5", nullptr));

        label_4->setText(QCoreApplication::translate("MainWindow", "Plate3", nullptr));
        randButton->setText(QCoreApplication::translate("MainWindow", "Randomize Angles", nullptr));
        BaseAngle_4->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        progressTestBar->setFormat(QCoreApplication::translate("MainWindow", "%v/%m", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
