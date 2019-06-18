#ifndef Interpreter_H
#define Interpreter_H

#include<config.h>
#include<iostream>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
#include<cstring>
#include<interpreter\createTable.h>
#include<interpreter/insertRecord.h>
#include<interpreter/dropTable.h>
#include<interpreter/dropIndex.h>
#include<interpreter/createIndex.h>
#include<interpreter\Select.h>
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
			stringProcesser::trim(y);
			if (y.empty())continue;
			x += y;
			if (y[y.size() - 1] == ';')break;
		}
		int len = x.size();
		for (int i = 0; i < len; i++)
		{
			if (x[i] >= 'A' && x[i] <= 'Z')x[i] += 32;
			else if (x[i] == '	'||x[i]=='	')x[i] = ' ';
		}
		stringProcesser::trim(x);
		x = x.substr(0, x.size() - 1);
		stringProcesser::trim(x);
		return x;
	}

	static string getType(string x)
	{
		string y = x;
		int head = x.find_first_of('(');

		if (head != -1)
		{
			y = x.substr(0, head);
			int tail = x.find_last_of(')');
			if (tail == -1)return "syntax error";
			x = x.substr(head + 1, tail - head-1);
			vector<string> temp = stringProcesser::split(y, " ");
			int size = temp.size();
			for (int k = 0; k < size; k++)stringProcesser::trim(temp[k]);

			if (temp[0] == "create")
			{
				if (size == 3 && temp[1] == "table")
				{
					if(createTable::create(temp[2], x))return "create table";
					else return "syntax error";
				}
				else if (size == 5 && temp[1] == "index" && temp[3] == "on")
				{
					if(createIndex::create(temp[2],temp[4]))return "create index";
					else return "syntax error";
				}
				else return "syntax error";
			}
			else if (temp[0] == "insert")
			{
				if (size == 4 && temp[1] == "into" &&temp[3]=="values")
				{
					if(insertRecord::insert(temp[2], x))return "insert record";
					else return "syntax error";
				}
				else return "syntax error";
			}
		}
		else
		{
			vector<string> temp = stringProcesser::split(x, " ");
			int size = temp.size();
			for (int k = 0; k < size; k++)stringProcesser::trim(temp[k]);

			if (size == 0)return "syntax error";
			else
			{
				if (temp[0] == "drop")
				{
					if (size != 3)return "syntax error";
					else if (temp[1] == "table")
					{
						if(dropTable::drop(temp[2]))return "drop table";
						else return "syntax error";
					}
					else if (temp[1] == "index")
					{
						if(dropIndex::drop(temp[2]))return "drop index";
						else return "syntax error";
					}
					else return "syntax error";
				}
				else
				{
					if (temp[0] == "select")
					{
						if (temp[2] != "from"||temp[4]!="where")return "syntax error";
						int pos = x.find("where");
						if (Select::get(temp[3], x.substr(pos + 5)) != NULL)return "select";
						else return "syntax error";
					}
					else if (temp[0] == "delete")
					{

					}
					else return "syntax error";
				}
			}
		}

		

		return "";
	}
};

#endif // !Interpreter_H
