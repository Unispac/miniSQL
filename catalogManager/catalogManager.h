#ifndef  catalogManager_H
#define catalogManager_H

#include<config.h>
#include<table/Table.h>
#include<index/Index.h>
#include<set>
#include <map>
#include <vector>

using namespace std;

class catalogManager
{
public:
	catalogManager();
	~catalogManager();
	Table * getTable(string TableName);
	bool createTable(string TableName, vector<dbDataType*>*attr);
	bool dropTable(string TableName);
	bool updateTable(Table * table);

	Index * getIndexByName(string indexName);
	Index * getIndexByTableCol(string TableName, string colName);
	void getIndexByTable(string TableName, vector<Index*>* vec);
	bool createIndex(string indexName, string TableName, string colName);
	bool dropIndex(string indexName);
	bool dropIndex(string TableName, string colName);

private:
	static set <string> tableNameList;
	static map <string, Index*> indexMap;
};
#endif // ! catalogManager_H
