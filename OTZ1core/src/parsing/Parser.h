#pragma once

#include "../lexing/Token.h"
#include<string>
#include "../ast/Exp.h"

class Parser
{
protected:
	token_list * tokens;
	Token lookahead = Token(TOKEN_EPSILON);
	unsigned int index;

public:
	Parser(token_list *);

	void parse();

	Exp * parse_exp();

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
	
};