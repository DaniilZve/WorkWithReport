#pragma once
#include <iostream>
struct Date
{
	int d = 0;
	std::string m = "";
	int y = 0;
};
struct Adres
{
	std::string street = "";
	unsigned int house = 0;
};

struct FIO
{
	std::string f = "";
	std::string i = "";
	std::string o = "";
};

struct Transport
{
	std::string brand = "";
	std::string model = "";
};

struct DataCourier
{
	unsigned int passNum = 0;
	FIO fio;
	Transport transp;
};
struct Cell
{
	unsigned int key = 0;
	uint8_t state = 0;
	int index = 0;
};

struct DataOrder
{
	unsigned int passNum = 0;
	Adres adres;
	Date date;
	double price = 0;
};

struct DataReport
{
	unsigned int passNum;
	FIO fio;
	Transport transp;
	Adres adres;
	Date date;
	double price;
};

struct Filters
{
	Date date;
	Adres adres;
	FIO fio;
};