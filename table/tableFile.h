#ifndef tableFile_H
#define tableFile_H

#include<config.h>
#include<bufferManager/binaryFile.h>
#include<bufferManager/bufferManager.h>
#include<catalogManager/catalogManager.h>
#include<table/Table.h>
#include<vector>

class tableFile
{
public:
	tableFile(string);
	~tableFile();
	bool insertRecord(vector<tableValue>* value);
	bool deleteRecord(int id,bool commit=true);
	vector<tableValue> * getRecord(int id);
	bool updateHeader();
	int getBlockId(int recordId);
private:
	int sizePerRecord;
	int recordNumPerBlock;
	int emptyId, blockCnt, recordCnt, maxId;
	string tableName;
	catalogManager * catalog;
	bufferManager * buffer;
	Table * table;

};

#endif // !tableFile_H
