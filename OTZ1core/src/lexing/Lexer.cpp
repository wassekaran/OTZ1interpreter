#include "Lexer.h"
#include<fstream>
#include<iostream>

Lexer::Lexer(char * path)
{
	this->path = path;
}

token_list * Lexer::tokenize()
{
	std::ifstream infile(path);

	if (infile)
	{
		token_list * tokens = new token_list();

		std::string line;
		while (std::getline(infile, line))
		{
			tokenizeLine(line, tokens);
		}

		return tokens;
	}
	else
	{
		std::cout << "Could not open file: " << path << std::endl;
		return nullptr;
	}
}

void Lexer::tokenizeLine(std::string line, token_list * list)
{
	unsigned int i = 0;

	while (i < line.size())
	{
		switch (line[i])
		{
		// Comment
		case '#':
			return;

		// Whitespaces
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			i++;
			break;

		case '+':
			list->push_back(Token(TOKEN_PLUS));
			i++;
			break;

		case '-':
			list->push_back(Token(TOKEN_MINUS));
			i++;
			break;

		case '*':
			list->push_back(Token(TOKEN_STAR));
			i++;
			break;

		case '/':
			list->push_back(Token(TOKEN_SLASH));
			i++;
			break;

		case '(':
			list->push_back(Token(TOKEN_LEFT_PAR));
			i++;
			break;

		case ')':
			list->push_back(Token(TOKEN_RIGHT_PAR));
			i++;
			break;

		case ':':
			list->push_back(Token(TOKEN_COLON));
			i++;
			break;

		case '?':
			list->push_back(Token(TOKEN_QUESTION_MARK));
			i++;
			break;

		case '%':
			list->push_back(Token(TOKEN_PERCENT));
			i++;
			break;

		case '[':
			list->push_back(Token(TOKEN_LEFT_SQBR));
			i++;
			break;

		case ']':
			list->push_back(Token(TOKEN_RIGHT_SQBR));
			i++;
			break;

		case ',':
			list->push_back(Token(TOKEN_COMA));
			i++;
			break;

		case '{':
			list->push_back(Token(TOKEN_BEGIN));
			i++;
			break;

		case '}':
			list->push_back(Token(TOKEN_END));
			i++;
			break;

		case '\'':
		{
			std::string val = "";
			i++;
			while (i < line.size() && line[i] != '\'')
			{
				val += line[i];
				i++;
			}

			if (line[i] == '\'')
			{
				list->push_back(Token(TOKEN_STRING, val));
				i++;
			}
			else
			{
				std::cout << "' expected, got: " << line[i] << " code: " << ((int)line[i]) << std::endl;
			}

			break;
		}

		case '=':
			i++;
			if (i < line.size() && line[i] == '=')
			{
				list->push_back(Token(TOKEN_EQUALS));
				i++;
			}
			else
			{
				list->push_back(Token(TOKEN_ASSIGN));
			}
			break;

		case '!':
			i++;
			if (i < line.size() && line[i] == '=')
			{
				list->push_back(Token(TOKEN_NOT_EQUALS));
				i++;
			}
			else
			{
				list->push_back(Token(TOKEN_NOT));
			}
			break;

		case '>':
			i++;
			if (i < line.size() && line[i] == '=')
			{
				list->push_back(Token(TOKEN_GOE_THAN));
				i++;
			}
			else
			{
				list->push_back(Token(TOKEN_GREATER_THAN));
			}
			break;

		case '<':
			i++;
			if (i < line.size() && line[i] == '=')
			{
				list->push_back(Token(TOKEN_LOE_THAN));
				i++;
			}
			else
			{
				list->push_back(Token(TOKEN_LESS_THAN));
			}
			break;

		default:
			if (isdigit(line[i]))
			{
				std::string num = "";
				num += line[i];
				i++;
				while (i < line.size() && isdigit(line[i]))
				{
					num += line[i];
					i++;
				}

				list->push_back(Token(TOKEN_INTEGER, num));
			}
			else if (isalpha(line[i]) || line[i] == '_')
			{
				std::string id = "";
				id += line[i];
				i++;

				while (i < line.size() && (isalnum(line[i]) || line[i] == '_'))
				{
					id += line[i];
					i++;
				}

				if (id == "and")
					list->push_back(Token(TOKEN_AND));
				else if (id == "or")
					list->push_back(Token(TOKEN_OR));
				else if (id == "true")
					list->push_back(Token(TOKEN_TRUE));
				else if (id == "false")
					list->push_back(Token(TOKEN_FALSE));
				else if (id == "func")
					list->push_back(Token(TOKEN_FUNC));
				else if (id == "needs")
					list->push_back(Token(TOKEN_NEEDS));
				else if (id == "if")
					list->push_back(Token(TOKEN_IF));
				else if (id == "while")
					list->push_back(Token(TOKEN_WHILE));
				else if (id == "for")
					list->push_back(Token(TOKEN_FOR));
				else if (id == "print")
					list->push_back(Token(TOKEN_PRINT));
				else if (id == "return")
					list->push_back(Token(TOKEN_RETURN));
				else
					list->push_back(Token(TOKEN_IDENTIFIER, id));
			}
			else
			{
				std::cout << "Unknown token: " << line[i] << " (code: " << (int(line[i])) << ")" << std::endl;
				return;
			}
		}
	}
}