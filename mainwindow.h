#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSplitter>
#include "Stracts/Stracts.h"
#include "ScrHashTable/HashTable.h"
#include "ScrMainAVLTree/MainTree.h"
#include"ChekersInputData/CheckerInputData.h"

class DebugWindow;
class MainTree;
class HashTable;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void updateDebugWindow();

private slots:
    void addCourier();
    void deleteCourier();
    void loadCouriers();
    void searchCourier();

    void addOrder();
    void deleteOrder();
    void loadOrders();
    void searchOrder();

    void onGenerateReportClicked();
    void showDebugWindow();

    void saveCouriersToFile();
    void saveOrdersToFile();

private:
    void setupUI();
    void logMessage(const QString& msg);
    void updateCourierTable();
    void updateOrderTable();
    bool initializeStructuresIfNeeded();
    void clearAllData();
    Filters FilterAssembly(QString& filterDate, QString& filterAddress, QString& filterFio);

    // GUI Elements
    QTableWidget* courierTable;
    QTableWidget* orderTable;
    QTextEdit* logArea;
    DebugWindow* debugWindow;

    // Backend Structures
    HashTable* hashTable;
    MainTree* mainTree;
    ReportTree* reportTree;
};

#endif 