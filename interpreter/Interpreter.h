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
#include<interpreter/deleteRecord.h>
#include<table/Table.h>
#include<fstream>
using namespace std;

extern systemAPI * api;

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

	
	static void Execute(string x)
	{
		if (x.find("execfile") == 0)
		{
			x = x.substr(8);
			stringProcesser::trim(x);
			exeFile(x);
			return;
		}
		else if (x.find("quit") == 0)
		{
			if (x.length() != 4)
			{
				syntaxError::Error();
				return;
			}
			else
			{
				cout << "Good Bye!!" << endl;
				exit(0);
				return;
			}
		}


		string y = x;
		int head = x.find_first_of('(');

		if (head != -1)
		{
			y = x.substr(0, head);
			int tail = x.find_last_of(')');
			if (tail == -1)
			{
				syntaxError::Error();
				return;
			}
			x = x.substr(head + 1, tail - head-1);
			vector<string> temp = stringProcesser::split(y, " ");
			int size = temp.size();
			for (int k = 0; k < size; k++)stringProcesser::trim(temp[k]);

			if (temp[0] == "create")
			{
				if (size == 3 && temp[1] == "table")
				{
					//cout << "aaaa!!" << endl;
					if (createTable::create(temp[2], x))createTableResult(true, temp[2]);
					else createTableResult(false, temp[2]);
					return;
				}
				else if (size == 5 && temp[1] == "index" && temp[3] == "on")
				{
					//cout << "aha??" << endl;
					if (createIndex::create(temp[2], temp[4],x))createIndexResult(true, temp[2]);
					else createIndexResult(false, temp[2]);
					return;
				}
				else
				{
					syntaxError::Error();
					return;
				}
			}
			else if (temp[0] == "insert")
			{
				if (size == 4 && temp[1] == "into" &&temp[3]=="values")
				{
					if (insertRecord::insert(temp[2], x))insertResult(true);
					else insertResult(false);
					return;
				}
				else
				{
					syntaxError::Error();
					return;
				}
			}
		}
		else
		{
			vector<string> temp = stringProcesser::split(x, " ");
			int size = temp.size();
			for (int k = 0; k < size; k++)stringProcesser::trim(temp[k]);

			if (size == 0)
			{
				syntaxError::Error();
				return;
			}
			else
			{
				if (temp[0] == "drop")
				{
					if (size != 3)
					{
						syntaxError::Error();
						return;
					}
					else if (temp[1] == "table")
					{
						if (dropTable::drop(temp[2]))dropTableResult(true, temp[2]);
						else dropTableResult(false, temp[2]);
						return;
					}
					else if (temp[1] == "index")
					{
						if (dropIndex::drop(temp[2]))dropIndexResult(true, temp[2]);
						else dropIndexResult(false, temp[2]);
						return;
					}
					else
					{
						syntaxError::Error();
						return;
					}
				}
				else if(size>=3)
				{
					if (temp[0] == "select"&&temp[2]=="from")
					{
						int pos = x.find("where");
						string conditionString;

						if (pos == -1)
						{
							if (size != 4) { syntaxError::Error(); return; }
							conditionString = "";
						}
						else
						{
							if (size <= 5 || temp[4] != "where") { syntaxError::Error(); return; }
							conditionString = x.substr(pos + 5);
						}
						selectResult(Select::get(temp[3], conditionString),temp[3]);
						return;
					}
					else if (temp[0] == "delete"&&temp[1]=="from")
					{
						string conditionString;
						if (size == 3)conditionString = "";
						else
						{
							if (size <= 4 || temp[3] != "where") { syntaxError::Error(); return; }
							int loc = x.find("where");
							conditionString = x.substr(loc + 5);
						}
						deleteResult(deleteRecord::Delete(temp[2], conditionString));
						return;
					}
					else
					{
						syntaxError::Error();
						return;
					}
				}
			}
		}
		return;
	}

private:
	static void createTableResult(bool success,string tableName)
	{
		cout << endl;
		if (success)cout << "[Done] Successfully create table : " << tableName << endl;
		else cout << "[Failed] Check again !!!" << endl;
		cout << endl;
	}
	static void createIndexResult(bool success, string indexName)
	{
		cout << endl;
		if (success)cout << "[Done] Successfully create index : " << indexName<< endl;
		else cout << "[Failed] Check again !!!" << endl;
		cout << endl;
	}
	static void insertResult(bool success)
	{
		cout << endl;
		if (success)cout << "[Done] Insertion succeed !!! " << endl;
		else cout << "[Failed] Check again !!!" << endl;
		cout << endl;
	}
	static void dropTableResult(bool success, string tableName)
	{
		cout << endl;
		if (success)cout << "[Done] Successfully drop table : " << tableName << endl;
		else cout << "[Failed] Check again !!!" << endl;
		cout << endl;
	}
	static void dropIndexResult(bool success, string indexName)
	{
		cout << endl;
		if (success)cout << "[Done] Successfully drop index : " << indexName << endl;
		else cout << "[Failed] Check again !!!" << endl;
		cout << endl;
	}
	static void selectResult(vector<vector<tableValue>*> *result,string tableName)
	{
		cout << endl;
		if(result==NULL)cout << "[Failed] Check again !!!" << endl;
		else
		{
			Table *table = api->getTable(tableName);
			vector<dbDataType*> * attrList = table->attrList;
			dbDataType * temp;
			int size = attrList->size();
			cout << tableName << "_schema(";
			for (int i = 0; i < size; i++)
			{
				temp = (*attrList)[i];
				cout << temp->name;
				if(i!=size-1)cout<< " , ";
			}
			cout << ");";
			cout << endl;

			int cnt = result->size();
			vector<tableValue>* tempValue;
	
			for(int i=0;i<cnt;i++)
			{
				tempValue = (*result)[i];
				for (int j = 0; j < size; j++)
				{
					temp = (*attrList)[j];
					switch (temp->dbType)
					{
					case DB_INT:
						cout << (*tempValue)[j].INT << "   ";
						break;
					case DB_FLOAT:
						cout << (*tempValue)[j].FLOAT << "   ";
						break;
					case DB_CHAR:
						cout << (*tempValue)[j].CHAR << "   ";
						break;
					}
				}
				cout << endl;
			}
		}
		cout << endl;
	}
	static void deleteResult(int cnt)
	{
		cout << endl;
		if (cnt==-1)cout << "[Failed] Check again !!!" << endl;
		else
		{
			cout << "[Done] "<<cnt<<" records have been deleted" << endl;
		}
		cout << endl;
	}
	static void exeFile(string path)
	{
		ifstream infile;
		infile.open(path, ios::in);
		if (infile.fail())
		{
			cout << endl;
			cout << "[Failed] Can't find the target file : "<<path << endl;
			cout << endl;
			return;
		}

		string x, y;
		x = "";
		while (getline(infile,y))
		{
			stringProcesser::trim(y);
			if (y.empty())continue;
			x += y;
			if (y[y.size() - 1] == ';')
			{
				int len = x.size();
				for (int i = 0; i < len; i++)
				{
					if (x[i] >= 'A' && x[i] <= 'Z')x[i] += 32;
					else if (x[i] == '	' || x[i] == '	')x[i] = ' ';
				}
				stringProcesser::trim(x);
				x = x.substr(0, x.size() - 1);
				stringProcesser::trim(x);
				Execute(x);
				x = "";
			}
		}
		if (x != "")
		{
			cout << endl;
			cout << "[Failed] Syntax Error!!" << endl;
			cout << endl;
		}
	}
};

#endif // !Interpreter_H
