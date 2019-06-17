#ifndef Interpreter_H
#define Interpreter_H

#include<config.h>
#include<iostream>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
#include<cstring>
using namespace std;

class Interpreter
{
public:
	static string CMD()
	{
		string x, y;
		
		while (true)
		{
			cout << ">> ";
			getline(cin, y);
			x += y;
			if (y[y.size() - 1] == ';')break;
		}
		int len = x.size();
		for (int i = 0; i < len; i++)
		{
			if (x[i] >= 'A' && x[i] <= 'Z')x[i] += 32;
			else if (x[i] == '	'||x[i]=='	')x[i] = ' ';
		}
		return x.substr(0,x.size()-1);
	}

	static string getType(string x)
	{
		string y = x;
		int head = x.find_first_of('(');

		if (head != -1)
		{
			y = x.substr(0, head);
			int tail = x.find_last_of(')');
			x = x.substr(head + 1, tail - head-1);
			vector<string> temp = stringProcesser::split(y, " ");
			int size = temp.size();

			if (temp[0] == "create")
			{
				if (size == 3 && temp[1] == "table")
				{
					return "create table";
				}
				else if (size == 5 && temp[2] == "index" && temp[4] == "on")
				{
					return "create index";
				}
				else return "syntax error";
			}
			else if (temp[0] == "insert")
			{
				if (size == 4 && temp[1] == "into"&&temp[3] == "values")
				{
					return "insert record";
				}
				else return "syntax error";
			}
		}
		else
		{
			vector<string> temp = stringProcesser::split(x, " ");
			int size = temp.size();
			if (size == 0)return "syntax error";
			else
			{
				if (temp[0] == "drop")
				{
					if (size != 3)return "syntax error";
					else if (temp[1] == "table")return "drop table";
					else if (temp[1] == "index")return "drop index";
					else return "syntax error";
				}
				else
				{
					return "select or delete";
				}
			}
		}

		

		return "";
	}
};

#endif // !Interpreter_H
