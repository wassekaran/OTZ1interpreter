#pragma once

#include<string>
#include<vector>

typedef enum TokenType
{
	TOKEN_IDENTIFIER, TOKEN_EPSILON,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_PERCENT,
	TOKEN_INTEGER,
	TOKEN_LEFT_PAR, TOKEN_RIGHT_PAR,
	TOKEN_COLON, TOKEN_QUESTION_MARK,
	TOKEN_AND, TOKEN_OR, TOKEN_ASSIGN, TOKEN_NOT,
	TOKEN_EQUALS, TOKEN_NOT_EQUALS, TOKEN_GREATER_THAN, TOKEN_LESS_THAN, TOKEN_GOE_THAN, TOKEN_LOE_THAN,
	TOKEN_TRUE, TOKEN_FALSE,
	TOKEN_LEFT_SQBR, TOKEN_RIGHT_SQBR,
	TOKEN_COMA, TOKEN_STRING
};

struct Token
{
	TokenType type;
	std::string value;

	Token(TokenType type)
	{
		this->type = type;
	}

	Token(TokenType type, std::string value)
	{
		this->type = type;
		this->value = value;
	}

};

typedef std::vector<Token> token_list;