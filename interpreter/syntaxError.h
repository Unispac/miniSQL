#ifndef  syntaxError_H
#define syntaxError_H
#include<cstdlib>
#include<cstdio>
class syntaxError
{
public:
	static void Error()
	{
		cout << endl;
		cout << "[Syntax Error] Check again !!!" << endl;
		cout << endl;
		return;
		//exit(0);
	}
};
#endif // ! syntaxError_H
