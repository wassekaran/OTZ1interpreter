#pragma once

#include "Scope.h"

class ScopeTree
{
protected:
	Scope * globalScope;
	Scope * currentScope;

public:
	ScopeTree()
	{
		globalScope = new Scope();
		currentScope = globalScope;
	}

	inline void push()
	{
		currentScope = new Scope(currentScope);
	}

	inline void pop()
	{
		currentScope = currentScope->getParent();
	}

	inline ScopeEntry * reference(std::string id)
	{
		return currentScope->reference(id);
	}

	inline void declare(std::string id)
	{
		currentScope->declare(id);
	}

	inline void dump()
	{
		currentScope->dump();
	}
};