#pragma once

#include<string>
#include "Token.h"

class Lexer
{
protected:
	char * path;

public:
	Lexer(char * path);
	token_list * tokenize();

protected:
	void tokenizeLine(std::string line, token_list * list);
};