#include "dialogs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QDialogButtonBox>
#include"ScrReportAVLTree/ReportTree.h"
#include <sstream>


// =================== ВВОД КУРЬЕРА ===================
CourierInputDialog::CourierInputDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Данные курьера");
    resize(420, 150);
    setModal(true);

    QFormLayout* formLayout = new QFormLayout(this);

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Например: 12345");
    formLayout->addRow("Номер пропуска:", passEdit);

    fioEdit = new QLineEdit(this);
    fioEdit->setPlaceholderText("Иванов Иван Иванович (на латинице)");
    formLayout->addRow("ФИО (через пробел):", fioEdit);

    tsEdit = new QLineEdit(this);
    tsEdit->setPlaceholderText("Toyota Camry (на латинице)");
    formLayout->addRow("Транспорт (Марка Модель):", tsEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    formLayout->addRow(buttonBox);
}

QString CourierInputDialog::getInputString() const {
    // Собираем в единую строку через пробелы для передачи в CheckInputCourier
    return QString("%1 %2 %3").arg(passEdit->text().trimmed())
        .arg(fioEdit->text().trimmed())
        .arg(tsEdit->text().trimmed());
}





// =================== ВВОД ЗАКАЗА ===================
OrderInputDialog::OrderInputDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Данные заказа");
    resize(420, 170);
    setModal(true);

    QFormLayout* formLayout = new QFormLayout(this);

    passEdit = new QLineEdit(this);
    passEdit->setPlaceholderText("Пример: 54321");
    formLayout->addRow("Номер пропуска (5 цифр):", passEdit);

    adresEdit = new QLineEdit(this);
    adresEdit->setPlaceholderText("Пример: Permskaya 12");
    formLayout->addRow("Адрес:", adresEdit);

    

    dateEdit = new QLineEdit(this);
    dateEdit->setPlaceholderText("Пример: 13 jun 2026");
    formLayout->addRow("Дата (DD mon YYYY):", dateEdit);

    priceEdit = new QLineEdit(this);
    priceEdit->setPlaceholderText("Пример: 1500.75");
    formLayout->addRow("Стоимость заказа:", priceEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    formLayout->addRow(buttonBox);
}

QString OrderInputDialog::getInputString() const {
    // Формируем строку в порядке чтения: pass street house day m year price
    return QString("%1 %2 %3 %4")
        .arg(passEdit->text().trimmed())
        .arg(adresEdit->text().trimmed())
        .arg(dateEdit->text().trimmed())
        .arg(priceEdit->text().trimmed());
}

// =================== ФИЛЬТРЫ ОТЧЕТА ===================
ReportFilterDialog::ReportFilterDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Параметры формирования отчёта");
    setModal(true);

    QFormLayout* formLayout = new QFormLayout(this);

    // Блок даты
    dayEdit = new QLineEdit(this); dayEdit->setPlaceholderText("13");
    monthEdit = new QLineEdit(this); monthEdit->setPlaceholderText("jun");
    yearEdit = new QLineEdit(this); yearEdit->setPlaceholderText("2026");

    // Блок адреса
    streetEdit = new QLineEdit(this); streetEdit->setPlaceholderText("Permskaya");
    houseEdit = new QLineEdit(this); houseEdit->setPlaceholderText("12");

    // Блок ФИО (Новое!)
    surnameEdit = new QLineEdit(this); surnameEdit->setPlaceholderText("Иванов");
    nameEdit = new QLineEdit(this); nameEdit->setPlaceholderText("Иван");
    patronymicEdit = new QLineEdit(this); patronymicEdit->setPlaceholderText("Иванович");

    //formLayout->addRow("--- Дата заказа ---");
    formLayout->addRow("День (DD):", dayEdit);
    formLayout->addRow("Месяц (3 буквы):", monthEdit);
    formLayout->addRow("Год (YYYY):", yearEdit);

    //formLayout->addRow("--- Адрес заказа ---");
    formLayout->addRow("Улица:", streetEdit);
    formLayout->addRow("Дом:", houseEdit);

    //formLayout->addRow("--- ФИО Курьера (Фильтр) ---");
    formLayout->addRow("Фамилия:", surnameEdit);
    formLayout->addRow("Имя:", nameEdit);
    formLayout->addRow("Отчество:", patronymicEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    formLayout->addRow(buttonBox);
}

QString ReportFilterDialog::getDateString() const {
    return QString("%1 %2 %3").arg(dayEdit->text().trimmed())
        .arg(monthEdit->text().trimmed())
        .arg(yearEdit->text().trimmed());
}

QString ReportFilterDialog::getAddressString() const {
    return QString("%1 %2").arg(streetEdit->text().trimmed())
        .arg(houseEdit->text().trimmed());
}

QString ReportFilterDialog::getFioString() const {
    return QString("%1 %2 %3").arg(surnameEdit->text().trimmed())
        .arg(nameEdit->text().trimmed())
        .arg(patronymicEdit->text().trimmed()).trimmed();
}


// =================== ОКНО ОТЧЕТА ===================
ReportWindow::ReportWindow(ReportTree* reportTree, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Сформированный отчёт (Текстовый вид)");
    resize(850, 550);

    // Настройки независимого окна
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    setModal(false);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // 1. Создаем виджет многострочного текста
    reportTextEdit = new QPlainTextEdit(this);
    reportTextEdit->setReadOnly(true); // Запрещаем пользователю редактировать текст

    // 2. Устанавливаем шрифт "Courier New" (моноширинный), чтобы текст выглядел ровно, как в .txt файле
    QFont textFont("Courier New", 10);
    reportTextEdit->setFont(textFont);

    layout->addWidget(reportTextEdit);

    // 3. Собираем весь текст отчета
    QString fileContent = "";

    if (reportTree && reportTree->ReportArr) {
            std::stringstream ss;
            reportTree->PrintReport(ss);
            // Преобразуем стандартную строку std::string в QString и добавляем в общий отчет
            fileContent += QString::fromStdString(ss.str());
        
    }

    // 4. Записываем собранный текст в поле
    if (fileContent.isEmpty()) {
        reportTextEdit->setPlainText("Нет данных, удовлетворяющих заданным фильтрам (Дата, Адрес, ФИО).");
    }
    else {
        reportTextEdit->setPlainText(fileContent);
    }
}