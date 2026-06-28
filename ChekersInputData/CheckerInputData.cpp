#include"CheckerInputData.h"
#include"../Stracts/Stracts.h"
#include<string>
#include<map>


bool isPowerOfTwo(size_t n) {

    return n > 0 && (n & (n - 1)) == 0;
}
bool isNatural(const std::string& s)
{
    if (s.empty()) return false;

    for (char c : s)
    {
        if (c < '0' || c > '9')
            return false;
    }

    return true;
}
bool isPositiveNumber(const std::string& s)
{
    if (s.empty()) return false;

    bool dot = false;
    bool hasDigit = false;

    for (char c : s)
    {
        if (c >= '0' && c <= '9')
        {
            hasDigit = true;
        }
        else if (c == '.')
        {
            if (dot) return false; // вторая точка
            dot = true;
        }
        else
        {
            return false; // любой другой символ
        }
    }

    // должна быть хотя бы одна цифра
    if (!hasDigit) return false;

    // "." не допускаем
    if (s == ".") return false;

    // нельзя начинать или заканчивать точкой
    if (s.front() == '.' || s.back() == '.') return false;

    return true;
}


bool RegisterCheck(std::string& str) {
    if (str.empty()) {
        return false;
    }

    unsigned char first = str[0];
    // первая буква — заглавная латинская
    if (!(first >= 'A' && first <= 'Z')) {
        return false;
    }

    // остальные — строчные латинские
    for (size_t i = 1; i < str.size(); i++) {
        unsigned char c = str[i];

        if (!(c >= 'a' && c <= 'z')) {
            return false;
        }
    }

    return true;
}

int ConvMonToInt(std::string& m)
{
    std::map<std::string, int> months = {
        {"jan", 1}, {"feb", 2}, {"mar", 3},
        {"apr", 4}, {"may", 5}, {"jun", 6},
        {"jul", 7}, {"aug", 8}, {"sep", 9},
        {"oct", 10}, {"nov", 11}, {"dec", 12}
    };

    auto it = months.find(m);
    if (it != months.end())
        return it->second;
    return -1; // если не найдено
}

std::string ConvIntToMon(int m)
{
    std::map<int, std::string> months = {
        {1, "jan"}, {2, "feb"}, {3, "mar"},
        {4, "apr"}, {5, "may"}, {6,"jun"},
        {7, "jul"}, {8, "aug"}, {9, "sep"},
        {10, "oct"}, {11, "nov"}, {12, "dec"}
    };

    auto it = months.find(m);
    if (it != months.end())
        return it->second;
    return ""; // если не найдено
}

// Функция проверки корректности даты (с учетом високосного года)
bool isValidDate(int day, int month, int year) {
    // Базовые проверки
    if (day < 1 || month < 1 || month > 12) return false;

    int daysInMonth = 31; // По умолчанию 31 день (янв, мар, май, июл, авг, окт, дек)

    // Месяцы с 30 днями (апр, июн, сен, ноя)
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    }
    // Февраль
    else if (month == 2) {
        // Проверка на високосный год (кратен 4, но не кратен 100, либо кратен 400)
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        daysInMonth = isLeap ? 29 : 28;
    }

    return day <= daysInMonth;
}

std::pair <DataOrder, std::string> CheckInputOrder(std::istream& in)
{
    DataOrder elem;

    std::string passStr, houseStr, priceStr, dayStr, yearStr, extra;

    // чтение
    if (!(in >> passStr >> elem.adres.street >> houseStr
        >> dayStr >> elem.date.m >> yearStr
        >> priceStr))
    {
        return { elem, "Ошибка считывания. Возможно не хватает каких-то данных" };
    }

    in >> extra;
    if (!extra.empty())
    {
        return { elem, "Были введены лишние данные" };
    }
    // Номер пропуска 
    if (!isNatural(passStr) || passStr.length() != 5)
    {
        return { elem, "Ошибка в записи номера пропуска. Проверьте, чтобы длинна была равна 5, а сам номер был натуральным числом" };
    }
    elem.passNum = std::stoi(passStr);
    if (elem.passNum == 0)
    {
        return { elem, "Ошибка в записи номера пропуска. Номер пропуска не может быть равен 0" };
    }

    // Адрес
    if (!RegisterCheck(elem.adres.street))
    {
        return { elem,  "Ошибка в записи адреса(улица).Проверьте чтобы улица была записана латинскими символами с заглавной буквы" };
    }

    if (!isNatural(houseStr))
    {
        return { elem, "Ошибка в записи номер дома. Проверьте, чтобы номер был натуральным числом " };
    }
    elem.adres.house = std::stoi(houseStr);
    if (elem.adres.house == 0)
    {
        return { elem, "Ошибка в записи номера дома. Номер дома не может быть равен 0" };
    }

    // Дата
    if (!isNatural(dayStr) || !isNatural(yearStr) || yearStr.length() != 4)
    {
        return { elem, "Ошибка в записи даты. Проверьте, чтобы дата была в формате \"DD mon YYYY\"" };
    }
    elem.date.d = std::stoi(dayStr);
    elem.date.y = std::stoi(yearStr);
    if ((elem.date.d < 1) || (elem.date.d > 31))
    {
        return { elem, "Ошибка в записи даты. День должен находиться в диапазоне от 1 до 31" };
    }
    if ((elem.date.y < 1990) || (elem.date.y > 2026))
    {
        return { elem, "Ошибка в записи даты. Год должен находиться в диапазоне от 1990 до 2026" };
    }
   
    int NewDate = ConvMonToInt(elem.date.m);
    if (NewDate == -1)
    {
        return { elem, "Ошибка в записи даты. Проверьте месяц." };
    }
    if (!isValidDate(elem.date.d, NewDate, elem.date.y)) {
        return { elem, "Ошибка в записи даты. Указан некорректный день для данного месяца (возможно, ошибка високосного года)." };
    }

    // Цена (целая или вещественная, но положительная) ---
    if (!isPositiveNumber(priceStr))
    {
        return { elem, "Ошибка в записи цены Проверьте, чтобы цена была положительным вещественным числом"};
    }
    elem.price = std::stod(priceStr);
    if (elem.price <= 0.00001)
    {
        return { elem, "Ошибка в записи цены. Цена не может быть равна 0"};
    }

    return { elem, ""};
}
std::pair <DataCourier, std::string> CheckInputCourier(std::istream& in) {


    DataCourier elem;
    std::string passStr, extra;

    // чтение 
    if (!(in >> passStr >> elem.fio.f >> elem.fio.i
        >> elem.fio.o >> elem.transp.brand >> elem.transp.model))
    {

        return { elem, "Ошибка считывания. Возможно не хватает каких-то данных"};
    }

    in >> extra;
    if (!extra.empty())
    {
        return { elem, "Были введены лишние данные" };
    }

    // Номер пропуска 
    if (!isNatural(passStr) || passStr.length() != 5) {
        return { elem, "Ошибка в записи номера пропуска. Проверьте, чтобы длинна была равна 5, а сам номер был натуральным числом"};
    }
    elem.passNum = std::stoi(passStr);
    if (elem.passNum == 0) {
        return { elem, "Ошибка в записи номера пропуска.Номер пропуска не может быть равен 0"};
    }

    // ФИО
    if (!RegisterCheck(elem.fio.f))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы фамилия была записана латинскими символами с заглавной буквы"};
    }
    if (!RegisterCheck(elem.fio.i))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы имя было записано латинскими символами с заглавной буквы" };
    }
    if (!RegisterCheck(elem.fio.o))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы отчество было записано латинскими символами с заглавной буквы" };
    }

    //Марка
    if (!RegisterCheck(elem.transp.brand))
    {
        return { elem, "Ошибка в записи марки машины. Проверьте, чтобы марка была записана латинскими символами с заглавной буквы"};
    }
    //Модель
    // не русские
    for (size_t i = 0; i < elem.transp.model.size(); i++) {
        unsigned char c = elem.transp.model[i];
        if (!(c >= 'A' && c <= 'Z')) {
            if (!(c >= 'a' && c <= 'z'))
            {
                if (!(c >= '0' && c <= '9'))
                {
                    return { elem, "Ошибка в модели машины. В модели не может быть букв русского алфавита" };
                }
            }
        }
    }
    if (elem.transp.model.empty())
    {
        return { elem, "Ошибка в модели машины. Модель отсутствует" };
    }

    return { elem, ""};


}


std::pair <Filters, std::string> CheckInputFilters(std::istream& in)
{

    Filters elem;
    std::string dayStr, monthStr, yearStr, streetStr, houseStr, fStr, iStr, oStr, extra;

    // чтение
    if (!(in >> dayStr >> monthStr >> yearStr
        >> streetStr >> houseStr
        >> fStr >> iStr >> oStr )){
        return { elem, "Ошибка считывания. Возможно не хватает каких-то данных"};
    }

    in >> extra;
    if (!extra.empty())
    {
        return { elem, "Были введены лишние данные" };
    }

    // Дата
    if (!isNatural(dayStr) || !isNatural(yearStr) || yearStr.length() != 4)
    {
        return { elem, "Ошибка в записи даты. Проверьте, чтобы дата была в формате \"DD mon YYYY\" " };
    }
    elem.date.d = std::stoi(dayStr);
    elem.date.y = std::stoi(yearStr);
    if ((elem.date.d < 1) || (elem.date.d > 31))
    {
        return { elem, "Ошибка в записи даты. День должен находиться в диапазоне от 1 до 31"};
    }
    if ((elem.date.y < 1990) || (elem.date.y > 2026))
    {
        return { elem, "Ошибка в записи даты. Год должен находиться в диапазоне от 1990 до 2026"};
    }
    int NewDate = ConvMonToInt(monthStr);
    if (NewDate == -1)
    {
        return { elem, "Ошибка в записи даты. Проверьте месяц." };
    }
    if (!isValidDate(elem.date.d, NewDate, elem.date.y)) {
        return { elem, "Ошибка в записи даты. Указан некорректный день для данного месяца (возможно, ошибка високосного года)." };
    }
    elem.date.m = monthStr;

    // Адрес
    if (!RegisterCheck(streetStr))
    {
        return { elem, "Ошибка в записи адреса(улица).Проверьте чтобы улица была записана латинскими символами с заглавной буквы" };
    }
    elem.adres.street = streetStr;

    if (!isNatural(houseStr))
    {
        return { elem, "Ошибка в записи номер дома. Проверьте, чтобы номер был натуральным числом " };
    }
    elem.adres.house = std::stoi(houseStr);
    if (elem.adres.house == 0)
    {
        return { elem, "Ошибка в записи номера дома. Номер дома не может быть равен 0" };
    }

    // ФИО
    if (!RegisterCheck(fStr))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы фамилия была записана латинскими символами с заглавной буквы" };
    }
    elem.fio.f = fStr;
    if (!RegisterCheck(iStr))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы имя было записано латинскими символами с заглавной буквы"  };
    }
    elem.fio.i = iStr;
    if (!RegisterCheck(oStr))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы отчество было записано латинскими символами с заглавной буквы" };
    }
    elem.fio.o = oStr;
    return { elem, ""};


    


}