#pragma once

#include<vector>
#include<iostream>
#include "../DebugConfig.h"

typedef enum ValueType
{
	VALUE_INTEGER, VALUE_BOOL, VALUE_ARRAY, VALUE_STRING
};

struct BoolValue;
struct IntegerValue;
struct ArrayValue;

#ifdef __DEBUG_MEM_LEAK__
extern unsigned int _CREATES;
extern unsigned int _DELETES;
#endif

struct Value
{
	ValueType type;

	Value()
	{
#ifdef __DEBUG_MEM_LEAK__
		_CREATES++;
#endif
	}

	virtual ~Value()
	{
#ifdef __DEBUG_MEM_LEAK__
		_DELETES++;
#endif
	}
};

struct IntegerValue : public Value
{
	int val;

	IntegerValue(int value) : val(value)
	{
		this->type = VALUE_INTEGER;
	}

	friend std::ostream& operator<< (std::ostream& os, const IntegerValue * val)
	{
		os << val->val;
		return os;
	}
};

struct BoolValue : public Value
{
	bool val;

	BoolValue(bool value) : val(value)
	{
		this->type = VALUE_BOOL;
	}

	friend std::ostream& operator<< (std::ostream& os, const BoolValue* val)
	{
		os << (val->val ? "true" : "false");
		return os;
	}
};

struct ArrayValue : public Value
{
	std::vector<Value*> val;

	ArrayValue(std::vector<Value*> value) : val(value)
	{
		this->type = VALUE_ARRAY;
	}

	~ArrayValue()
	{
		for (unsigned int i = 0; i < val.size(); i++)
		{
			// TODO
			//delete val.at(i);
		}
	}

	friend std::ostream& operator<< (std::ostream& os, const ArrayValue * val)
	{
		os << "[";
		for (unsigned int i = 0; i < val->val.size(); i++)
		{
			if (val->val.at(i)->type == VALUE_INTEGER)
				os << ((IntegerValue*) val->val.at(i));
			else if (val->val.at(i)->type == VALUE_BOOL)
				os << ((BoolValue*) val->val.at(i));
			else if (val->val.at(i)->type == VALUE_ARRAY)
				os << ((ArrayValue*) val->val.at(i));

			if (i < val->val.size() - 1)
			{
				os << ", ";
			}
		}
		os << "]";
		return os;
	}
};

struct StringValue : public Value
{
	std::string val;

	StringValue(std::string value) : val(value)
	{
		this->type = VALUE_STRING;
	}

	friend std::ostream& operator<< (std::ostream& os, const StringValue* val)
	{
		os << val->val.c_str();
		return os;
	}
};