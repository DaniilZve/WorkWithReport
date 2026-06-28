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
    void displayTree(BaseNode* root);
    void calculatePositions(BaseNode* node, int depth, int& x_counter, int x_spacing, int y_spacing, QMap<BaseNode*, QPointF>& positions);
    void drawTreeElements(BaseNode* node, const QMap<BaseNode*, QPointF>& positions);

    QTableWidget* hashTableWidget;
    QGraphicsView* treeView;
    QGraphicsScene* treeScene;
};

#endif 