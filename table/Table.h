#ifndef Table_H
#define Table_H

#include<config.h>
#include<index/Index.h>
#include<vector>

using namespace std;
struct dbDataType
{
	int dbType;
	int n;
	string name;
	bool unique;
	bool primary;
	bool hasIndex;
	dbDataType(int inType, int inN, string inName,bool inUnique = false, bool inPrimary = false,bool inIndex=false)
	{
		dbType = inType;
		n = inN;
		unique = inUnique;
		primary = inPrimary;
		name = inName;
		hasIndex = inIndex;
	}
	int getKeyLength()
	{
		if (dbType == DB_INT || dbType == DB_FLOAT) return 4;
		else return n;
	}
};

struct tableValue
{
	int INT;
	float FLOAT;
	char* CHAR;
};

class Table
{
public:
	string name;
	int colCnt;
	int primaryKey;
	int instanceNumPerBlock;
	int sizePerInstance;
	vector<dbDataType*> * attrList;
	vector<string> * attributesHaveIndex;
	Table(string tableName, vector<dbDataType*>*attr, vector<string> * index);
	dbDataType* findAttrByName(string name);
	dbDataType* findAttrByName(const char* name);
};

#endif // !Table_H
