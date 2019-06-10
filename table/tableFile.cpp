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
bool tableFile::insertRecord(vector<tableValue>* value)
{
	int blockId = (emptyId / recordNumPerBlock) + 1;
	int offset = emptyId % recordNumPerBlock;

	maxId = max(maxId, emptyId);
	Block *block;
	if (blockId < blockCnt)block = buffer->getBlock(tableName, blockId);
	else
	{
		block = buffer->appendBlock(tableName);
		blockCnt++;
	}

	recordCnt++;
	offset = offset * sizePerRecord;
	char *p = block->data + offset;

	if (recordCnt-1==maxId)emptyId++;
	else binaryFile::readInt(p, &emptyId);

	binaryFile::writeTableValue(p, value, table);
	buffer->writeBlock(block);

	updateHeader();
	return true;
}
bool tableFile::deleteRecord(int id,bool commit=true)
{
	int blockId = (emptyId / recordNumPerBlock) + 1;
	int offset = emptyId % recordNumPerBlock;

	if (id > maxId)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

	Block * block = buffer->getBlock(tableName, blockId);
	offset *= sizePerRecord;
	char *p = block->data + offset;
	char mark;
	memcpy(&mark, p, 1);
	if (mark == 0)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

	mark = 0;
	binaryFile::writeChar(p, &mark, 1);
	p++;
	binaryFile::writeInt(p, emptyId);
	emptyId = id;
	recordCnt--;

	if (commit == true)
	{
		buffer->writeBlock(block);
		updateHeader();
	}
	else
	{
		block->pin = true; //如果不立即写回，那需要锁住，防止cache丢失。
	}
	return true;
}
vector<tableValue> * tableFile::getRecord(int id)
{
	int blockId = (emptyId / recordNumPerBlock) + 1;
	int offset = emptyId % recordNumPerBlock;

	if (id > maxId)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

	Block * block = buffer->getBlock(tableName, blockId);
	offset *= sizePerRecord;
	char *p = block->data + offset;
	char mark;
	memcpy(&mark, p, 1);
	if (mark == 0)
		errorHandler->reportErrorCode(RECORD_DOES_NOT_EXIST);

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
}
int tableFile::getBlockId(int recordId)
{
	return recordId / recordNumPerBlock + 1;
}