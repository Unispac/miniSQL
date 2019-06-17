#include<config.h>
#include <bufferManager/binaryFile.h>
#include<bufferManager/bufferManager.h>
#include<catalogManager/catalogManager.h>
#include<recordManager/recordManager.h>
#include <indexManager/indexManager.h>
#include<fstream>
#include<table/Table.h>
#include<interpreter\Interpreter.h>
#include<systemAPI\systemAPI.h>
using namespace std;

errorReporter * errorHandler;
systemAPI * api;
void configInit()
{
	CreateDirectory("data", NULL);
	CreateDirectory("catalog", NULL);
	CreateDirectory("data/index", NULL);
	if (fopen("catalog/tableNameList.mdb", "rb") == NULL)
		fopen("catalog/tableNameList.mdb", "ab+");
	if (fopen("catalog/indexNameList.mdb", "rb") == NULL)
		fopen("catalog/indexNameList.mdb", "ab+");
}
int main()
{
	//Interpreter::getType(Interpreter::CMD());
	//system("pause");
	//return 0;
	configInit();
	catalogManager * p = new catalogManager();
	recordManager *r = new recordManager();
	indexManager* imanager = new indexManager();

	errorHandler = new errorReporter();
	api = new systemAPI();
	cout << Interpreter::getType(Interpreter::CMD()) << endl;
	system("pause");
	return 0;
}
/*
int main()
{
	configInit();
	cout << Interpreter::getType(Interpreter::CMD()) << endl;
	system("pause");
	return 0;

	bool x = true;
	vector<dbDataType*> *test = new vector<dbDataType*>;
	test->push_back(new dbDataType(DB_CHAR, 1000, "strid", true, true, false));
	test->push_back(new dbDataType(DB_CHAR, 1000, "strname", true, false, false));
	p->createTable("t1", test);
	r->createTable("t1");
	p->createIndex("strid_t1", "t1", "strid");
	imanager->createIndex("strid_t1");
	p->createIndex("strname_t1", "t1", "strname");
	imanager->createIndex("strname_t1");

	vector<tableValue> *v = new vector<tableValue>;
	tableValue temp;
	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "1", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "wyh", 1000);
	v->push_back(temp);
	int pos1 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "1", pos1);
	imanager->insert("strname_t1", "wyh", pos1);
	cout << imanager->find("strid_t1", "1") << endl;
	cout << imanager->find("strname_t1", "wyh") << endl;
	v->clear();
	system("pause");

	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "2", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "qxy", 1000);
	v->push_back(temp);
	int pos2 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "2", pos2);
	imanager->insert("strname_t1", "qxy", pos2);
	cout << imanager->find("strid_t1", "2") << endl;
	cout << imanager->find("strname_t1", "qxy") << endl;
	v->clear();
	system("pause");

	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "3", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "lwc", 1000);
	v->push_back(temp);
	int pos3 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "3", pos3);
	imanager->insert("strname_t1", "lwc", pos3);
	cout << imanager->find("strid_t1", "3") << endl;
	cout << imanager->find("strname_t1", "lwc") << endl;
	v->clear();
	system("pause");

	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "4", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "jsb", 1000);
	v->push_back(temp);
	int pos4 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "4", pos4);
	imanager->insert("strname_t1", "jsb", pos4);
	cout << imanager->find("strid_t1", "4") << endl;
	cout << imanager->find("strname_t1", "jsb") << endl;
	v->clear();
	system("pause");

	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "5", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "lmr", 1000);
	v->push_back(temp);
	int pos5 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "5", pos5);
	imanager->insert("strname_t1", "lmr", pos5);
	cout << imanager->find("strid_t1", "5") << endl;
	cout << imanager->find("strname_t1", "lmr") << endl;
	v->clear();
	system("pause");

	temp.CHAR = new char[1000];
	memcpy(temp.CHAR, "6", 1000);
	v->push_back(temp);
	memcpy(temp.CHAR, "lyy", 1000);
	v->push_back(temp);
	int pos6 = r->insertTableInstance("t1", v);
	imanager->insert("strid_t1", "6", pos6);
	imanager->insert("strname_t1", "lyy", pos6);
	cout << imanager->find("strid_t1", "6") << endl;
	cout << imanager->find("strname_t1", "lyy") << endl;
	v->clear();
	system("pause");

	imanager->remove("strid_t1", "5");
	cout << imanager->find("strid_t1", "4") << endl;
	cout << imanager->find("strid_t1", "5") << endl;
	system("pause");
	
	//	dbDataType(int inType, int inN, string inName, 
	//	bool inUnique = false, bool inPrimary = false, bool inIndex = false)
	
	test->clear();
	test->push_back(new dbDataType(  DB_INT,1,"id", true, true, false));
	test->push_back(new dbDataType(  DB_FLOAT, 1,"float",true, false, false));
	test->push_back(new dbDataType(  DB_CHAR,50,"float_2", true, false, false));

	p->createTable("test", test);
	r->createTable("test");
	cout << " Now you can check whether the table is created !" << endl;
	cout << " The process will pause. Press enter to continue." << endl;
	system("pause");

	p->createIndex("id_of_test", "test", "id");
	imanager->createIndex("id_of_test");

	//bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// 每次只需要添加一条表项，将值表列出来就行了。
	vector<tableValue> * value = new vector<tableValue>;
	temp.INT = 10086;
	value->push_back(temp);
	temp.FLOAT = 585.65;
	value->push_back(temp);
	temp.CHAR = new char[50];
	memcpy(temp.CHAR, "你们好啊!", 50);
	value->push_back(temp);
	
	int pos; char chr_id[blockSize];
	pos = r->insertTableInstance("test", value);

	(*value)[0].INT = 996;
	(*value)[1].FLOAT = 19.96;
	memcpy((*value)[2].CHAR, "另一种字符CHUAN", 50);
	pos = r->insertTableInstance("test", value);
	
	(*value)[0].INT = 99586;
	(*value)[1].FLOAT = 1.96;
	memcpy((*value)[2].CHAR, "另一种字符串", 50);
	r->insertTableInstance("test", value);

	cout << "test records have been added" << endl;
	cout << "press enter to add index on attribute " << endl;

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
	system("pause");

//	imanager->insert("id_of_test", )

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
*/