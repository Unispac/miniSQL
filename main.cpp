#include<config.h>
#include<bufferManager/bufferManager.h>
#include<catlogManager/catalogManager.h>
#include<recordManager/recordManager.h>
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
	test->push_back(new dbDataType(  DB_CHAR,50,"float_2", true, false, false));
	catalogManager * p = new catalogManager();
	recordManager *r = new recordManager();
	
	p->createTable("test", test);
	r->createTable("test");
	cout << " Now you can check whether the table is created !" << endl;
	cout << " The process will pause. Click enter to continue." << endl;
	system("pause");


	//bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// ÿ��ֻ��Ҫ���һ�������ֵ���г��������ˡ�
	vector<tableValue> * value = new vector<tableValue>;
	tableValue temp;
	temp.INT = 10086;
	value->push_back(temp);
	temp.FLOAT = 585.65;
	value->push_back(temp);
	temp.CHAR = new char[50];
	memcpy(temp.CHAR, "���Ǻð�!", 50);
	value->push_back(temp);
	r->insertTableInstance("test", value);

	(*value)[0].INT = 996;
	(*value)[1].FLOAT = 19.96;
	r->insertTableInstance("test", value);

	(*value)[0].INT = 99586;
	(*value)[1].FLOAT = 1.96;
	memcpy((*value)[2].CHAR, "��һ���ַ���", 50);
	r->insertTableInstance("test", value);

	
	system("pause");
	return 0;
}