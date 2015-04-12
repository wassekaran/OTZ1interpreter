#pragma once

#include "Value.h"
#include "../ast/Exp.h"
#include "../ast/Stmt.h"

class Visitor
{
public:
	virtual Value * eval(Exp * exp) = 0;

	virtual void exec(Stmt * stmt) = 0;
};