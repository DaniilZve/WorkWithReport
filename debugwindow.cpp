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
    if (hTable) {
        for (int i = 0; i < hTable->GetMaxSizeArr(); i++) {
            int row = hashTableWidget->rowCount();
            hashTableWidget->insertRow(row);

           
            auto cell = hTable->GetCellTable(i);

            // Базовые значения (статус выводим всегда, остальное по умолчанию "-")
            QString stateStr = QString::number(cell->state);
            QString primaryHashStr = "-";
            QString secondaryHashStr = QString::number(i); // Фактический индекс ячейки таблицы
            QString keyStr = "-";
            QString indexStr = "-";

            // Если статус OCCUPIED (1) или DELETED (2), то в ячейке есть осмысленные данные
            if (cell->state != 0 ) {
                // Первичный хеш вычисляем вручную: ключ % размер таблицы
                primaryHashStr = QString::number(cell->key % hTable->GetMaxSizeArr());
                keyStr = QString::number(cell->key).rightJustified(5, '0');
                if (cell->state == 1) indexStr = QString::number(cell->index);
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
        displayTree(mTree->root);
    }
}


void DebugWindow::displayTree(BaseNode* root) {
    treeScene->clear();
    if (!root) return;

    int x_counter = 0; // Глобальный счетчик шагов по горизонтали
    int y_spacing = 70; // Расстояние между уровнями (высота этажа)
    int x_spacing = 65; // Жесткий шаг между соседними по порядку узлами

    // Запускаем сначала только расчет координат для каждого узла
    // Чтобы нарисовать линии, родителю нужно знать точные координаты детей
    QMap<BaseNode*, QPointF> nodePositions;
    calculatePositions(root, 0, x_counter, x_spacing, y_spacing, nodePositions);

    // Теперь, когда координаты всех 100 элементов известны, рисуем их
    drawTreeElements(root, nodePositions);

    // Автоматически подгоняем размер сцены под получившееся дерево
    treeScene->setSceneRect(treeScene->itemsBoundingRect());
}

// Вспомогательный метод: рассчитывает координаты 
void DebugWindow::calculatePositions(BaseNode* node, int depth, int& x_counter,
    int x_spacing, int y_spacing,
    QMap<BaseNode*, QPointF>& positions) {
    if (!node) return;

    // Сначала уходим в самое левое поддерево
    calculatePositions(node->left, depth + 1, x_counter, x_spacing, y_spacing, positions);

    // Вычисляем уникальные координаты для текущего узла
    int currentX = x_counter * x_spacing;
    int currentY = depth * y_spacing;
    positions[node] = QPointF(currentX, currentY);

    x_counter++; // Сдвигаем счетчик для следующего по порядку элемента

    // Затем уходим в правое поддерево
    calculatePositions(node->right, depth + 1, x_counter, x_spacing, y_spacing, positions);
}

//  Вспомогательный метод: берет готовые координаты и строит графику
void DebugWindow::drawTreeElements(BaseNode* node, const QMap<BaseNode*, QPointF>& positions) {
    if (!node || !positions.contains(node)) return;

    QPointF currentPos = positions[node];
    int x = currentPos.x();
    int y = currentPos.y();

    // Отрисовка линий к детям
    if (node->left && positions.contains(node->left)) {
        QPointF leftPos = positions[node->left];
        treeScene->addLine(x, y, leftPos.x(), leftPos.y(), QPen(Qt::black, 1));
        drawTreeElements(node->left, positions);
    }
    if (node->right && positions.contains(node->right)) {
        QPointF rightPos = positions[node->right];
        treeScene->addLine(x, y, rightPos.x(), rightPos.y(), QPen(Qt::black, 1));
        drawTreeElements(node->right, positions);
    }

    // Формирование текста (Ключ + Цепочка индексов)
    QString nodeText = QString::number(node->GetKey()).rightJustified(5, '0') + "\n[";
    if (node->IndexList && node->IndexList->h) {
        NodeList* p = node->IndexList->h;
        do {
            nodeText += QString::number(p->Index) + " ";
            p = p->next;
        } while (p != node->IndexList->h);
    }
    nodeText += "]";

    // Отрисовка геометрии узла
    int radius = 33;
    // Делаем красивый контрастный круг: белая заливка, синяя рамка
    treeScene->addEllipse(x - radius, y - radius, radius * 2, radius * 2, QPen(QColor("#0055ff"), 2), QBrush(Qt::black));

    // Добавляем текст
    QGraphicsTextItem* text = treeScene->addText(nodeText);
    text->setDefaultTextColor(Qt::white);

    // Принудительно уменьшаем шрифт, чтобы строки не взрывали круги
    QFont font = text->font();
    font.setPointSize(7);
    font.setBold(true);
    text->setFont(font);

    // Выравниваем текст ровно по центру круга
    text->setPos(x - 22, y - 18);
}

