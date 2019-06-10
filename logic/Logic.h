#ifndef  logic_H
#define logic_H
#include<config.h>
#include<table/Table.h>

#define EQUAL 0
#define NOT_EQUAL 1
#define LESS_THAN 2
#define GREATER_THAN 3
#define LESS_EQUAL 4
#define GREATER_EQUAL 5

class Logic
{
public:
	string valName;
	int opcode;
	tableValue immediate;
	Logic(string name,int code,tableValue im);
	~Logic();
	bool checkCondition(int result);
	static int compareChar(char*, char*, int);
	static int compareInt(int, int);
	static int compareFloat(float, float);
};

#endif // ! logic_H
