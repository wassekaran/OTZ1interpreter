#pragma once

#include "Visitor.h"

class Interpreter : public Visitor
{
public:
	Value * eval(Exp * exp) override;

private:
	Value * eval(TernaryExp * exp);
	Value * eval(BoolExp * exp);
	Value * eval(RelExp * exp);
	Value * eval(BinaryExp * exp);
	Value * eval(BinaryTerm * exp);
	Value * eval(ValueExp * exp);
	Value * eval(ArrayExp * exp);
	Value * eval(ArrayElementExp * exp);
};