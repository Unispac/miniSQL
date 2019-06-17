#include<catalogManager/catalogManager.h>
#include<iostream>
#include<fstream>

extern errorReporter * errorHandler;

set<string> catalogManager::tableNameList;
map<string, Index*> catalogManager::indexMap;

catalogManager::catalogManager()
{
	if (tableNameList.empty())
	{
		string filePath = "catalog/tableNameList.mdb";
		ifstream infile;
		infile.open(filePath, ios::in);
	
		if (infile.fail())errorHandler->reportErrorCode(NO_TABLE_LIST);
	
		string temp;
		while (infile >> temp)
		{
			tableNameList.insert(temp);
		}
		infile.close();
	}
	
	if (indexMap.empty())
	{
		string filePath = "catalog/indexNameList.mdb";
		ifstream infile;
		infile.open(filePath, ios::in);

		if (infile.fail()) errorHandler->reportErrorCode(NO_INDEX_LIST);

		string name, tableName, colName;
		while (infile >> name >> tableName >> colName)
		{
			char data[3 * maxTableAttr];
			memset(data, 0, sizeof(data));
			memcpy(data, name.c_str(), name.size());
			memcpy(data + maxTableAttr, tableName.c_str(), tableName.size());
			memcpy(data + 2 * maxTableAttr, colName.c_str(), colName.size());
			Index* index = new Index(data);
			indexMap[name] = index;
		}
	}
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

bool catalogManager::updateTable(Table * table)
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
	outfile.close();
	return true;
}

Index * catalogManager::getIndexByName(string indexName)
{
	if (indexMap.find(indexName) == indexMap.end())
	{
		errorHandler->reportErrorCode(NO_INDEX_NAME);
		return NULL;
	}
	return indexMap[indexName];
}

Index * catalogManager::getIndexByTableCol(string TableName, string colName)
{
	for (auto it : indexMap)
	{
		Index* index = it.second;
		if (
			strcmp(index->getTableName(), TableName.c_str()) == 0 &&
			strcmp(index->getColName(), colName.c_str()) == 0
			)
			return index;
	}
	return NULL;
}

void catalogManager::getIndexByTable(string TableName, vector<Index*>* vec)
{
	for (auto it : indexMap)
	{
		Index* index = it.second;
		if (strcmp(index->getTableName(), TableName.c_str()) == 0)
			vec->push_back(index);
	}
}

bool catalogManager::createIndex(string indexName, string TableName, string colName)
{
	if (indexMap.find(indexName) != indexMap.end())
	{
		errorHandler->reportErrorCode(INDEX_NAME_DUPLICATE);
		return false;
	}
	if (tableNameList.find(TableName) == tableNameList.end())
	{
		errorHandler->reportErrorCode(NO_TABLE);
		return false;
	}

	Table* table = getTable(TableName);
	dbDataType* attr = table->findAttrByName(colName);
	bool unique;
	if (attr != NULL)
	{
		if (!attr->unique)
		{
			errorHandler->reportErrorCode(NOT_UNIQUE);
			return false;
		}
	}
	else
	{
		errorHandler->reportErrorCode(ATTR_NOT_FOUND);
		return false;
	}

	Index* tcIndex = getIndexByTableCol(TableName, colName);
	if (tcIndex != NULL)
	{
		errorHandler->reportErrorCode(MULTIPLE_INDEX);
		return false;
	}

	attr->hasIndex = true;
	table->attributesHaveIndex->push_back(colName);
	updateTable(table);

	char indexData[maxTableAttr * 3];
	memset(indexData, 0, sizeof(indexData));
	memcpy(indexData, indexName.c_str(), indexName.size());
	memcpy(indexData + maxTableAttr, TableName.c_str(), TableName.size());
	memcpy(indexData + maxTableAttr * 2, colName.c_str(), colName.size());
	indexMap[indexName] = new Index(indexData);

	ofstream outFile;
	outFile.open("catalog/indexNameList.mdb", ios::app);
	outFile << indexName << ' ' << TableName << ' ' << colName << endl;
	outFile.close();
	return true;
}

bool catalogManager::dropIndex(string indexName)
{
	if (indexMap.find(indexName) == indexMap.end())
	{
		errorHandler->reportErrorCode(NO_INDEX_NAME);
		return false;
	}

	Table* table = getTable(string(indexMap[indexName]->getTableName()));
	dbDataType* attr = table->findAttrByName(indexMap[indexName]->getColName());
	attr->hasIndex = false;
	int pos;
	for (pos = 0; pos < table->attributesHaveIndex->size(); pos++)
	{
		if ((*table->attributesHaveIndex)[pos] == attr->name) break;
	}
	table->attributesHaveIndex->erase(table->attributesHaveIndex->begin() + pos);
	updateTable(table);

	delete indexMap[indexName];
	indexMap.erase(indexName);
	ofstream outFile;
	outFile.open("catalog/indexNameList.mdb", ios::out);
	for (auto it : indexMap)
	{
		Index* index = it.second;
		outFile << string(index->getName()) << ' ' << string(index->getTableName()) << ' '
			<< string(index->getColName()) << endl;
	}
	outFile.close();

	return true;
}

bool catalogManager::dropIndex(string TableName, string colName)
{
	Index* index = getIndexByTableCol(TableName, colName);
	if (index == NULL)
	{
		errorHandler->reportErrorCode(NO_INDEX_NAME);
		return false;
	}
	string indexName = index->getName();
	delete index;
	dropIndex(indexName);
	return true;
}
