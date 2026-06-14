/********************************************************************************
** Form generated from reading UI file 'ReportsApplication.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTSAPPLICATION_H
#define UI_REPORTSAPPLICATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReportsApplicationClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ReportsApplicationClass)
    {
        if (ReportsApplicationClass->objectName().isEmpty())
            ReportsApplicationClass->setObjectName("ReportsApplicationClass");
        ReportsApplicationClass->resize(600, 400);
        menuBar = new QMenuBar(ReportsApplicationClass);
        menuBar->setObjectName("menuBar");
        ReportsApplicationClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ReportsApplicationClass);
        mainToolBar->setObjectName("mainToolBar");
        ReportsApplicationClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(ReportsApplicationClass);
        centralWidget->setObjectName("centralWidget");
        ReportsApplicationClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ReportsApplicationClass);
        statusBar->setObjectName("statusBar");
        ReportsApplicationClass->setStatusBar(statusBar);

        retranslateUi(ReportsApplicationClass);

        QMetaObject::connectSlotsByName(ReportsApplicationClass);
    } // setupUi

    void retranslateUi(QMainWindow *ReportsApplicationClass)
    {
        ReportsApplicationClass->setWindowTitle(QCoreApplication::translate("ReportsApplicationClass", "ReportsApplication", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ReportsApplicationClass: public Ui_ReportsApplicationClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTSAPPLICATION_H
