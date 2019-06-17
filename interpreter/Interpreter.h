#ifndef Interpreter_H
#define Interpreter_H

#include<config.h>
#include<iostream>

class Interpreter
{
public:
	static void CMD()
	{
		string x,y;
		while (true)
		{
			cout << ">> ";
			getline(cin, y);
			cout << "check : " << y << endl;
		}
	}
};

#endif // !Interpreter_H
