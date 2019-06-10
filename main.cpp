#include<config.h>
#include<bufferManager/bufferManager.h>
#include<catlogManager/catalogManager.h>
#include<fstream>
#include<table/Table.h>
using namespace std;

errorReporter * errorHandler;
void configInit()
{
	errorHandler = new errorReporter();
	CreateDirectory("data", NULL);
	CreateDirectory("catalog", NULL);
	if (fopen("catalog/tableNameList.mdb", "rb") == NULL)
		fopen("catalog/tableNameList.mdb", "ab+");
}
int main()
{
	configInit();
	bool x = true;
	vector<dbDataType*> *test = new vector<dbDataType*>;
	/*
		dbDataType(int inType, int inN, string inName, 
		bool inUnique = false, bool inPrimary = false, bool inIndex = false)
	*/
	test->push_back(new dbDataType(  DB_INT,1,"id", true, true, false));
	test->push_back(new dbDataType(  DB_FLOAT, 1,"float",true, false, false));
	test->push_back(new dbDataType(  DB_CHAR,500,"float_2", true, false, false));
	catalogManager * p = new catalogManager();
	p->dropTable("test3");
	system("pause");
	return 0;
}