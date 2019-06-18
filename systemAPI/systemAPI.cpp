#include<systemAPI/systemAPI.h>
#include<utils/stringProcesser.h>
#include <catalogManager/catalogManager.h>
#include<sstream>
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
	if(!recorder->createTable(tableName))return false;
	return true;
}

bool systemAPI::dropTable(string tableName)
{
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
		
	}
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

	//
	vector<int> * selectId = recorder->select(tableName, conditions);
	vector<vector<tableValue>*>* result = new vector<vector<tableValue>*>;
	int i, id, len = selectId->size();
	for (i = 0; i < len; i++)
	{
		id = (*selectId)[i];
		result->push_back(recorder->getRecordById(tableName,id));
	}
	return result;
}

bool systemAPI::insert(string tableName, vector<string> vList)  //Ŀǰ��û����Լ�����
{
	Table * table = catalog->getTable(tableName);
	
	if (table == NULL)return false;
	vector<dbDataType*> * attrList = table->attrList;
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
	bool result =recorder->insertTableInstance(tableName, value);
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
	return result;
}
bool systemAPI::remove(string tableName, vector<Logic>* conditions)
{
	vector<int> * selectId = recorder->select(tableName, conditions);
	recorder->deleteTableInstance(tableName,selectId);
	return true;
}