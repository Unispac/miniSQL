#include<config.h>
#include<bufferManager/bufferManager.h>
#include<catalogManager/catalogManager.h>
#include<recordManager/recordManager.h>
#include <indexManager/indexManager.h>
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
	cout << " The process will pause. Press enter to continue." << endl;
	system("pause");


	//bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// 每次只需要添加一条表项，将值表列出来就行了。
	vector<tableValue> * value = new vector<tableValue>;
	tableValue temp;
	temp.INT = 10086;
	value->push_back(temp);
	temp.FLOAT = 585.65;
	value->push_back(temp);
	temp.CHAR = new char[50];
	memcpy(temp.CHAR, "你们好啊!", 50);
	value->push_back(temp);

	
	r->insertTableInstance("test", value);

	(*value)[0].INT = 996;
	(*value)[1].FLOAT = 19.96;
	memcpy((*value)[2].CHAR, "另一种字符CHUAN", 50);
	r->insertTableInstance("test", value);

	(*value)[0].INT = 99586;
	(*value)[1].FLOAT = 1.96;
	memcpy((*value)[2].CHAR, "另一种字符串", 50);
	r->insertTableInstance("test", value);

	cout << "test records have been added" << endl;
	cout << "press enter to read the records from the database" << endl;
	system("pause");

	value = r->getRecordById("test",1);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;
	value = r->getRecordById("test", 2);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;
	value = r->getRecordById("test", 0);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;

	cout << "press enter to test deletion " << endl;
	system("pause");
	vector<int> *removeList = new vector<int>;
	removeList->push_back(0);
	removeList->push_back(1);
	removeList->push_back(2);
	r->deleteTableInstance("test", removeList);

	cout << "The 3 records are deleted from the table." << endl;
	cout << "Now, if the deletion is normal, insert the 3 records again will suceed." << endl;
	cout << "Press enter to continue" << endl;
	system("pause");

	temp.INT = 10086;
	value->push_back(temp);
	temp.FLOAT = 585.65;
	value->push_back(temp);
	temp.CHAR = new char[50];
	memcpy(temp.CHAR, "你们好啊!", 50);
	value->push_back(temp);

	
	r->insertTableInstance("test", value);


	(*value)[0].INT = 996;
	(*value)[1].FLOAT = 19.96;
	memcpy((*value)[2].CHAR, "另一种字符CHUAN", 50);


	r->insertTableInstance("test", value);


	(*value)[0].INT = 99586;
	(*value)[1].FLOAT = 1.96;
	memcpy((*value)[2].CHAR, "另一种字符串", 50);


	r->insertTableInstance("test", value);


	cout << "test records have been added" << endl;
	cout << "press enter to read the records from the database" << endl;
	system("pause");

	value = r->getRecordById("test", 1);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;
	value = r->getRecordById("test", 2);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;
	value = r->getRecordById("test", 0);
	cout << (*value)[0].INT << " " << (*value)[1].FLOAT << " " << (*value)[2].CHAR << endl;

	cout << "NO BUG FOUND!!!!!!!!" << endl;

	
	system("pause");
	return 0;
}