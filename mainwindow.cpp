#include "mainwindow.h"
#include "debugwindow.h"
#include "dialogs.h" 
#include"ChekersInputData/CheckerInputData.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <sstream>



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), hashTable(nullptr), mainTree(nullptr), reportTree(nullptr), debugWindow(nullptr) {
    setupUI();
}

MainWindow::~MainWindow() {
    if (reportTree) delete reportTree;
    if (mainTree) delete mainTree;
    if (hashTable) delete hashTable;
}

void MainWindow::setupUI() {
    setWindowTitle("Справочная система: Курьеры и Заказы");
    resize(1200, 800);

    // Основной виджет и сплиттер
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    // Таблица Курьеров
    courierTable = new QTableWidget(0, 6, this);
    courierTable->setHorizontalHeaderLabels({ "Пропуск", "Фамилия", "Имя", "Отчество", "Марка ТС", "Модель ТС" });
    courierTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    splitter->addWidget(courierTable);

    // Таблица Заказов
    orderTable = new QTableWidget(0, 7, this);
    orderTable->setHorizontalHeaderLabels({ "Пропуск", "Улица", "Дом", "День", "Месяц", "Год", "Стоимость" });
    orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    splitter->addWidget(orderTable);

    mainLayout->addWidget(splitter, 4);

    // Лог событий
    logArea = new QTextEdit(this);
    logArea->setReadOnly(true);
    mainLayout->addWidget(logArea, 1);

    // Меню
    QMenu* courierMenu = menuBar()->addMenu("Курьеры");
    courierMenu->addAction("Добавление", this, &MainWindow::addCourier);
    courierMenu->addAction("Удаление", this, &MainWindow::deleteCourier);
    courierMenu->addAction("Загрузка из файла", this, &MainWindow::loadCouriers);
    courierMenu->addAction("Сохранение в файл", this, &MainWindow::saveCouriersToFile);
    courierMenu->addAction("Поиск", this, &MainWindow::searchCourier);

    QMenu* orderMenu = menuBar()->addMenu("Заказы");
    orderMenu->addAction("Добавление", this, &MainWindow::addOrder);
    orderMenu->addAction("Удаление", this, &MainWindow::deleteOrder);
    orderMenu->addAction("Загрузка из файла", this, &MainWindow::loadOrders);
    orderMenu->addAction("Сохранение в файл", this, &MainWindow::saveOrdersToFile);
    orderMenu->addAction("Поиск", this, &MainWindow::searchOrder);

    menuBar()->addAction("Формирование отчёта", this, &MainWindow::onGenerateReportClicked);
    menuBar()->addAction("Отладка", this, &MainWindow::showDebugWindow);
}

void MainWindow::logMessage(const QString& msg) {
    logArea->append(msg);
}

                    

bool MainWindow::initializeStructuresIfNeeded() {
    if (!hashTable) {
        bool ok;
        size_t size = QInputDialog::getInt(this, "Инициализация", "Введите начальный размер справочника (для хеш-таблицы). \nВведённый размер должен являться степенью двойки (2;4;8;16 и т.д):", 10, 1, 10000, 1, &ok);
        if (!ok) return false;
        if (!isPowerOfTwo(size))
        {
            QMessageBox::critical(this, "Ошибка", "Введённый размер не является степенью двойки.");
            return false;
        }
        hashTable = new HashTable(size);
        mainTree = new MainTree(hashTable);
        reportTree = new ReportTree(mainTree, hashTable);
        mainTree->SetReportTree(reportTree);
        logMessage("Внутренние структуры инициализированы размером: " + QString::number(size));
    }
    return true;
}

void MainWindow::clearAllData() {
    if (mainTree) { delete mainTree; mainTree = nullptr; }
    if (hashTable) { delete hashTable; hashTable = nullptr; }
    if (reportTree) { delete reportTree; reportTree = nullptr; }
    courierTable->setRowCount(0);
    orderTable->setRowCount(0);
}

void MainWindow::updateDebugWindow() {
    if (debugWindow && debugWindow->isVisible()) {
        debugWindow->updateViews(hashTable, mainTree);
    }
}




// Обновление КУРЬЕРОВ 
void MainWindow::updateCourierTable() {

    courierTable->setRowCount(0);
    if (!hashTable) return;
    for (int i = 0; i < hashTable->GetMaxSizeArr(); i++) {
        if (hashTable->GetCellCourierArr(i) != nullptr) {
            int row = courierTable->rowCount();
            courierTable->insertRow(row);

            courierTable->setItem(row, 0, new QTableWidgetItem(QString::number(hashTable->GetCellCourierArr(i)->passNum).rightJustified(5, '0')));
            courierTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.f)));
            courierTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.i)));
            courierTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.o)));
            courierTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(hashTable->GetCellCourierArr(i)->transp.brand)));
            courierTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(hashTable->GetCellCourierArr(i)->transp.model)));
            
        }
    }
}

// Действия в справочнике КУРЬЕРЫ


// Добавить курьера
void MainWindow::addCourier() {
    if (!initializeStructuresIfNeeded()) return;

    CourierInputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        std::istringstream iss(dlg.getInputString().toStdString());
        auto result = CheckInputCourier(iss);

        if (result.second.empty()) {
            
            std::pair <bool, int> resultAdd = hashTable->AddInTable(result.first);
            if (resultAdd.first == true)
            {
                updateCourierTable();
                updateDebugWindow();
                logMessage(QString("Курьер с пропуском %1 успешно добавлен вручную.").arg(result.first.passNum));
            }
            else
            {
                if (resultAdd.second == NO_CELL) QMessageBox::critical(this, "Ошибка", "Коэффициент заполненности слишком высок, из-за чего не удалось найти место вставки.");
                else QMessageBox::critical(this, "Ошибка", "Курьер с таким номером пропуска уже есть в справочнике.");

                logMessage(QString("Курьера с пропуском %1 не удалось добавить.").arg(result.first.passNum));
            }
            
        }
        else {
            QMessageBox::critical(this, "Ошибка", QString::fromStdString(result.second));
            logMessage(QString("Курьера не удалось добавить. Вводимые данные были не корректны").arg(result.first.passNum));
        }
    }
}

// Удалить курьера
void MainWindow::deleteCourier() {
    if (!hashTable || hashTable->GetNumElem() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник курьеров пуст.");
        return;
    }

    CourierInputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        std::istringstream iss(dlg.getInputString().toStdString());
        auto result = CheckInputCourier(iss); 

        if (result.second.empty()) {

            // Проверка на связанные данные
            if (mainTree && mainTree->SearchInTree(result.first.passNum).first != nullptr) {
                QMessageBox::critical(this, "Ошибка", "Нельзя удалить курьера: есть связанные данные в справочнике 'Заказы'.");
                return;
            }
            if (hashTable->DelInTable(result.first))
            {
               
                    updateCourierTable();
                    updateDebugWindow();
                    logMessage(QString("Курьер с пропуском %1 удален.").arg(result.first.passNum));
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Курьер не найден.");;
                logMessage(QString("Курьера с пропуском %1 не удалось удалить.").arg(result.first.passNum));
            }
        }
        else {
            QMessageBox::critical(this, "Ошибка", QString::fromStdString(result.second));
            logMessage(QString("Курьера не удалось удалить. Вводимые данные были не корректны").arg(result.first.passNum));
        }
    }
}


// Загрузить курьеров 
void MainWindow::loadCouriers() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл курьеров", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    // Перезапись при повторной загрузке
    if (hashTable) clearAllData();
    if (!initializeStructuresIfNeeded()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для чтения.");
        logMessage("Не удалось загрузить данные о курьерах из текстового файла");
        return;
    }
    QTextStream in(&file);
    int lineNum = 1;
    bool hasError = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) { lineNum++; continue; }

        std::istringstream iss(line.toStdString());
        auto result = CheckInputCourier(iss);

        if (result.second.empty()) {
            
            std::pair <bool, int> resultAdd = hashTable->AddInTable(result.first);
            if ( resultAdd.first)
            {
               
                int row = courierTable->rowCount();
                courierTable->insertRow(row);
                
                courierTable->setItem(row, 0, new QTableWidgetItem(QString::number(result.first.passNum).rightJustified(5, '0')));
                courierTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(result.first.fio.f)));
                courierTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(result.first.fio.i)));
                courierTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(result.first.fio.o)));
                courierTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(result.first.transp.brand)));
                courierTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(result.first.transp.model)));
            }
            else
            {
                if (resultAdd.second == NO_CELL) QMessageBox::critical(this, "Ошибка загрузки", QString("Коэффициент заполненности слишком высок, из-за чего не удалось найти место вставки. Загрузка остановлена на строке %1").arg(lineNum));
                else QMessageBox::critical(this, "Ошибка загрузки", QString("Очередной курьер уже есть в справочнике. Загрузка остановлена на строке %1").arg(lineNum));
                hasError = true;
                break;
            }
        }
        else {
            QMessageBox::critical(this, "Ошибка загрузки", QString("Ошибка в строке %1. Загрузка остановлена.").arg(lineNum));
            hasError = true;
            break;
        }
        lineNum++;
    }

    if (hasError)
    {
        logMessage("Не удалось загрузить данные о курьерах из текстового файла");
        clearAllData(); // Отмена действия при ошибке
    }
    else logMessage("Данные о курьерах успешно загружены из текстового файла.");

    updateDebugWindow();
}

// Сохранение курьеров в текстовый файл
void MainWindow::saveCouriersToFile() {
    if (!hashTable || hashTable->GetNumElem() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник курьеров пуст. Нечего сохранять.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл курьеров", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logMessage("Не удалось сохранить данные о курьерах в текстовый файл.");
        return;
    }

    QTextStream out(&file);
    // Обходим внутренний массив хеш-таблицы
    for (int i = 0; i < hashTable->GetMaxSizeArr(); i++) {
        if (hashTable->GetCellCourierArr(i) != nullptr) {
            out << QString::number(hashTable->GetCellCourierArr(i)->passNum).rightJustified(5, '0') << " "
                << QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.f) << " "
                << QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.i) << " "
                << QString::fromStdString(hashTable->GetCellCourierArr(i)->fio.o) << " "
                << QString::fromStdString(hashTable->GetCellCourierArr(i)->transp.brand) << " "
                << QString::fromStdString(hashTable->GetCellCourierArr(i)->transp.model) << "\n";
            
        }
    }

    file.close();
    logMessage("Данные о курьерах успешно сохранены в текстовый файл: " + fileName);
}

//Найти курьера 
void MainWindow::searchCourier() {
    if (!hashTable || hashTable->GetNumElem() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник курьеров пуст.");
        return;
    }


    bool ok;
    std::string passStr = QInputDialog::getText(this, "Поиск", "Введите номер пропуска:", QLineEdit::Normal, "", &ok).toStdString();
    if (!ok) return;

    if (isNatural(passStr) && (passStr.length() == 5))
    {
        unsigned int passNam = stoi(passStr);
        std::pair <Cell*, int> cell = hashTable->SearchInHashTable(passNam);
        if (cell.first) {
            DataCourier* c = hashTable->GetCellCourierArr(cell.first->index);
            QString info = QString("Найден курьер!\nПропуск: %1\nФИО: %2 %3 %4\nТС: %5 %6")
                .arg(c->passNum).arg(c->fio.f.c_str()).arg(c->fio.i.c_str()).arg(c->fio.o.c_str())
                .arg(c->transp.brand.c_str()).arg(c->transp.model.c_str());
            QMessageBox::information(this, "Успешный поиск", info);
            logMessage(QString("Поиск курьера %1 прошел успешно. Количество шагов: %2").arg(stoi(passStr)).arg(cell.second));
        }
        else {
            QMessageBox::warning(this, "Результат", "Элемент не найден.");
            logMessage(QString("Поиск курьера %1 не удался. Количество шагов: %2").arg(stoi(passStr)).arg(cell.second));
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", QString::fromStdString("Ошибка в записи номера пропуска.Проверьте, чтобы длинна была равна 5, а сам номер был натуральным числом"));
        logMessage(QString("Курьера не удалось найти. Ошибка записи номера пропуска."));
    }
}




// Обновление ЗАКАЗОВ
void MainWindow::updateOrderTable() {

    orderTable->setRowCount(0);
    if (!mainTree) return;

    // Идем по всему массиву указателей структуры данных AVL-дерева
    for (int i = 0; i < mainTree->GetCapasityOrdersArr(); i++) {
        if (mainTree->GetCellOrderArr(i) != nullptr) {
            int row = orderTable->rowCount();
            orderTable->insertRow(row);
           
            orderTable->setItem(row, 0, new QTableWidgetItem(QString::number(mainTree->GetCellOrderArr(i)->passNum).rightJustified(5, '0')));

            QString dateStr = QString("%1 %2 %3")
                .arg(mainTree->GetCellOrderArr(i)->date.d)
                .arg(QString::fromStdString(mainTree->GetCellOrderArr(i)->date.m))
                .arg(mainTree->GetCellOrderArr(i)->date.y);

            orderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(mainTree->GetCellOrderArr(i)->adres.street)));
            orderTable->setItem(row, 2, new QTableWidgetItem(QString::number(mainTree->GetCellOrderArr(i)->adres.house)));

            orderTable->setItem(row, 3, new QTableWidgetItem(QString::number(mainTree->GetCellOrderArr(i)->date.d)));
            orderTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(mainTree->GetCellOrderArr(i)->date.m)));
            orderTable->setItem(row, 5, new QTableWidgetItem(QString::number(mainTree->GetCellOrderArr(i)->date.y)));

            
            orderTable->setItem(row, 6, new QTableWidgetItem(QString::number(mainTree->GetCellOrderArr(i)->price, 'f', 2)));
        }
    }
}

// Действия в справочнике Заказы

// Добавить заказ
void MainWindow::addOrder() {
    if (!hashTable) {
        QMessageBox::critical(this, "Ошибка", "Невозможно добавить заказ: справочник 'Курьеры' еще не инициализирован.");
        return;
    }

    OrderInputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        std::istringstream iss(dlg.getInputString().toStdString());
        auto result = CheckInputOrder(iss); 

        if (result.second.empty()) {

            // Проверка: Курьер должен существовать в Хэш-таблице
            if (hashTable->SearchInHashTable(result.first.passNum).first == nullptr) {
                QMessageBox::critical(this, "Ошибка целостности", "Действие отменено: курьера с таким номером пропуска не существует!");
                logMessage(QString("Добавление заказа отменено: курьер %1 отсутствует.").arg(result.first.passNum));
                return;
            }

            mainTree->AddElemInArr(result.first);
            updateOrderTable();
            updateDebugWindow();
            logMessage(QString("Заказ курьера %1 успешно добавлен вручную.").arg(result.first.passNum));
            
            
        }
        else {
            QMessageBox::critical(this, "Ошибка", QString::fromStdString(result.second));
            logMessage(QString("Заказ курьера не удалось добавить.Вводимые данные были не корректны").arg(result.first.passNum));
        }
    }
}


// Удалить заказ
void MainWindow::deleteOrder() {
    if (!mainTree || mainTree->GetNumElemOrdersArr() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник заказов пуст.");
        return;
    }

    OrderInputDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        std::istringstream iss(dlg.getInputString().toStdString());
        auto result = CheckInputOrder(iss); 

        if (result.second.empty()) {
            if (mainTree->DelElemInArr(result.first))
            {
                updateOrderTable();
                updateDebugWindow();
                logMessage(QString("Заказ курьера %1 удалён.").arg(result.first.passNum));
            }
            else
            {
                QMessageBox::critical(this, "Ошибка", "Введённый заказ не найден");
                logMessage(QString("Заказ курьера %1 не удалось удалить.").arg(result.first.passNum));
            }
        }
        else {
            QMessageBox::critical(this, "Ошибка", QString::fromStdString(result.second));
            logMessage(QString("Заказ курьера не удалось удалить.Вводимые данные были не корректны").arg(result.first.passNum));
        }
    }
}


// Загрузить заказы
void MainWindow::loadOrders() {
    if (!hashTable) {
        QMessageBox::critical(this, "Ошибка", "Сначала загрузите или заполните справочник курьеров!");
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл заказов", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    // При повторном вызове данные справочника полностью перезаписываются
    if (mainTree) {
        delete mainTree;
        delete reportTree;
        mainTree = new MainTree(hashTable);
        reportTree = new ReportTree(mainTree, hashTable);
        mainTree->SetReportTree(reportTree);
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для чтения.");
        logMessage("Не удалось загрузить данные о заказах из текстового файла");
        return;
    }

    QTextStream in(&file);
    int lineNum = 1;
    bool hasError = false;
    QString errorMsg = "";

    orderTable->setRowCount(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty()) { lineNum++; continue; }

        std::istringstream iss(line.toStdString());
        auto result = CheckInputOrder(iss); 

        if (!result.second.empty()) {
            errorMsg = QString("Некорректный формат данных в строке %1.").arg(lineNum);
            hasError = true;
            break;
        }

        // Проверка: присутствует ли курьер в хеш-таблице
        if (hashTable->SearchInHashTable(result.first.passNum).first == nullptr) {
            errorMsg = QString("Строка %1: Курьера с пропуском %2 нет в справочнике \"Курьеры\".").arg(lineNum).arg(result.first.passNum);
            hasError = true;
            break;
        }


        
        // Добавляем в AVL-дерево
        mainTree->AddElemInArr(result.first);
        

            int row = orderTable->rowCount();
            orderTable->insertRow(row);

            orderTable->setItem(row, 0, new QTableWidgetItem(QString::number(result.first.passNum).rightJustified(5, '0')));
            orderTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(result.first.adres.street)));
            orderTable->setItem(row, 2, new QTableWidgetItem(QString::number(result.first.adres.house)));
            orderTable->setItem(row, 3, new QTableWidgetItem(QString::number(result.first.date.d)));
            orderTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(result.first.date.m)));
            orderTable->setItem(row, 5, new QTableWidgetItem(QString::number(result.first.date.y)));
            orderTable->setItem(row, 6, new QTableWidgetItem(QString::number(result.first.price, 'f', 2)));


            lineNum++;
        
        
    }

    if (hasError) {
        // Если данные некорректны, загрузка полностью останавливается, действие отменяется
        QMessageBox::critical(this, "Ошибка загрузки файлов", errorMsg + "\nЗагрузка прервана. Данные аннулированы.");
        delete mainTree;
        mainTree = new MainTree(hashTable);
        orderTable->setRowCount(0);
        logMessage("Не удалось загрузить данные о заказах из текстового файла.");
    }
    else {
        logMessage("Данные о заказах успешно загружены из текстового файла.");
    }

    updateDebugWindow();
}

// Сохранение заказов в текстовый файл
void MainWindow::saveOrdersToFile() {
    if (!mainTree || mainTree->GetNumElemOrdersArr() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник заказов пуст. Нечего сохранять.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл заказов", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        logMessage("Не удалось сохранить данные о заказах в текстовый файл.");
        return;
    }

    QTextStream out(&file);
    // Обходим массив указателей AVL-дерева
    for (int i = 0; i < mainTree->GetNumElemOrdersArr(); i++) {
        if (mainTree->GetCellOrderArr(i) != nullptr) {
            out << QString::number(mainTree->GetCellOrderArr(i)->passNum).rightJustified(5, '0') << " "
                << QString::fromStdString(mainTree->GetCellOrderArr(i)->adres.street) << " "
                << mainTree->GetCellOrderArr(i)->adres.house << " "
                << mainTree->GetCellOrderArr(i)->date.d << " "
                << QString::fromStdString(mainTree->GetCellOrderArr(i)->date.m) << " "
                << mainTree->GetCellOrderArr(i)->date.y << " "
                // Ограничиваем стоимость 2 знаками после запятой для красивой структуры
                << QString::number(mainTree->GetCellOrderArr(i)->price, 'f', 2) << "\n";
        }
    }

    file.close();
    logMessage("Данные о заказах успешно сохранены в текстовый файл: " + fileName);
}

// Найти заказ
void MainWindow::searchOrder() {
    if (!mainTree || mainTree->GetNumElemOrdersArr() == 0) {
        QMessageBox::warning(this, "Внимание", "Справочник заказов пуст.");
        return;
    }

    bool ok;
    std::string passStr = QInputDialog::getText(this, "Поиск", "Введите номер пропуска:", QLineEdit::Normal, "", &ok).toStdString();

    if (!ok) return;

   
    if (isNatural(passStr) && (passStr.length() == 5))
    {

        std::pair <BaseNode*, int> Node = mainTree->SearchInTree(stoi(passStr));
        if (Node.first) {
            NodeList* c = Node.first->IndexList->h;

            QString info = QString("Найдены заказы курьера!\nПропуск: %1\n")
                .arg(Node.first->GetKey());
            do {
                c = c->next;
                DataOrder* FoundOrder = mainTree->GetCellOrderArr(c->Index);
                info += QString("%1 %2 %3 %4 %5 %6 %7\n").arg(FoundOrder->passNum)
                    .arg(FoundOrder->adres.street).arg(FoundOrder->adres.house)
                    .arg(FoundOrder->date.d).arg(FoundOrder->date.m).arg(FoundOrder->date.y)
                    .arg(FoundOrder->price);

            } while (c != Node.first->IndexList->h);

            QMessageBox::information(this, "Успешный поиск", info);
            logMessage(QString("Поиск заказа %1 прошел успешно. Количество шагов: %2").arg(stoi(passStr)).arg(Node.second));
        }
        else {
            QMessageBox::warning(this, "Результат", "Элементы не найдены.");
            logMessage(QString("Заказ %1 не удалось найти. Количество шагов: %2").arg(stoi(passStr)).arg(Node.second));
        }
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", QString::fromStdString("Ошибка в записи номера пропуска.Проверьте, чтобы длинна была равна 5, а сам номер был натуральным числом"));
        logMessage(QString("Заказ не удалось найти. Ошибка записи номера пропуска."));
    }
}



Filters MainWindow::FilterAssembly(QString& filterDate, QString& filterAddress, QString& filterFio)
{

    //Собираем структуру Date (из строки вида "13 jun 2026")
    Date searchDate;
    QStringList dateParts = filterDate.split(" ", Qt::SkipEmptyParts);
    if (dateParts.size() == 3) {
        searchDate.d = dateParts[0].toInt();
        searchDate.m = dateParts[1].toStdString(); // Переводим QString в std::string
        searchDate.y = dateParts[2].toInt();
    }

    //Собираем структуру Adres (из строки вида "Permskaya 12")
    Adres searchAdres;
    QStringList addrParts = filterAddress.split(" ", Qt::SkipEmptyParts);
    if (addrParts.size() >= 2) {
        // Предполагаем, что последнее слово — это номер дома, а всё перед ним — улица
        searchAdres.house = addrParts.last().toInt();
        addrParts.removeLast();
        searchAdres.street = addrParts.join(" ").toStdString();
    }

    //Собираем структуру FIO (из строки вида "Иванов Иван Иванович")
    FIO searchFio;
    QStringList fioParts = filterFio.split(" ", Qt::SkipEmptyParts);
    if (fioParts.size() >= 3) {
        searchFio.f = fioParts[0].toStdString();
        searchFio.i = fioParts[1].toStdString();
        searchFio.o = fioParts[2].toStdString();
    }

    return { searchDate,searchAdres, searchFio };
}

//Формирование отчёта
void MainWindow::onGenerateReportClicked() {
    if (!mainTree || !hashTable) {
        QMessageBox::warning(this, "Внимание", "Базы данных курьеров или заказов пусты!");
        return;
    }

    // Открываем модальный диалог запроса параметров (Дата, Адрес, ФИО)
    ReportFilterDialog paramDlg(this);
    if (paramDlg.exec() != QDialog::Accepted) {
        return; // Пользователь нажал Отмена
    }

    QString filterDate = paramDlg.getDateString();
    QString filterAddress = paramDlg.getAddressString();
    QString filterFio = paramDlg.getFioString();
    

    std::istringstream iss(filterDate.toStdString() + " " + filterAddress.toStdString() + " " + filterFio.toStdString());
    std::pair <Filters, std::string> result = CheckInputFilters(iss);
    if (result.second == "")
    {
        
        Filters filters = FilterAssembly(filterDate, filterAddress, filterFio); // сборка структуры Filters

        reportTree->GenerateReport(filters.date, filters.adres, filters.fio);
        // Создаем и показываем окно отчета как ОТДЕЛЬНОЕ независимое окно
        
        ReportWindow* reportWin = new ReportWindow(reportTree, this);
        reportWin->setAttribute(Qt::WA_DeleteOnClose);

        // Показываем окно немодально (пользователь может переключаться на главное окно)
        reportWin->show();
        reportWin->raise();
        reportWin->activateWindow();

        logMessage(QString("Сформирован отчет по параметрам -> Дата: [%1], Адрес: [%2], ФИО курьера: [%3]")
            .arg(filterDate).arg(filterAddress).arg(filterFio.isEmpty() ? "Все" : filterFio));
    }
    else
    {
        QMessageBox::critical(this, "Ошибка", QString::fromStdString(result.second));
        logMessage(QString("Ошибка в формировании отчёта. Данные были не корректны"));
          
    }
}


// Открытие окна ОТЛАДКИ
void MainWindow::showDebugWindow() {
    // Если окно отладки ещё не было создано за текущий сеанс
    if (!debugWindow) {
      
        debugWindow = new DebugWindow(this);
        debugWindow->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
        
    }

    // Принудительно обновляем в нём картинку/текст перед показом
    updateDebugWindow();
    debugWindow->updateViews(hashTable, mainTree);

    // Показываем окно (show вместо exec, чтобы оно не блокировало главное окно)
    debugWindow->show();
    debugWindow->raise();          // Выводим на передний план
    debugWindow->activateWindow(); // Фокусируем интерфейс на нём
}



