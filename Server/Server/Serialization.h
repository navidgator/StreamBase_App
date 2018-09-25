/*
Serialization.h
A template class for serilizing and unserializing different data types and arrays

@author Navid Assadian
*/

#pragma once
#include <memory>
#include <cstring>


template <class T>
class Serialization
{
public:
	static void serialize(char* out_message, T& data)
	{
		memcpy(out_message, &data, sizeof(data));
	}

	static void unserialize(const char* in_message, T& (data))
	{
		memcpy(&data, in_message, sizeof(data));
	}

	static void serialize(char* out_message, T*  data)
	{
		size_t len = sizeof(data) / sizeof(T);
		memcpy(out_message, &len, sizeof(size_t));
		memcpy(out_message + sizeof(size_t), data, len * sizeof(T));
	}

	static void unserialize(const char* in_message, T* data)
	{
		size_t len;
		memcpy(&len, in_message, sizeof(size_t));
		data = new T[len];
		memcpy(data, in_message + sizeof(size_t), len * sizeof(T));
	}


};

