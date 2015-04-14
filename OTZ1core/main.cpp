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

#ifdef __DEBUG_EXEC_TIME__
#include<time.h>
#endif

int main()
{
	Lexer lexer("testfiles/test.txt");
	token_list * tokens = lexer.tokenize();

	Parser parser(tokens);

	stmt_list prg = parser.parse();

	Interpreter * interpreter = new Interpreter();

#ifdef __DEBUG_EXEC_TIME__
	clock_t start = clock();
#endif

	for (unsigned int i = 0; i < prg.size(); i++)
	{
		interpreter->exec(prg.at(i));
	}

#ifdef __DEBUG_EXEC_TIME__
	clock_t end = clock();

	std::cout << "Execution time: " << (end - start) << "sec" << std::endl;
#endif

#ifdef __DEBUG_MEM_LEAK__
	extern unsigned int CREATES;
	extern unsigned int DELETES;

	std::cout << "Creates: " << _CREATES << std::endl;
	std::cout << "Deletes: " << _DELETES << std::endl;
#endif

	getchar();
}