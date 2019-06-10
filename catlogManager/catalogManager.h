#ifndef  catalogManager_H
#define catalogManager_H

#include<config.h>
#include<table/Table.h>
#include<index/Index.h>
#include<set>

using namespace std;

class catalogManager
{
public:
	catalogManager();
	~catalogManager();
	Table * getTable(string TableName);
	bool createTable(string TableName, vector<dbDataType*>*attr);
	bool dropTable(string TableName);

	Index * getIndex(string TableName, string attrName);
	bool createIndex(string TableName, string attrName);
	bool dropIndex(string TableName, string attrName);

private:
	set <string> tableNameList;

};
#endif // ! catalogManager_H
