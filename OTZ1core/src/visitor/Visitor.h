#pragma once

#include "Value.h"
#include "../ast/Exp.h"

class Visitor
{
public:
	virtual Value * eval(Exp * exp) = 0;
};