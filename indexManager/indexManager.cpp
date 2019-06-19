#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

#include <table/Table.h>
#include <catalogManager/catalogManager.h>
#include <index/BPTree.h>
#include <indexManager/indexManager.h>
#include <config.h>
#include <utils/errorReporter.h>
#include <index/Index.h>

extern errorReporter* errorHandler;


indexManager::indexManager()
{
	buffer = new bufferManager();
}

void deleteFile(string filePath)
{
	remove(filePath.c_str());
	return;
}

int indexManager::find(const char* indexName, const char* key)
{
	BPTree* tree = new BPTree(("index/" + string(indexName)).c_str());
	int ret = tree->find(key);
	delete tree;
	return ret;
}

bool indexManager::insert(const char* indexName, const char* key, int value)
{
	BPTree* tree = new BPTree(("index/" + string(indexName)).c_str());
	bool ret = tree->add(key, value);
	if (!ret) errorHandler->reportErrorCode(BPTREE_DUPLICATE_KEY);
	delete tree;
	return ret;
}

bool indexManager::remove(const char* indexName, const char* key)
{
	BPTree* tree = new BPTree(("index/" + string(indexName)).c_str());
	bool ret = tree->remove(key);
	if (!ret) errorHandler->reportErrorCode(BPTREE_KEY_NOT_FOUND);
	delete tree;
	return ret;
}

bool indexManager::createIndex(const char* indexName)
{
	catalogManager* manager = new catalogManager();
	Index* index = manager->getIndexByName(string(indexName));
	if (index == NULL)
		return false;
	Table* table = manager->getTable(string(index->getTableName()));
	if (table == NULL)
		return false;
	dbDataType* temp = table->findAttrByName(index->getColName());
	int keyLength = temp->getKeyLength();
	int dataType = temp->dbType;
	BPTree::createFile(("index/" + string(indexName)).c_str(), keyLength, dataType);
	return true;
}

bool indexManager::dropIndex(const char* indexName)
{
	string filePath = string("data/") + string("index/") + string(indexName) + string(".mdb");	
	deleteFile(filePath);
	buffer->clearBuffer();
	return true;
}
