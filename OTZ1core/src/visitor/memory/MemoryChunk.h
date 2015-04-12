#pragma once

#include "../Value.h"
#include "../../ast/Stmt.h"
#include<vector>
#include<string>

struct MemoryChunk
{
	std::string id;

	MemoryChunk(std::string id) : id(id) {}
};

struct Variable : public MemoryChunk
{
	Value * value;

	Variable(std::string id, Value * value) : MemoryChunk(id), value(value) {}
};

struct Function : public MemoryChunk
{
	std::vector<std::string> args;
	stmt_list body;

	Function(std::string id, std::vector<std::string> args, stmt_list body) : MemoryChunk(id), args(args), body(body) {}
};