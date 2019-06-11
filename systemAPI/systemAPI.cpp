#include<systemAPI\systemAPI.h>



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
	catalog->createTable(tableName, attr);
	recorder->createTable(tableName);
	return true;
}

bool systemAPI::dropTable(string tableName)
{
	catalog->dropTable(tableName);
	recorder->dropTable(tableName);
	return true;
}

bool systemAPI::createIndex(string indexName, string tableName, string attrName)
{
	return false;
}

bool systemAPI::dropIndex(string indexName)
{
	return false;
}

vector<vector<tableValue>*> * systemAPI::selet(string tableName, vector<Logic>* conditions)
{
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

bool systemAPI::insert(string tableName, vector<tableValue>* value)
{
	recorder->insertTableInstance(tableName, value);
	return true;
}
bool systemAPI::remove(string tableName, vector<Logic>* conditions)
{
	vector<int> * selectId = recorder->select(tableName, conditions);
	recorder->deleteTableInstance(tableName,selectId);
	return true;
}