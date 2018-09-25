/*
Data.h: A sample data class for communicating between server and client

@author Navid Assadian
*/

#pragma once
#include "Serialization.h"



class Data
{
private:
	int int_data;
	double double_data;
	char* string_data;
	int ID;

public:
	Data();
	~Data();
	Data(int, double, char*);

	void Ser(char*);
	void Unser(char*);

	size_t size();
	void set_int(int);
	void set_double(double);
	void set_string(const char*);
	void set_ID(int);

	int get_int();
	double get_double();
	char* get_string();
	int get_ID();

};