#pragma once

#include "Node.h"
#include "../DebugConfig.h"
#include "../lexing/Token.h"
#include "../visitor/Value.h"
#include<iostream>
#include<vector>

#ifdef __DEBUG_MEM_LEAK__
extern unsigned int _CREATES;
extern unsigned int _DELETES;
#endif

struct Exp : public Node
{
	Exp()
	{
#ifdef __DEBUG_MEM_LEAK__
		_CREATES++;
#endif
	}

	virtual ~Exp()
	{
#ifdef __DEBUG_MEM_LEAK__
		_DELETES++;
#endif
	}
};

typedef std::vector<Exp*> exp_list;

struct TernaryExp : public Exp
{
	Exp * condition;
	Exp * left;
	Exp * right;

	TernaryExp(Exp * condition, Exp * left, Exp * right) : condition(condition), left(left), right(right)
	{
		this->type = NODE_TERNARY_EXP;
	}

	~TernaryExp()
	{
		delete condition;
		delete left;
		delete right;
	}
};

struct BoolExp : public Exp
{
	Exp * left;
	TokenType op;
	Exp * right;

	BoolExp(Exp * left, TokenType op, Exp * right) : left(left), op(op), right(right)
	{
		this->type = NODE_BOOL_EXP;
	}

	~BoolExp()
	{
		delete left;
		delete right;
	}
};

struct RelExp : public Exp
{
	Exp * left;
	TokenType op;
	Exp * right;

	RelExp(Exp * left, TokenType op, Exp * right) : left(left), op(op), right(right)
	{
		this->type = NODE_REL_EXP;
	}

	~RelExp()
	{
		delete left;
		delete right;
	}
};

struct BinaryExp : public Exp
{
	Exp * left;
	TokenType op;
	Exp * right;

	BinaryExp(Exp * left, TokenType op, Exp * right) : left(left), op(op), right(right)
	{
		this->type = NODE_BINARY_EXP;
	}

	~BinaryExp()
	{
		delete left;
		delete right;
	}
};

struct BinaryTerm : public Exp
{
	Exp * left;
	TokenType op;
	Exp * right;

	BinaryTerm(Exp * left, TokenType op, Exp * right) : left(left), op(op), right(right)
	{
		this->type = NODE_BINARY_TERM;
	}

	~BinaryTerm()
	{
		delete left;
		delete right;
	}
};

struct ValueExp : public Exp
{
	Value * val;

	ValueExp(Value * val) : val(val)
	{
		this->type = NODE_VALUE_EXP;
	}

	~ValueExp()
	{
		delete val;
	}
};

struct ArrayExp : public Exp
{
	exp_list expList;

	ArrayExp(exp_list expList) : expList(expList)
	{
		this->type = NODE_ARRAY_EXP;
	}

	~ArrayExp()
	{
		for (unsigned int i = 0; i < expList.size(); i++)
		{
			delete expList.at(i);
		}
	}
};

struct ArrayElementExp : public Exp
{
	Exp * arr;
	Exp * index;

	ArrayElementExp(Exp * arr, Exp * index) : arr(arr), index(index)
	{
		this->type = NODE_ARRAY_ELEMENT_EXP;
	}

	~ArrayElementExp()
	{
		delete arr;
		delete index;
	}
};

struct VarExp : public Exp
{
	std::string id;

	VarExp(std::string id) : id(id)
	{
		this->type = NODE_VAR_EXP;
	}
};

struct FuncCallExp : public Exp
{
	std::string id;
	exp_list args;

	FuncCallExp(std::string id, exp_list args) : id(id), args(args)
	{
		this->type = NODE_FUNCCALL_EXP;
	}
};