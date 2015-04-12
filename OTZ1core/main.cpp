#include "src\lexing\Lexer.h"
#include<iostream>
#include "src\parsing\Parser.h"
#include "src\ast\Exp.h"
#include "src\visitor\Interpreter.h"
#include "src\DebugConfig.h"

#ifdef __DEBUG_MEM_LEAK__
unsigned int _CREATES = 0;
unsigned int _DELETES = 0;
#endif

int main()
{
	Lexer lexer("testfiles/test.txt");
	token_list * tokens = lexer.tokenize();

	Parser parser(tokens);

	Exp * exp = parser.parse_exp();
	
	Interpreter * interpreter = new Interpreter();

	if (exp == nullptr)
	{
		std::cout << "No expression found" << std::endl;
	}
	else
	{
		std::cout << "An expression is found" << std::endl;
		Value * val = interpreter->eval(exp);

		if (val->type == VALUE_BOOL)
		{
			std::cout << "Boolean value = " << ((BoolValue*)val) << std::endl;
		}
		else if (val->type == VALUE_INTEGER)
		{
			std::cout << "Integer value = " << ((IntegerValue*)val) << std::endl;
		}
		else if (val->type == VALUE_ARRAY)
		{
			std::cout << "Array value = " << ((ArrayValue*)val) << std::endl;
		}
		else
		{
			std::cout << "String value = " << ((StringValue*)val) << std::endl;
		}

		delete val;
		delete exp;
	}

#ifdef __DEBUG_MEM_LEAK__
	extern unsigned int CREATES;
	extern unsigned int DELETES;

	std::cout << "Creates: " << _CREATES << std::endl;
	std::cout << "Deletes: " << _DELETES << std::endl;
#endif

	getchar();
}