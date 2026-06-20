#pragma once
#ifndef DIALOGS_H
#define DIALOGS_H
#include <QPlainTextEdit> 
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QDialog>
#include <QLineEdit>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include "Stracts/Stracts.h"
#include "ScrReportAVLTree/ReportTree.h"



class OrderInputDialog : public QDialog {
    Q_OBJECT
public:
    explicit OrderInputDialog(QWidget* parent = nullptr);
    
    // Возвращает строку в формате, полностью готовом для CheckInputOrder
    QString getInputString() const;

    // Поля ввода для детальной валидации
    QLineEdit* passEdit;
    QLineEdit* adresEdit;
    QLineEdit* dateEdit;
    QLineEdit* priceEdit;
};

// Диалоговое окно для ручного ввода курьера
class CourierInputDialog : public QDialog {
    Q_OBJECT
public:
    explicit CourierInputDialog(QWidget* parent = nullptr);
    QString getInputString() const; // Возвращает строку, готовую для парсинга через istringstream

private:
    QLineEdit* passEdit;
    QLineEdit* fioEdit;
    QLineEdit* tsEdit;
};

// Диалоговое окно ввода фильтров для отчета
class ReportFilterDialog : public QDialog {
    Q_OBJECT
public:
    explicit ReportFilterDialog(QWidget* parent = nullptr);
    QString getDateString() const;
    QString getAddressString() const;
    QString getFioString() const;

private:
    QLineEdit* dayEdit;
    QLineEdit* monthEdit;
    QLineEdit* yearEdit;
    QLineEdit* streetEdit;
    QLineEdit* houseEdit;
    QLineEdit* surnameEdit;
    QLineEdit* nameEdit;
    QLineEdit* patronymicEdit;
};



class ReportTree;

class ReportWindow : public QDialog {
    Q_OBJECT
public:
    explicit ReportWindow(ReportTree* reportTree, QWidget* parent = nullptr);
private:
    QPlainTextEdit* reportTextEdit; // Текстовое поле для вывода данных как в файле
};

#endif 