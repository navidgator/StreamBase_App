/**
Data.cpp
The implementation of functions of Data.h: A sample data class for communicating between server and client

@author Navid Assadian

*/

#include "Data.h"
#include <cstring>
#include <string>
#include "Serialization.h"

#define BUFFERSIZE 512
Data::Data()
{
	int_data = 0;
	double_data = 0.;
	string_data = new char[1];
	ID = -1;
}
Data::~Data()
{
	//delete[] string_data;
}
Data::Data(int int_num, double double_num, char* str)
{
	int_data = int_num;
	double_data = double_num;
	string_data = new char[sizeof(str)];
	strncpy_s(string_data, BUFFERSIZE, str, sizeof(str));
	ID = -1;
}

void Data::set_int(int int_num)
{
	int_data = int_num;
}
void Data::set_double(double double_num)
{
	double_data = double_num;
}
void Data::set_string(const char* str)
{
	string_data = new char[sizeof(str)];
	strncpy_s(string_data, BUFFERSIZE, str, sizeof(str));
}

void Data::set_ID(int ID_num)
{
	ID = ID_num;
}

int Data::get_int()
{
	return int_data;
}
double Data::get_double()
{
	return double_data;
}
char* Data::get_string()
{
	return string_data;
}
int Data::get_ID()
{
	return ID;
}

void Data::Ser(char* out_message)
{
	Serialization<int>::serialize(out_message, int_data);
	Serialization<double>::serialize(out_message + sizeof(int), double_data);
	Serialization<int>::serialize(out_message + sizeof(int) + sizeof(double), ID);
	Serialization<char*>::serialize(out_message + 2 * sizeof(int) + sizeof(double), string_data);
}

void Data::Unser(char* in_message)
{
	Serialization<int>::unserialize(in_message, int_data);
	Serialization<double>::unserialize(in_message + sizeof(int), double_data);
	Serialization<char*>::unserialize(in_message + 2 * sizeof(int) + sizeof(double), string_data);
	Serialization<int>::unserialize(in_message + sizeof(int) + sizeof(double), ID);
}

size_t Data::size()
{
	return (sizeof(int_data) + sizeof(ID) + sizeof(double_data) + sizeof(string_data));
}
