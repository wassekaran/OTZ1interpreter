#include "Interpreter.h"
#include<iostream>

Interpreter::Interpreter()
{
	globalSpace = new MemorySpace();
	currentSpace = globalSpace;
}

Value * Interpreter::eval(Exp * exp)
{
	switch (exp->type)
	{
	case NODE_TERNARY_EXP:
		return eval(TERNARY_EXP(exp));

	case NODE_BOOL_EXP:
		return eval(BOOL_EXP(exp));

	case NODE_REL_EXP:
		return eval(REL_EXP(exp));

	case NODE_BINARY_EXP:
		return eval(BINARY_EXP(exp));

	case NODE_BINARY_TERM:
		return eval(BINARY_TERM(exp));

	case NODE_VALUE_EXP:
		return eval(VALUE_EXP(exp));

	case NODE_ARRAY_EXP:
		return eval(ARRAY_EXP(exp));

	case NODE_ARRAY_ELEMENT_EXP:
		return eval(ARRAY_ELEMENT_EXP(exp));

	case NODE_FUNCCALL_EXP:
		return eval(FUNC_CALL_EXP(exp));

	case NODE_VAR_EXP:
		return eval(VAR_EXP(exp));

	default:
		std::cout << "What" << std::endl;
	}

	return nullptr;
}

Value * Interpreter::eval(TernaryExp * exp)
{
	Value * cond = eval(exp->condition);

	if (cond->type == VALUE_BOOL)
	{
		if (((BoolValue*)cond)->val)
		{
			return eval(exp->left);
		}
		else
		{
			return eval(exp->right);
		}
	}

	std::cout << "No way 555" << std::endl;
	return nullptr;
}

Value * Interpreter::eval(BoolExp * exp)
{
	Value * left = eval(exp->left);

	if (left->type == VALUE_BOOL)
	{
		if (exp->op == TOKEN_OR)
		{
			if (((BoolValue*)left)->val)
			{
				delete left;
				return new BoolValue(true);
			}
			else
			{
				delete left;

				Value * right = eval(exp->right);
				if (right->type == VALUE_BOOL)
				{
					return new BoolValue(((BoolValue*) right)->val);
				}
				std::cout << "OMGG" << std::endl;
			}
		}
		else
		{
			Value * right = eval(exp->right);

			if (right->type == VALUE_BOOL)
			{
				BoolValue * result = new BoolValue(((BoolValue*)left)->val && ((BoolValue*)right)->val);

				delete left;
				delete right;

				return result;
			}

			std::cout << "NOOOO" << std::endl;
		}
	}

	std::cout << "No way4" << std::endl;
	return nullptr;
}

Value * Interpreter::eval(RelExp * exp)
{
	Value * left = eval(exp->left);
	Value * right = eval(exp->right);

	if (left->type == VALUE_INTEGER && right->type == VALUE_INTEGER)
	{
		Value * result = nullptr;
		if (exp->op == TOKEN_EQUALS)
			result = new BoolValue(((IntegerValue*)left)->val == ((IntegerValue*)right)->val);
		else if (exp->op == TOKEN_NOT_EQUALS)
			result = new BoolValue(((IntegerValue*)left)->val != ((IntegerValue*)right)->val);
		else if (exp->op == TOKEN_GREATER_THAN)
			result = new BoolValue(((IntegerValue*)left)->val > ((IntegerValue*)right)->val);
		else if (exp->op == TOKEN_LESS_THAN)
			result = new BoolValue(((IntegerValue*)left)->val < ((IntegerValue*)right)->val);
		else if (exp->op == TOKEN_GOE_THAN)
			result = new BoolValue(((IntegerValue*)left)->val >= ((IntegerValue*)right)->val);
		else
			result = new BoolValue(((IntegerValue*)left)->val <= ((IntegerValue*)right)->val);

		delete left;
		delete right;

		return result;
	}
	else if (left->type == VALUE_BOOL && right->type == VALUE_BOOL)
	{
		Value * result = nullptr;
		if (exp->op == TOKEN_EQUALS)
			result = new BoolValue(((BoolValue*)left)->val == ((BoolValue*)right)->val);
		else
			result = new BoolValue(((BoolValue*)left)->val != ((BoolValue*)right)->val);

		delete left;
		delete right;

		return result;
	}

	std::cout << "No way3" << std::endl;
	return nullptr;
}

Value * Interpreter::eval(BinaryExp * exp)
{
	Value * left = eval(exp->left);
	Value * right = eval(exp->right);

	if (left->type == VALUE_INTEGER && right->type == VALUE_INTEGER)
	{
		Value * result = nullptr;
		if (exp->op == TOKEN_PLUS)
			result = new IntegerValue(((IntegerValue*)left)->val + ((IntegerValue*)right)->val);
		else
			result = new IntegerValue(((IntegerValue*)left)->val - ((IntegerValue*)right)->val);

		delete left;
		delete right;

		return result;
	}
	else if (left->type == VALUE_STRING && right->type == VALUE_STRING)
	{
		Value * result = nullptr;
		if (exp->op == TOKEN_PLUS)
			result = new StringValue(((StringValue*)left)->val + ((StringValue*)right)->val);

		delete left;
		delete right;

		return result;
	}

	std::cout << "No way2" << std::endl;
	return nullptr;
}

Value * Interpreter::eval(BinaryTerm * exp)
{
	Value * left = eval(exp->left);
	Value * right = eval(exp->right);

	if (left->type == VALUE_INTEGER && right->type == VALUE_INTEGER)
	{
		Value * result = nullptr;
		if (exp->op == TOKEN_STAR)
			result = new IntegerValue(((IntegerValue*)left)->val * ((IntegerValue*)right)->val);
		else if (exp->op == TOKEN_STAR)
			result = new IntegerValue(((IntegerValue*)left)->val / ((IntegerValue*)right)->val);
		else
			result = new IntegerValue(((IntegerValue*)left)->val % ((IntegerValue*)right)->val);

		delete left;
		delete right;

		return result;
	}

	std::cout << "No way" << std::endl;
	return nullptr;
}

Value * Interpreter::eval(ValueExp * exp)
{
	if (exp->val->type == VALUE_BOOL)
	{
		return new BoolValue(((BoolValue*)exp->val)->val);
	}
	else if (exp->val->type == VALUE_INTEGER)
	{
		return new IntegerValue(((IntegerValue*)exp->val)->val);
	}
	else if (exp->val->type == VALUE_STRING)
	{
		return new StringValue(((StringValue*)exp->val)->val);
	}
}

Value * Interpreter::eval(ArrayExp * exp)
{
	std::vector<Value*> arrval;
	Value * curVal;
	for (unsigned int i = 0; i < exp->expList.size(); i++)
	{
		curVal = eval(exp->expList.at(i));
		arrval.push_back(curVal);
	}

	return new ArrayValue(arrval);
}

Value * Interpreter::eval(ArrayElementExp * exp)
{
	Value * arr = eval(exp->arr);

	if (arr->type != VALUE_ARRAY && arr->type != VALUE_STRING)
	{
		std::cout << "Not array" << std::endl;
		return nullptr;
	}

	Value * index = eval(exp->index);
	if (index->type != VALUE_INTEGER)
	{
		std::cout << "Not integer as idx" << std::endl;
		return nullptr;
	}

	Value * val = nullptr;
	
	if (arr->type == VALUE_ARRAY)
		val = ((ArrayValue*)arr)->val.at(((IntegerValue*)index)->val);
	else if (arr->type == VALUE_STRING)
	{
		std::string v = "";
		v += ((StringValue*)arr)->val[((IntegerValue*)index)->val];
		val = new StringValue(v);
	}

	Value * ret = nullptr;

	if (val->type == VALUE_INTEGER)
	{
		ret = new IntegerValue(*(IntegerValue*)val);
	}
	else if (val->type == VALUE_BOOL)
	{
		ret = new BoolValue(*(BoolValue*)val);
	}
	else if (val->type == VALUE_ARRAY)
	{
		ret = new ArrayValue(*(ArrayValue*)val);
	}
	else if (val->type == VALUE_STRING)
	{
		ret = new StringValue(*(StringValue*)val);
		delete val;
	}

	delete arr;
	delete index;

	return ret;
}

Value * Interpreter::eval(FuncCallExp * exp)
{
	Function * func = (Function*)globalSpace->getMemoryChunk(exp->id);

	currentSpace = new MemorySpace();
	callstack.push(currentSpace);

	for (unsigned int i = 0; i < exp->args.size(); i++)
	{
		Value * val = eval(exp->args.at(i));
		currentSpace->addMemoryChunk(new Variable(func->args.at(i), val));
	}

	Value * val = nullptr;

	try
	{
		for (unsigned int i = 0; i < func->body.size(); i++)
		{
			exec(func->body.at(i));
		}
	}
	catch (Value * v)
	{
		val = v;
	}

	callstack.pop();

	if (callstack.size() == 0)
		currentSpace = globalSpace;
	else
		currentSpace = callstack.top();

	if (val == nullptr)
	{
		std::cout << "Function did not return anything" << std::endl;
	}

	return val;
}

Value * Interpreter::eval(VarExp * exp)
{
	Variable * var = (Variable*) getMemoryChunk(exp->id);

	return var->value;
}

void Interpreter::exec(Stmt * stmt)
{
	switch (stmt->type)
	{
	case NODE_FUNCDEF_STMT:
		exec(FUNCDEF_STMT(stmt));
		break;

	case NODE_VARDEF_STMT:
		exec(VARDEF_STMT(stmt));
		break;

	case NODE_VARASS_STMT:
		exec(VARASS_STMT(stmt));
		break;

	case NODE_FUNCCALL_STMT:
		exec(FUNCCALL_STMT(stmt));
		break;

	case NODE_IF_STMT:
		exec(IF_STMT(stmt));
		break;

	case NODE_PRINT_STMT:
		exec(PRINT_STMT(stmt));
		break;

	case NODE_RETURN_STMT:
		exec(RETURN_STMT(stmt));
		break;

	default:
		std::cout << "No way, stmt not?" << std::endl;
	}
}

void Interpreter::exec(FuncDefStmt * stmt)
{
	globalSpace->addMemoryChunk(new Function(stmt->id, stmt->args, stmt->body));
	std::cout << "Size: " << stmt->body.size() << std::endl;
}

void Interpreter::exec(VarDefStmt * stmt)
{
	Value * val = eval(stmt->exp);
	currentSpace->addMemoryChunk(new Variable(stmt->id, val));
}

void Interpreter::exec(VarAssStmt * stmt)
{
	Variable * var = (Variable*) getMemoryChunk(stmt->id);

	Value * val = eval(stmt->exp);
	var->value = val;
}

void Interpreter::exec(FuncCallStmt * stmt)
{
	eval(stmt->funcall);
}

void Interpreter::exec(IfStmt * stmt)
{
	Value * cond = eval(stmt->cond);

	if (cond->type != VALUE_BOOL)
	{
		std::cout << "Cond is not bool" << std::endl;
	}

	if (((BoolValue*)cond)->val)
	{
		for (unsigned int i = 0; i < stmt->body.size(); i++)
		{
			exec(stmt->body.at(i));
		}
	}
}

void Interpreter::exec(PrintStmt * stmt)
{
	Value * val = eval(stmt->exp);

	if (val->type == VALUE_ARRAY)
		std::cout << ((ArrayValue*)val) << std::endl;
	else if (val->type == VALUE_BOOL)
		std::cout << ((BoolValue*)val) << std::endl;
	else if (val->type == VALUE_INTEGER)
		std::cout << ((IntegerValue*)val) << std::endl;
	else if (val->type == VALUE_STRING)
		std::cout << ((StringValue*)val) << std::endl;
	else
		std::cout << "WAAAT" << std::endl;

	delete val;
}

void Interpreter::exec(ReturnStmt * stmt)
{
	Value * val = eval(stmt->exp);

	throw val;
}

MemoryChunk * Interpreter::getMemoryChunk(std::string id)
{
	MemoryChunk * mem = currentSpace->getMemoryChunk(id);
	if (mem == nullptr)
	{
		return globalSpace->getMemoryChunk(id);
	}

	return mem;
}