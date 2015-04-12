#pragma once

#include "../lexing/Token.h"
#include<string>
#include "../ast/Exp.h"
#include "../ast/Stmt.h"
#include "../scope/ScopeTree.h"

class Parser
{
protected:
	token_list * tokens;
	Token lookahead = Token(TOKEN_EPSILON);
	unsigned int index;
	ScopeTree scopeTree;

public:
	Parser(token_list *);

	stmt_list parse();

	Exp * parse_exp();
	stmt_list parse_program();

protected:
	void match(std::string);
	void match(TokenType);
	void nextToken();

private:
	// Expression parsing
	Exp * parse_ternary_exp();
	Exp * parse_bool_exp();
	Exp * parse_rel_exp();
	Exp * parse_binary_exp();
	Exp * parse_binary_term();
	Exp * parse_atom();
	Exp * parse_funccall_exp(std::string);

	exp_list parse_exp_list();
	
	// Statement parsing
	Stmt * parse_stmt();
	Stmt * parse_funcdef_stmt();
	Stmt * parse_import_stmt();
	Stmt * parse_inner_stmt();
	Stmt * parse_varass_stmt(std::string);
	Stmt * parse_funccall_stmt(std::string);
	Stmt * parse_if_stmt();
	Stmt * parse_while_stmt();
	Stmt * parse_for_stmt();
	Stmt * parse_print_stmt();
	Stmt * parse_return_stmt();

	stmt_list parse_inner_stmt_block();
};