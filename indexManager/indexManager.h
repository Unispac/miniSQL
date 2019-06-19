#ifndef indexManager_H
#define indexManager_H
#include<config.h>
#include<index/Index.h>
#include <fstream>
#include<bufferManager/bufferManager.h>

class indexManager
{
public:
	indexManager();
	int find(const char* indexName, const char* key);
	bool insert(const char* indexName, const char* key, int value);
	bool remove(const char* indexName, const char* key);
	bool createIndex(const char* indexName);
	bool dropIndex(const char* indexName);
private:
	bufferManager *buffer;
};

#endif // !indexManager_H
