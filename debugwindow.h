// debugwindow.h
#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QSplitter>
#include "ScrHashTable/HashTable.h"
#include "ScrMainAVLTree/MainTree.h"
#include "List/ListTree.h"

class DebugWindow : public QWidget {
    Q_OBJECT
public:
    DebugWindow(QWidget* parent = nullptr);
    void updateViews(HashTable* hTable, MainTree* mTree);

private:
    void drawTree(NodeMainTree* node, int x, int y, int hSpacing);

    QTableWidget* hashTableWidget;
    QGraphicsView* treeView;
    QGraphicsScene* treeScene;
};

#endif // DEBUGWINDOW_H