#include<catlogManager/catalogManager.h>
#include<iostream>
#include<fstream>

extern errorReporter * errorHandler;

catalogManager::catalogManager()
{
	string filePath = "catalog/tableNameList.mdb";
	ifstream infile;
	infile.open(filePath, ios::in);
	
	if (infile.fail())errorHandler->reportErrorCode(NO_TABLE_LIST);
	
	string temp;
	while (infile>>temp)
	{
		tableNameList.insert(temp);
	}
	infile.close();
}

catalogManager::~catalogManager()
{

}

Table * catalogManager::getTable(string TableName)
{
	string filePath = "catalog/table_" + TableName + ".mdb";
	ifstream infile;
	
	if(tableNameList.count(TableName)==0)
		errorHandler->reportErrorCode(NO_TABLE);

	infile.open(filePath, ios::in);
	if (infile.fail())errorHandler->reportErrorCode(NO_TABLE);
	

	vector<dbDataType*> * attr = new vector<dbDataType*>;
	vector<string> * index = new vector<string>;
	int dbType, n, unique, primary, hasIndex;
	string name;
	while ( infile>>dbType)
	{
		infile >> n >> name >> unique >> primary >> hasIndex;
		attr->push_back(new dbDataType(dbType, n, name, unique, primary,hasIndex));
		if (hasIndex)index->push_back(name);
	}
	infile.close();
	return new Table(TableName, attr, index);
}

bool catalogManager::createTable(string TableName,vector<dbDataType*>*attr)
{
	dbDataType * temp;

	if (tableNameList.count(TableName) != 0)
		errorHandler->reportErrorCode(TABLE_ALREADY_EXIST);
	
	int len = attr->size();
	if (len > maxTableAttr)errorHandler->reportErrorCode(TOO_MANY_ATTR);

	int cntOfPrimary=0;
	set<string>attrName;

	for (int i = 0; i < len; i++)
	{
		temp = (*attr)[i];
		if (temp->primary)cntOfPrimary++;
		if (temp->dbType==DB_CHAR && temp->n > maxCharLen)errorHandler->reportErrorCode(TOO_LARGE_CHAR);
		if (attrName.count(temp->name) != 0)errorHandler->reportErrorCode(ATTR_NAME_REDUNDANCY);
		if ((temp->unique == false&&temp->primary == false) && temp->hasIndex == true)errorHandler->reportErrorCode(CAN_NOT_SET_INDEX);

		attrName.insert(temp->name);

		if (temp->dbType != DB_CHAR && temp->dbType != DB_FLOAT && temp->dbType != DB_INT)
			errorHandler->reportErrorCode(ILLEGAL_DATA_TYPE);
	}

	if (cntOfPrimary > 1)
		errorHandler->reportErrorCode(TOO_MANY_PRIMARY);
	else if (cntOfPrimary < 1)
		errorHandler->reportErrorCode(NO_PRIMARYKEY);

	string filePath = "catalog/table_" + TableName + ".mdb";
	ofstream outfile;
	outfile.open(filePath, ios::out);
	//infile >> dbType >> n >> name >> unique >> primary>>hasIndex;
	for (int i = 0; i < len; i++)
	{
		temp = (*attr)[i];
		outfile << temp->dbType << " " << temp->n << " " << temp->name << " " << temp->unique << " " << temp->primary << " " << temp->hasIndex << endl;
	}
	outfile.close();
	tableNameList.insert(TableName);
	outfile.open("catalog/tableNameList.mdb", ios::app);
	outfile << TableName << endl;
	return true;
}
bool catalogManager::dropTable(string TableName)
{
	if (tableNameList.count(TableName) == 0)
		errorHandler->reportErrorCode(NO_TABLE_TO_DROP);

	tableNameList.erase(TableName);
	string filePath = "catalog/table_" + TableName + ".mdb";

	remove(filePath.c_str());

	set<string>::iterator iter = tableNameList.begin();
	ofstream outfile;
	outfile.open("catalog/tableNameList.mdb",ios::out);
	while (iter != tableNameList.end())
	{
		outfile << *iter << endl;
		iter++;
	}
	outfile.close();

	return true;
}

/*bool catalogManager::updateTable(Table * table)
{
	string tableName = table->name;
	string filePath = "catalog/table_" + tableName+ ".mdb";
	ofstream outfile;
	outfile.open(filePath, ios::out);
	int len = table->colCnt;
	dbDataType * temp;
	for (int i = 0; i < len; i++)
	{
		temp = (*(table->attrList))[i];
		outfile << temp->dbType << " " << temp->n << " " << temp->name << " " << temp->unique << " " << temp->primary << " " << temp->hasIndex << endl;
	}
	outfile << table->rowCnt << endl; //rowCnt=0
	outfile.close();
	return true;
}*/

Index * catalogManager::getIndex(string TableName, string attrName)
{
	return NULL;
}
bool catalogManager::createIndex(string TableName, string attrName)
{
	return true;
}
bool catalogManager::dropIndex(string TableName, string attrName)
{
	return true;
}
