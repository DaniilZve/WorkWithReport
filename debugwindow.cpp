#include "debugwindow.h"
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include "dialogs.h"




DebugWindow::DebugWindow(QWidget* parent) : QWidget(parent, Qt::Window) {
    setWindowTitle("Окно отладки");
    resize(1000, 600);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    
    hashTableWidget = new QTableWidget(0, 5, this);
    hashTableWidget->setHorizontalHeaderLabels({
        "Статус",
        "Первичный хеш",
        "Вторичный хеш",
        "Ключ",
        "Индекс Массива"
        });
    splitter->addWidget(hashTableWidget);

    treeScene = new QGraphicsScene(this);
    treeView = new QGraphicsView(treeScene, this);
    splitter->addWidget(treeView);

    layout->addWidget(splitter);
}

void DebugWindow::updateViews(HashTable* hTable, MainTree* mTree) {
    // Обновление хэш таблицы
    hashTableWidget->setRowCount(0);
    if (hTable && hTable->Table) {
        for (size_t i = 0; i < hTable->MaxSizeArr; i++) {
            int row = hashTableWidget->rowCount();
            hashTableWidget->insertRow(row);

           
            auto cell = hTable->Table[i];

            // Базовые значения (статус выводим всегда, остальное по умолчанию "-")
            QString stateStr = QString::number(cell->state);
            QString primaryHashStr = "-";
            QString secondaryHashStr = QString::number(i); // Фактический индекс ячейки таблицы
            QString keyStr = "-";
            QString indexStr = "-";

            // Если статус OCCUPIED (1) или DELETED (2), то в ячейке есть осмысленные данные
            if (cell->state != 0 ) {
                // Первичный хеш вычисляем вручную: ключ % размер таблицы
                primaryHashStr = QString::number(cell->key % hTable->MaxSizeArr);
                keyStr = QString::number(cell->key).rightJustified(5, '0');
                indexStr = QString::number(cell->index);
            }

            // Заполняем строку таблицы
            hashTableWidget->setItem(row, 0, new QTableWidgetItem(stateStr));
            hashTableWidget->setItem(row, 1, new QTableWidgetItem(primaryHashStr));
            hashTableWidget->setItem(row, 2, new QTableWidgetItem(secondaryHashStr));
            hashTableWidget->setItem(row, 3, new QTableWidgetItem(keyStr));
            hashTableWidget->setItem(row, 4, new QTableWidgetItem(indexStr));
        }
    }

    // Обновление дерева
    treeScene->clear();
    if (mTree && mTree->root) {
        drawTree(mTree->root, 0, 0, 150);
    }
}

void DebugWindow::drawTree(NodeMainTree* node, int x, int y, int hSpacing) {
    if (!node) return;

    // Отрисовка связей
    if (node->left) {
        treeScene->addLine(x, y, x - hSpacing, y + 60);
        drawTree(node->left, x - hSpacing, y + 60, hSpacing / 2);
    }
    if (node->right) {
        treeScene->addLine(x, y, x + hSpacing, y + 60);
        drawTree(node->right, x + hSpacing, y + 60, hSpacing / 2);
    }

    // Формирование текста: Ключ + Цепочка индексов
    QString nodeText = QString::number(node->key).rightJustified(5, '0') + "\n[";
    NodeList* p = node->IndexList->h;
    if (p) {
        do {
            nodeText += QString::number(p->Index) + " ";
            p = p->next;
        } while (p != node->IndexList->h);
    }
    nodeText += "]";

    // Отрисовка узла
    QGraphicsEllipseItem* ellipse = treeScene->addEllipse(x - 25, y - 25, 50, 50, QPen(Qt::black), QBrush(Qt::black));
    QGraphicsTextItem* text = treeScene->addText(nodeText);
    text->setPos(x-20, y-20);
}

