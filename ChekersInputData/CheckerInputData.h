#pragma once
#include<iostream>

struct DataCourier;
struct DataOrder;
struct Filters;
bool isPowerOfTwo(size_t n);
bool isNatural(const std::string& s);
bool isPositiveNumber(const std::string& s);
bool RegisterCheck(std::string& str);
int ConvMonToInt(std::string& m);
std::string ConvIntToMon(int m);
bool isValidDate(int day, int month, int year);
std::pair <DataOrder, std::string> CheckInputOrder(std::istream& in);
std::pair <DataCourier, std::string> CheckInputCourier(std::istream& in);
std::pair <Filters, std::string> CheckInputFilters(std::istream& in);
