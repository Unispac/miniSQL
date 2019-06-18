#include<systemAPI/systemAPI.h>
#include<utils/stringProcesser.h>
#include <catalogManager/catalogManager.h>
#include<sstream>
#include <bufferManager/binaryFile.h>
#include <indexManager/indexManager.h>
#include <table/tableFile.h>
#include <recordManager/recordManager.h>
#include <table/Table.h>

extern errorReporter * errorHandler;

systemAPI::systemAPI()
{
	buffer = new bufferManager();
	recorder = new recordManager();
	catalog = new catalogManager();
	indexer = new indexManager();
}
systemAPI::~systemAPI()
{
	delete buffer;
	delete recorder;
	delete catalog;
	delete indexer;
}

bool systemAPI::createTable(string tableName, vector<dbDataType*>*attr)
{
	if(!catalog->createTable(tableName, attr))return false;
	for (auto col : *attr)
	{
		if (col->primary)
		{
			createIndex(string("primary_index_of_") + tableName, tableName, col->name);
			break;
		}
	}
	if(!recorder->createTable(tableName))return false;
	return true;
}

bool systemAPI::dropTable(string tableName)
{
	Table* table = catalog->getTable(tableName);
	for(auto colName: *(table->attributesHaveIndex))
	{
		Index* index = catalog->getIndexByTableCol(tableName, colName);
		dropIndex(index->getName());
		delete index;
	}
	if(!catalog->dropTable(tableName))return false;
	if(!recorder->dropTable(tableName))return false;
	return true;
}

bool systemAPI::createIndex(string indexName, string tableName, string attrName)
{
	if (!catalog->createIndex(indexName, tableName, attrName)) return false;
	if (!indexer->createIndex(indexName.c_str())) return false;
	return true;
}

bool systemAPI::dropIndex(string indexName)
{
	if (!catalog->dropIndex(indexName)) return false;
	if (!indexer->dropIndex(indexName.c_str())) return false;
	return true;
}

int systemAPI::find(string tableName, vector<Logic>* conditions, vector<vector<tableValue>*>* rst, vector<int>* ids)
{
	Table* table = catalog->getTable(tableName);
	for(auto logic: *conditions)
	{
		if (logic.opcode != EQUAL) continue;
		Index* index = catalog->getIndexByTableCol(tableName, logic.valName);
		if (index == NULL) continue;
		dbDataType* attr = table->findAttrByName(logic.valName);
		char* key = new char[attr->getKeyLength()];
		if (attr->dbType == DB_INT)
			binaryFile::writeInt(key, logic.immediate.INT);
		else if (attr->dbType == DB_FLOAT) 
			binaryFile::writeFloat(key, logic.immediate.FLOAT);
		else
			binaryFile::writeChar(key, logic.immediate.CHAR, attr->n);
		if (key == NULL) return 0;
		int id = indexer->find(index->getName(), key);

		int ret;
		if (id < 0) ret = 0;
		else
		{
			vector<tableValue>* record = recorder->getRecordById(tableName, id);
			if (recorder->checkRecord(table, record, conditions))
			{
				rst->push_back(record);
				ids->push_back(id);
				ret = 1;
			}
			else
				ret = 0;
		}

		return ret;
	}

	vector<int>* _ = new vector<int>;
	_ = recorder->select(tableName, conditions);
	for(auto id: *_)
	{
		ids->push_back(id);
		rst->push_back(recorder->getRecordById(tableName, id));
	}

	return ids->size();
}

vector<vector<tableValue>*> * systemAPI::select(string tableName, vector<Logic>* conditions)
{
	// check tablename and conditions_op and conditions_attr
	Table* table = catalog->getTable(tableName);
	if (table == NULL)
	{
		errorHandler->reportErrorCode(NO_TABLE);
		return NULL;
	}

	for(auto logic: *conditions)
	{
		if (logic.opcode < EQUAL || logic.opcode > GREATER_EQUAL)
		{
			errorHandler->reportErrorCode(OPCODE_NOT_FOUND);
			return NULL;
		}
		if (table->findAttrByName(logic.valName) == NULL)
		{
			errorHandler->reportErrorCode(ATTR_NOT_FOUND);
			return NULL;
		}
	}

	vector<vector<tableValue>*>* rst = new vector<vector<tableValue>*>;
	vector<int>* ids = new vector<int>;
	int selectCount = find(tableName, conditions, rst, ids);
	return rst;
}

void writeKey(dbDataType* attr, char* key, tableValue v)
{
	if (attr->dbType == DB_INT)
		binaryFile::writeInt(key, v.INT);
	else if (attr->dbType == DB_FLOAT)
		binaryFile::writeFloat(key, v.FLOAT);
	else
		binaryFile::writeChar(key, v.CHAR, attr->getKeyLength());
}

bool systemAPI::insert(string tableName, vector<string> vList)  //Ŀǰ��û����Լ�����
{
	Table * table = catalog->getTable(tableName);
	
	if (table == NULL)return false;
	vector<dbDataType*> * attrList = table->attrList;
	vector<bool>* isNULL = new vector<bool>;
	for (int i = 0; i < vList.size(); i++)
		isNULL->push_back(vList[i] == "" ? true : false);
	bool allHaveIndex = true;
	for (int i = 0; i < table->attrList->size(); i++)
	{
		dbDataType* attr = (*(table->attrList))[i];
		if (!attr->primary && !attr->unique) continue;
		if ((*isNULL)[i])
		{
			errorHandler->reportErrorCode(ATTR_NULL);
			return false;
		}
		Index* index = catalog->getIndexByTableCol(tableName, attr->name);
		if (index == NULL)
			allHaveIndex = false;
	}

	vector<tableValue> * value = new vector<tableValue>;
	tableValue x;

	int size = attrList->size();
	if (size != vList.size())
	{
		errorHandler->reportErrorCode(VALUE_TABLE_NOT_MATCH);
		return false;
	}

	//vector<tableValue>* value
	for (int i = 0; i < size; i++)
	{
		x.CHAR = NULL;
		if ((*attrList)[i]->dbType == DB_INT)
		{
			if (!stringProcesser::isInt(vList[i]))
			{
				errorHandler->reportErrorCode(VALUE_TABLE_NOT_MATCH);
				return false;
			}
			else
			{
				stringstream tran;
				tran << vList[i];
				tran >> x.INT;
				value->push_back(x);
			}
		}
		else if ((*attrList)[i]->dbType == DB_FLOAT)
		{
			if (!stringProcesser::isFloat(vList[i]))
			{
				errorHandler->reportErrorCode(VALUE_TABLE_NOT_MATCH);
				return false;
			}
			else
			{
				stringstream tran;
				tran << vList[i];
				tran >> x.FLOAT;
				value->push_back(x);
			}
		}
		else
		{
			if (!stringProcesser::isChar(vList[i]))
			{
				errorHandler->reportErrorCode(VALUE_TABLE_NOT_MATCH);
				return false;
			}
			else
			{
				stringProcesser::getRidQuo(vList[i]);
				int maxLen = (*attrList)[i]->n;
				if (vList[i].size() > maxLen)
				{
					errorHandler->reportErrorCode(CHAR_LEN_EXCEED);
				}
				x.CHAR = new char[maxLen];
				memcpy(x.CHAR, vList[i].c_str(), vList[i].size()+1);
				value->push_back(x);
			}
		}
	}

	bool noDuplicate = true;
	if (allHaveIndex)
	{
		for (int i = 0; i < table->attrList->size(); i++)
		{
			dbDataType* attr = table->attrList->at(i);
			if (!attr->primary && !attr->unique) continue;
			Index* index = catalog->getIndexByTableCol(tableName, attr->name);
			char* key = new char[attr->getKeyLength()];
			writeKey(attr, key, value->at(i));
			int pos = indexer->find(index->getName(), key);
			if (pos >= 0)
			{
				noDuplicate = false;
				break;
			}
		}
	}
	else
		noDuplicate = recorder->checkDuplicate(tableName, value);

	if (!noDuplicate)
	{
		errorHandler->reportErrorCode(ATTR_DUPLICATE);
		return false;
	}

	int id = recorder->insertTableInstance(tableName, value);
	for (int i = 0; i < table->attrList->size(); i++)
	{
		dbDataType* attr = table->attrList->at(i);
		if (!attr->primary && !attr->unique) continue;
		Index* index = catalog->getIndexByTableCol(tableName, attr->name);
		if (index != NULL)
		{
			char* key = new char[attr->getKeyLength()];
			writeKey(attr, key, value->at(i));
			indexer->insert(index->getName(), key, id);
		}
	}

	char *temp;
	for (int i = 0; i < size; i++)
	{
		//int t = (*attrList)[i]->dbType;
		//cout << "type : " << t << "     ";
		//if (t == DB_INT)cout << (*value)[i].INT << endl;
		//else if(t==DB_FLOAT)cout << (*value)[i].FLOAT << endl;
		//else cout << (*value)[i].CHAR << endl;

		temp = (*value)[i].CHAR;
		if (temp != NULL)delete temp;
	}
	delete value;
	delete table;
	return id >= 0;
}

int systemAPI::remove(string tableName, vector<Logic>* conditions)
{
	Table* table = getTable(tableName);
	if (table == NULL)
	{
		errorHandler->reportErrorCode(NO_TABLE);
		return 0;
	}
	vector<vector<tableValue>*>* rst = new vector<vector<tableValue>*>;
	vector<int>* ids = new vector<int>;
	int selectNum = find(tableName, conditions, rst, ids);

	// update index
	for(auto indexCol: *(table->attributesHaveIndex))
	{
		int pos = table->findPosByName(indexCol);
		dbDataType* attr = (*(table->attrList))[pos];
		char* key = new char[attr->getKeyLength()];
		Index* index = catalog->getIndexByTableCol(table->name, indexCol);
		for(auto record: *rst)
		{
			writeKey(attr, key, record->at(pos));
			indexer->remove(index->getName(), key);
		}
	}

	// clear record
	recorder->deleteTableInstance(tableName, ids);
	return selectNum;
}

Table * systemAPI::getTable(string tableName)
{
	return catalog->getTable(tableName);
}