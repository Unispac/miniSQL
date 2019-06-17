#ifndef insertRecord_H
#define insertRecord_H

#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;

class insertRecord
{
public:
	static bool insert(string tableName, string value)
	{
		/*
		cout << value << endl;
		int head = value.find_first_of("(");
		int tail = value.find_last_of(")");
		cout << head << " " << tail << value.substr(0, head) << endl;
		if (head==-1||tail==-1||value.substr(0, head) != "values") { syntaxError::Error(); return false; }
		cout << "??" << endl;
		value = value.substr(head + 1, tail - head - 1);*/
		vector<string> vList = stringProcesser::split(value, ",");
		return api->insert(tableName, vList);
	}
	
};

#endif // !InsertRecord_H
