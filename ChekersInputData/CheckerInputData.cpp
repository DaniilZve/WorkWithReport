#include"CheckerInputData.h"
#include"../Stracts/Stracts.h"
#include<string>
#include<map>
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


std::pair <DataOrder, std::string> CheckInputOrder(std::istream& in)
{
    DataOrder elem;

    std::string passStr, houseStr, priceStr, dayStr, yearStr;

    // чтение
    if (!(in >> passStr >> elem.adres.street >> houseStr
        >> dayStr >> elem.date.m >> yearStr
        >> priceStr))
    {
        return { elem, "Ошибка считывания"};
    }

    // Номер пропуска 
    if (!isNatural(passStr) || passStr.length() != 5)
    {
        return { elem, "Ошибка в записи номера пропуска. Проверьте, чтобы длинна была равна 5, а сам номер был натуральным числом"};
    }
    elem.passNum = std::stoi(passStr);
    if (elem.passNum == 0)
    {
        return { elem, "Ошибка в записи номера пропуска. Номер пропуска не может быть равен 0"};
    }

    // Адрес
    if (!RegisterCheck(elem.adres.street))
    {
        return { elem,  "Ошибка в записи адреса(улица).Проверьте чтобы улица была записана латинскими символами с заглавной буквы"};
    }

    if (!isNatural(houseStr))
    {
        return { elem, "Ошибка в записи номер дома. Проверьте, чтобы номер был натуральным числом "};
    }
    elem.adres.house = std::stoi(houseStr);
    if (elem.adres.house == 0)
    {
        return { elem, "Ошибка в записи номера дома. Номер дома не может быть равен 0"};
    }

    // Дата
    if (!isNatural(dayStr) || !isNatural(yearStr) || yearStr.length() != 4)
    {
        return { elem, "Ошибка в записи даты. Проверьте, чтобы дата была в формате \"DD mon YYYY\""};
    }
    elem.date.d = std::stoi(dayStr);
    elem.date.y = std::stoi(yearStr);
    if ((elem.date.d == 0) || (elem.date.y == 0))
    {
        return { elem, "Ошибка в записи даты. День или год не могут быть равны 0"};
    }
    std::map<std::string, int> months = {
        {"jan", 1}, {"feb", 2}, {"mar", 3},
        {"apr", 4}, {"may", 5}, {"jun", 6},
        {"jul", 7}, {"aug", 8}, {"sep", 9},
        {"oct", 10}, {"nov", 11}, {"dec", 12}
    };
    auto it = months.find(elem.date.m);
    int NewDate;
    if (it != months.end())
        NewDate = it->second;
    else
        NewDate = -1; // если не найдено
    if (NewDate == -1)
    {
        return { elem, "Ошибка в записи даты. Проверьте месяц."};
    }

    // Цена (целая или вещественная, но положительная) ---
    if (!isPositiveNumber(priceStr))
    {
        return { elem, "Ошибка в записи цены Проверьте, чтобы цена была положительным вещественным числом"};
    }
    elem.price = std::stod(priceStr);
    if (elem.price == 0)
    {
        return { elem, "Ошибка в записи цены. Цена не может быть равна 0"};
    }

    return { elem, ""};
}
std::pair <DataCourier, std::string> CheckInputCourier(std::istream& in) {


    DataCourier elem;
    std::string passStr;

    // чтение 
    if (!(in >> passStr >> elem.fio.f >> elem.fio.i
        >> elem.fio.o >> elem.transp.brand >> elem.transp.model))
    {

        return { elem, "Ошибка считывания"};
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
    if (elem.transp.model.empty())
    {
        return { elem, "Ошибка в модели машины. Модель отсутствует" };
    }

    return { elem, ""};


}

std::pair <Filters, std::string> CheckInputFilters(std::istream& in)
{

    Filters elem;
    std::string dayStr, monthStr, yearStr, streetStr, houseStr, fStr, iStr, oStr;

    // чтение
    if (!(in >> dayStr >> monthStr >> yearStr
        >> streetStr >> houseStr
        >> fStr >> iStr >> oStr)){
        return { elem, "Ошибка в чтении"};
    }

    // Дата
    if (!isNatural(dayStr) || !isNatural(yearStr) || yearStr.length() != 4)
    {
        return { elem, "Ошибка в записи даты. Проверьте, чтобы дата была в формате \"DD mon YYYY\" " };
    }
    elem.date.d = std::stoi(dayStr);
    elem.date.y = std::stoi(yearStr);
    if ((elem.date.d == 0) || (elem.date.y == 0))
    {
        return { elem, "Ошибка в записи даты. День или год не могут быть равны 0" };
    }
    std::map<std::string, int> months = {
        {"jan", 1}, {"feb", 2}, {"mar", 3},
        {"apr", 4}, {"may", 5}, {"jun", 6},
        {"jul", 7}, {"aug", 8}, {"sep", 9},
        {"oct", 10}, {"nov", 11}, {"dec", 12}
    };
    auto it = months.find(monthStr);
    int NewDate;
    if (it != months.end())
        NewDate = it->second;
    else
        NewDate = -1; // если не найдено
    if (NewDate == -1)
    {
        return { elem, "Ошибка в записи даты.Проверьте месяц." };
    }

    // Адрес
    if (!RegisterCheck(elem.adres.street))
    {
        return { elem, "Ошибка в записи адреса(улица).Проверьте чтобы улица была записана латинскими символами с заглавной буквы" };
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

    // ФИО
    if (!RegisterCheck(elem.fio.f))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы фамилия была записана латинскими символами с заглавной буквы" };
    }
    if (!RegisterCheck(elem.fio.i))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы имя было записано латинскими символами с заглавной буквы"  };
    }
    if (!RegisterCheck(elem.fio.o))
    {
        return { elem, "Ошибка в записи ФИО. Проверьте, чтобы отчество было записано латинскими символами с заглавной буквы" };
    }

    return { elem, ""};


    


}