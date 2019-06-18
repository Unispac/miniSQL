#include<table/tableFile.h>

extern errorReporter * errorHandler;

tableFile::tableFile(string name)
{
	tableName = name;
	catalog = new catalogManager();
	buffer = new bufferManager();

	table = catalog->getTable(name);
	sizePerRecord = table->sizePerInstance;
	recordNumPerBlock = table->instanceNumPerBlock;

	//cout << "aha" << endl;

	Block * header = buffer->getBlock(tableName, 0);
	binaryFile::readInt(header->data, &emptyId);
	binaryFile::readInt(header->data + 4, &blockCnt);
	binaryFile::readInt(header->data + 8, &recordCnt);
	binaryFile::readInt(header->data + 12, &maxId);
}
tableFile::~tableFile()
{
	delete catalog;
	delete buffer;
}
int tableFile::insertRecord(vector<tableValue>* value)
{
	//cout << "INSER : ID = " << emptyId << endl;
	int blockId = (emptyId / recordNumPerBlock) + 1;
	int offset = emptyId % recordNumPerBlock;
	int ret = emptyId;

	maxId = max(maxId, emptyId);
	Block *block;
	if (blockId < blockCnt)block = buffer->getBlock(tableName, blockId);
	else
	{
		buffer->appendBlock(tableName);
		blockCnt++;
		block=buffer->getBlock(tableName, blockCnt-1);
	}
	recordCnt++;
	offset = offset * sizePerRecord;
	char *p = block->data + offset;

	if (recordCnt-1==maxId)emptyId++;
	else
	{
		//cout << "MARK : " << int(*p) << endl;
		binaryFile::readInt(p+1, &emptyId);
		//cout << "READ POINTER : " << emptyId << endl;
	}
	binaryFile::writeTableValue(p, value, table);
	buffer->writeBlock(block);

	updateHeader();

	
	return ret;
}
bool tableFile::deleteRecord(int id,bool commit)
{
	int blockId = (id / recordNumPerBlock) + 1;
	int offset = id % recordNumPerBlock;

	if (id > maxId)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

	Block * block = buffer->getBlock(tableName, blockId);
	offset *= sizePerRecord;
	char *p = block->data + offset;

	char mark = *p;
	if (mark == 0)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);
	
	*p = 0;
	p++;
	binaryFile::writeInt(p, emptyId);
	emptyId = id;
	
	int testInt;
	binaryFile::readInt(p, &testInt);
	//cout << "delete : " << id << " , last pointer is : " << testInt << ", current pointer is : " << emptyId << endl;

	recordCnt--;

	if (commit == true)
	{
		buffer->writeBlock(block);
		updateHeader();
	}
	else
	{
		block->pin = true; //���������д�أ�����Ҫ��ס����ֹcache��ʧ��
	}
	return true;
}

int tableFile::getMaxId()
{
	return maxId;
}

vector<tableValue> * tableFile::getRecord(int id, bool reportWhenNULL)
{
	int blockId = (id/ recordNumPerBlock) + 1;
	int offset = id% recordNumPerBlock;

	if (id > maxId)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

	Block * block = buffer->getBlock(tableName, blockId);
	offset *= sizePerRecord;

	

	char *p = block->data + offset;
	char mark;
	memcpy(&mark, p, 1);
	if (mark == 0)
	{
		if (reportWhenNULL)
			errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);
		return NULL;
	}
	vector<tableValue> * recordContent = new vector<tableValue>;
	binaryFile::readTableValue(p, recordContent,table);
	return recordContent;
}
bool tableFile::updateHeader()
{
	Block * header = buffer->getBlock(tableName, 0);
	binaryFile::writeInt(header->data, emptyId);
	binaryFile::writeInt(header->data + 4, blockCnt);
	binaryFile::writeInt(header->data + 8, recordCnt);
	binaryFile::writeInt(header->data + 12, maxId);
	buffer->writeBlock(header);
	return true;
}
int tableFile::getBlockId(int recordId)
{
	return recordId / recordNumPerBlock + 1;
}