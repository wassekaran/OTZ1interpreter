#include "Parser.h"
#include<iostream>
#include<sstream>

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

stmt_list Parser::parse()
{
	return parse_program();
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

		exp_list expList = parse_exp_list();

		match(TOKEN_RIGHT_SQBR);

		result = new ArrayExp(expList);
	}
	else if (lookahead.type == TOKEN_STRING)
	{
		std::string val = lookahead.value;
		nextToken();
		
		result = new ValueExp(new StringValue(val));
	}
	else if (lookahead.type == TOKEN_IDENTIFIER)
	{
		std::string id = lookahead.value;
		nextToken();

		if (lookahead.type == TOKEN_LEFT_PAR)
		{
			nextToken();

			result = parse_funccall_exp(id);
		}
		else
		{
			if (scopeTree.reference("var " + id) == nullptr)
			{
				std::cout << "Undefined variable" << std::endl;
			}
			result = new VarExp("var " + id);
		}
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

exp_list Parser::parse_exp_list()
{
	exp_list expList;
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

	return expList;
}

Exp * Parser::parse_funccall_exp(std::string id)
{
	exp_list args;

	Exp * exp = parse_exp();
	if (exp != nullptr)
	{
		args.push_back(exp);

		while (lookahead.type == TOKEN_COMA)
		{
			nextToken();

			exp = parse_exp();
			if (exp == nullptr)
			{
				std::cout << "Expression expected" << std::endl;
			}

			args.push_back(exp);
		}
	}

	match(TOKEN_RIGHT_PAR);

	std::ostringstream oss;
	oss << args.size();

	if (scopeTree.reference("func " + id + " " + oss.str()) == nullptr)
	{
		std::cout << "Func not defined" << std::endl;
		scopeTree.dump();
		std::cout << "-----------" << std::endl;
	}

	return new FuncCallExp("func " + id + " " + oss.str(), args);
}

// STATEMENT PARSING /////////////////////////////
/*
Grammar:
Program -> [{ Statement }]
Stmt -> FuncDefStmt | ImportStmt | InnerStmt
FuncDefStmt -> 'func' IDENTIFIER '(' ArgList ')' InnerStmtBlock
ArgList -> [ IDENTIFIER [{ ',' IDENTIFIER }]]
InnerStmtBlock -> InnerStmt | ( '{' [{ InnerStmt }] '}' )
ImportStmt -> 'needs' STRING
InnerStmt -> FuncCallStmt | VarAssStmt | IfStmt | WhileStmt | ForStmt
FuncCallStmt -> IDENTIFIER '(' ExpList ')'
VarAssStmt -> IDENTIFIER [ '[' Exp ']' ] '=' Exp
IfStmt -> 'if' Exp InnerStmtBlock [{ 'elif' Exp InnerStmtBlock }] [ 'else' InnerStmtBlock ]
WhileStmt -> 'while' Exp InnerStmtBlock
ForStmt -> 'for' IDENTIFIER 'in' Exp InnerStmtBlock
*/

stmt_list Parser::parse_program()
{
	stmt_list list;
	Stmt * stmt = nullptr;

	while ((stmt = parse_stmt()) != nullptr)
	{
		list.push_back(stmt);
	}

	return list;
}

Stmt * Parser::parse_stmt()
{
	if (lookahead.type == TOKEN_NEEDS)
	{
		nextToken();

		return parse_import_stmt();
	}
	else if (lookahead.type == TOKEN_FUNC)
	{
		nextToken();

		return parse_funcdef_stmt();
	}
	
	return parse_inner_stmt();
}

Stmt * Parser::parse_funcdef_stmt()
{
	if (lookahead.type == TOKEN_IDENTIFIER)
	{
		std::string id = lookahead.value;
		nextToken();

		match(TOKEN_LEFT_PAR);

		std::vector<std::string> args;

		if (lookahead.type == TOKEN_IDENTIFIER)
		{
			args.push_back(lookahead.value);

			nextToken();

			while (lookahead.type == TOKEN_COMA)
			{
				nextToken();

				if (lookahead.type != TOKEN_IDENTIFIER)
				{
					std::cout << "Identifier expected" << std::endl;
				}

				args.push_back(lookahead.value);
				nextToken();
			}
		}

		match(TOKEN_RIGHT_PAR);

		std::ostringstream oss;
		oss << args.size();

		scopeTree.declare("func " + id + " " + oss.str());

		scopeTree.push();
		for (unsigned int i = 0; i < args.size(); i++)
		{
			scopeTree.declare("var " + args.at(i));
		}

		stmt_list body = parse_inner_stmt_block();
		scopeTree.pop();

		return new FuncDefStmt("func " + id + " " + oss.str(), args, body);
	}
	else
	{
		std::cout << "Ident expected" << std::endl;
	}

	return nullptr;
}

Stmt * Parser::parse_import_stmt()
{
	std::cout << "Import not yet" << std::endl;
	return nullptr;
}

Stmt * Parser::parse_inner_stmt()
{
	if (lookahead.type == TOKEN_IDENTIFIER)
	{
		std::string id = lookahead.value;
		nextToken();

		if (lookahead.type == TOKEN_LEFT_PAR)
		{
			nextToken();
			return parse_funccall_stmt(id);
		}

		return parse_varass_stmt(id);
	}
	else if (lookahead.type == TOKEN_IF)
	{
		nextToken();

		return parse_if_stmt();
	}
	else if (lookahead.type == TOKEN_WHILE)
	{
		nextToken();

		return parse_while_stmt();
	}
	else if (lookahead.type == TOKEN_FOR)
	{
		nextToken();

		return parse_for_stmt();
	}
	else if (lookahead.type == TOKEN_PRINT)
	{
		nextToken();

		return parse_print_stmt();
	}
	else if (lookahead.type == TOKEN_RETURN)
	{
		nextToken();

		return parse_return_stmt();
	}

	return nullptr;
}

Stmt * Parser::parse_varass_stmt(std::string id)
{
	match(TOKEN_ASSIGN);

	Exp * exp = parse_exp();

	if (exp == nullptr)
	{
		std::cout << "Expression expected" << std::endl;
	}

	if (scopeTree.reference("var " + id) == nullptr)
	{
		scopeTree.declare("var " + id);
		return new VarDefStmt("var " + id, exp);
	}

	return new VarAssStmt("var " + id, exp);
}

Stmt * Parser::parse_funccall_stmt(std::string id)
{
	Exp * exp = parse_funccall_exp(id);

	return new FuncCallStmt(exp);
}

Stmt * Parser::parse_if_stmt()
{
	Exp * cond = parse_exp();

	if (cond == nullptr)
	{
		std::cout << "Condition expected" << std::endl;
	}

	stmt_list body = parse_inner_stmt_block();

	return new IfStmt(cond, body);
}

Stmt * Parser::parse_while_stmt()
{
	Exp * cond = parse_exp();

	if (cond == nullptr)
	{
		std::cout << "Condition expected" << std::endl;
	}

	stmt_list body = parse_inner_stmt_block();

	return new WhileStmt(cond, body);
}

Stmt * Parser::parse_for_stmt()
{
	return nullptr;
}

Stmt * Parser::parse_print_stmt()
{
	Exp * exp = parse_exp();

	if (exp == nullptr)
	{
		std::cout << "Expression expected" << std::endl;
	}

	return new PrintStmt(exp);
}

Stmt * Parser::parse_return_stmt()
{
	Exp * exp = parse_exp();

	if (exp == nullptr)
	{
		std::cout << "Expression expected" << std::endl;
	}

	return new ReturnStmt(exp);
}

stmt_list Parser::parse_inner_stmt_block()
{
	scopeTree.push();

	if (lookahead.type == TOKEN_BEGIN)
	{
		nextToken();

		stmt_list list;

		Stmt * stmt = nullptr;
		while ((stmt = parse_stmt()) != nullptr)
		{
			list.push_back(stmt);
		}

		match(TOKEN_END);

		scopeTree.pop();

		return list;
	}

	Stmt * stmt = parse_stmt();
	if (stmt == nullptr)
	{
		std::cout << "Statement expected" << std::endl;
	}

	stmt_list list;
	list.push_back(stmt);

	scopeTree.pop();

	return list;
}