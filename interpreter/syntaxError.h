#ifndef  syntaxError_H
#define syntaxError_H
#include<cstdlib>
#include<cstdio>
class syntaxError
{
public:
	static void Error()
	{
		cout << "SYTAX ERROR " << endl;
		return;
		//exit(0);
	}
};
#endif // ! syntaxError_H
