#ifndef indexManager_H
#define indexManager_H
#include<config.h>
#include<index/Index.h>
#include <fstream>

void deleteFile(string filePath)
{
	remove(filePath.c_str());
	return;
}

class indexManager
{
public:
	int find(const char* indexName, const char* key);
	bool insert(const char* indexName, const char* key, int value);
	bool remove(const char* indexName, const char* key);
	bool createIndex(const char* indexName);
	bool dropIndex(const char* indexName);
};

#endif // !indexManager_H
