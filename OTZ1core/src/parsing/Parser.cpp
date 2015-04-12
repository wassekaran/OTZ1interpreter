#include "Parser.h"
#include<iostream>

void Parser::match(std::string id)
{
	if (lookahead.value == id)
	{
		nextToken();
	}
	else
	{
		std::cout << id << " expected" << std::endl;
	}
}

void Parser::match(TokenType type)
{
	if (lookahead.type == type)
	{
		nextToken();
	}
	else
	{
		std::cout << type << " expected" << std::endl;
	}
}

void Parser::nextToken()
{
	if (index < tokens->size())
	{
		lookahead = tokens->at(index);
		index++;
	}
	else
	{
		lookahead = Token(TOKEN_EPSILON);
	}
}

Parser::Parser(token_list * tokens)
{
	this->tokens = tokens;
	this->index = 0;
	nextToken();
}

void Parser::parse()
{
	
}

// EXPRESSION PARSING //////////////////////
/*
Grammar:

Exp -> TernaryExp
TernaryExp -> BoolExp '?' TernaryExp ':' TernaryExp
BoolExp -> RelExp ('and' | 'or') BoolExp
RelExp -> BinaryExp RelOp BinaryExp
BinaryExp -> BinaryTerm ('+' | '-') BinaryExp
BinaryTerm -> Atom ('*' | '/' | '%') BinaryTerm
Atom -> ('(' Exp ')') | (TRUE | FALSE) | (INTEGER)
*/

Exp * Parser::parse_exp()
{
	return parse_ternary_exp();
}

Exp * Parser::parse_ternary_exp()
{
	Exp * cond = parse_bool_exp();

	if (cond == nullptr)
		return nullptr;

	if (lookahead.type == TOKEN_QUESTION_MARK)
	{
		nextToken();

		Exp * left = parse_ternary_exp();
		
		if (left == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		match(TOKEN_COLON);

		Exp * right = parse_ternary_exp();

		if (right == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		return new TernaryExp(cond, left, right);
	}

	return cond;
}

Exp * Parser::parse_bool_exp()
{
	Exp * left = parse_rel_exp();

	if (left == nullptr)
		return nullptr;

	if (lookahead.type == TOKEN_AND || lookahead.type == TOKEN_OR)
	{
		TokenType op = lookahead.type;
		nextToken();

		Exp * right = parse_bool_exp();

		if (right == nullptr)
		{
			std::cout << "Expression exprected" << std::endl;
		}

		return new BoolExp(left, op, right);
	}

	return left;
}

Exp * Parser::parse_rel_exp()
{
	Exp * left = parse_binary_exp();

	if (left == nullptr)
		return nullptr;

	if (lookahead.type == TOKEN_EQUALS || lookahead.type == TOKEN_NOT_EQUALS ||
		lookahead.type == TOKEN_GREATER_THAN || lookahead.type == TOKEN_LESS_THAN ||
		lookahead.type == TOKEN_GOE_THAN || lookahead.type == TOKEN_LOE_THAN)
	{
		TokenType op = lookahead.type;
		nextToken();

		Exp * right = parse_binary_exp();

		if (right == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		return new RelExp(left, op, right);
	}

	return left;
}

Exp * Parser::parse_binary_exp()
{
	Exp * left = parse_binary_term();

	if (left == nullptr)
		return nullptr;

	if (lookahead.type == TOKEN_PLUS || lookahead.type == TOKEN_MINUS)
	{
		TokenType op = lookahead.type;
		nextToken();

		Exp * right = parse_binary_exp();

		if (right == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		return new BinaryExp(left, op, right);
	}

	return left;
}

Exp * Parser::parse_binary_term()
{
	Exp * left = parse_atom();

	if (left == nullptr)
		return nullptr;

	if (lookahead.type == TOKEN_STAR || lookahead.type == TOKEN_SLASH || lookahead.type == TOKEN_PERCENT)
	{
		TokenType op = lookahead.type;
		nextToken();

		Exp * right = parse_binary_term();

		if (right == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		return new BinaryTerm(left, op, right);
	}

	return left;
}

Exp * Parser::parse_atom()
{
	Exp * result = nullptr;
	if (lookahead.type == TOKEN_LEFT_PAR)
	{
		nextToken();

		Exp * exp = parse_exp();

		if (exp == nullptr)
		{
			std::cout << "Expression expected" << std::endl;
		}

		match(TOKEN_RIGHT_PAR);

		result = exp;
	}
	else if (lookahead.type == TOKEN_INTEGER)
	{
		int val = atoi(lookahead.value.c_str());
		nextToken();
		result = new ValueExp(new IntegerValue(val));
	}
	else if (lookahead.type == TOKEN_TRUE)
	{
		nextToken();
		result = new ValueExp(new BoolValue(true));
	}
	else if (lookahead.type == TOKEN_FALSE)
	{
		nextToken();
		result = new ValueExp(new BoolValue(false));
	}
	else if (lookahead.type == TOKEN_LEFT_SQBR)
	{
		nextToken();

		std::vector<Exp*> expList;
		Exp * exp = parse_exp();
		if (exp != nullptr)
		{
			expList.push_back(exp);
			while (lookahead.type == TOKEN_COMA)
			{
				nextToken();
				exp = parse_exp();
				
				if (exp == nullptr)
				{
					std::cout << "Expression expected" << std::endl;
				}

				expList.push_back(exp);
			}
		}

		match(TOKEN_RIGHT_SQBR);

		result = new ArrayExp(expList);
	}
	else if (lookahead.type == TOKEN_STRING)
	{
		std::string val = lookahead.value;
		nextToken();
		
		result = new ValueExp(new StringValue(val));
	}

	if (result != nullptr)
	{
		if (lookahead.type == TOKEN_LEFT_SQBR)
		{
			nextToken();

			Exp * index = parse_exp();
			if (index == nullptr)
			{
				std::cout << "Expression expected" << std::endl;
			}

			match(TOKEN_RIGHT_SQBR);

			Exp * out = new ArrayElementExp(result, index);
			
			while (lookahead.type == TOKEN_LEFT_SQBR)
			{
				nextToken();

				index = parse_exp();
				if (index == nullptr)
				{
					std::cout << "Expression expected" << std::endl;
				}

				match(TOKEN_RIGHT_SQBR);

				out = new ArrayElementExp(out, index);
			}

			return out;
		}

		return result;
	}

	return nullptr;
}