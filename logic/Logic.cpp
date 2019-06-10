#include<logic/Logic.h>

Logic::Logic(string name, int code, tableValue im)
{
	valName = name;
	opcode = code;
	immediate = im;
}
Logic::~Logic()
{

}
int Logic::compareChar(char*x, char*y, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (x[i] == '\0'&&y[i] != '\0')return LESS_THAN;
		else if (x[i] == '\0'&&y[i] == '\0')return EQUAL;
		else if (x[i] != '\0'&&y[i] == '\0')return GREATER_THAN;
		
		if (x[i] > y[i])return GREATER_THAN;
		else if (x[i] < y[i])return LESS_THAN;
	}
	return EQUAL;
}
int Logic::compareInt(int x, int y)
{
	if (x == y)return EQUAL;
	else if (x > y)return GREATER_THAN;
	else return LESS_THAN;
}
int Logic::compareFloat(float x, float y)
{
	if (x == y)return EQUAL;
	else if (x > y)return GREATER_THAN;
	else return LESS_THAN;
}
bool Logic::checkCondition(int result)
{
	switch (opcode)
	{
		case EQUAL :
			if (result != EQUAL)return false;
			return true;
		case NOT_EQUAL:
			if (result == EQUAL)return false;
			return true;
		case GREATER_THAN:
			if (result != GREATER_THAN) return false;
			return true;
		case LESS_THAN:
			if (result != LESS_THAN)return false;
			return true;
		case GREATER_EQUAL:
			if (result == LESS_THAN)return false;
			return true;
		case LESS_EQUAL:
			if (result == GREATER_THAN)return false;
			return true;
	}
	return false;
}