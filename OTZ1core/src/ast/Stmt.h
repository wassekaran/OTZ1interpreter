#pragma once

#include "Node.h"
#include "../DebugConfig.h"
#include<iostream>
#include<vector>

#ifdef __DEBUG_MEM_LEAK__
extern unsigned int _CREATES;
extern unsigned int _DELETES;
#endif

struct Stmt : public Node
{
	Stmt()
	{
#ifdef __DEBUG_MEM_LEAK__
		_CREATES++;
#endif
	}

	virtual ~Stmt()
	{
#ifdef __DEBUG_MEM_LEAK__
		_DELETES++;
#endif
	}
};

typedef std::vector<Stmt*> stmt_list;

struct FuncDefStmt : public Stmt
{
	std::string id;
	std::vector<std::string> args;
	stmt_list body;

	FuncDefStmt(std::string id, std::vector<std::string> args, stmt_list body) : id(id), args(args), body(body)
	{
		this->type = NODE_FUNCDEF_STMT;
	}
};

struct VarDefStmt : public Stmt
{
	std::string id;
	Exp * exp;

	VarDefStmt(std::string id, Exp * exp) : id(id), exp(exp)
	{
		this->type = NODE_VARDEF_STMT;
	}

	~VarDefStmt()
	{
		delete exp;
	}
};

struct VarAssStmt : public Stmt
{
	std::string id;
	Exp * exp;

	VarAssStmt(std::string id, Exp * exp) : id(id), exp(exp)
	{
		this->type = NODE_VARASS_STMT;
	}

	~VarAssStmt()
	{
		delete exp;
	}
};

struct FuncCallStmt : public Stmt
{
	Exp * funcall;

	FuncCallStmt(Exp * funcall) : funcall(funcall)
	{
		this->type = NODE_FUNCCALL_STMT;
	}
};

struct IfStmt : public Stmt
{
	Exp * cond;
	stmt_list body;

	IfStmt(Exp * cond, stmt_list body) : cond(cond), body(body)
	{
		this->type = NODE_IF_STMT;
	}

	~IfStmt()
	{
		delete cond;
	}
};

struct PrintStmt : public Stmt
{
	Exp * exp;

	PrintStmt(Exp * exp) : exp(exp)
	{
		this->type = NODE_PRINT_STMT;
	}

	~PrintStmt()
	{
		delete exp;
	}
};

struct ReturnStmt : public Stmt
{
	Exp * exp;

	ReturnStmt(Exp * exp) : exp(exp)
	{
		this->type = NODE_RETURN_STMT;
	}

	~ReturnStmt()
	{
		delete exp;
	}
};