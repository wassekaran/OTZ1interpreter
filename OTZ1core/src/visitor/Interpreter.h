#pragma once

#include "Visitor.h"
#include "memory\MemorySpace.h"
#include<stack>

class Interpreter : public Visitor
{
protected:
	MemorySpace * globalSpace;
	MemorySpace * currentSpace;
	std::stack<MemorySpace*> callstack;

public:
	Interpreter();

	Value * eval(Exp * exp) override;
	void exec(Stmt * stmt) override;

private:
	Value * eval(TernaryExp * exp);
	Value * eval(BoolExp * exp);
	Value * eval(RelExp * exp);
	Value * eval(BinaryExp * exp);
	Value * eval(BinaryTerm * exp);
	Value * eval(ValueExp * exp);
	Value * eval(ArrayExp * exp);
	Value * eval(ArrayElementExp * exp);
	Value * eval(FuncCallExp * exp);
	Value * eval(VarExp * exp);

	void exec(FuncDefStmt * stmt);
	void exec(VarDefStmt * stmt);
	void exec(VarAssStmt * stmt);
	void exec(FuncCallStmt * stmt);
	void exec(IfStmt * stmt);
	void exec(PrintStmt * stmt);
	void exec(ReturnStmt * stmt);

	MemoryChunk * getMemoryChunk(std::string);
};