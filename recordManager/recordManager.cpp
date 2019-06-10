#include<recordManager/recordManager.h>
#include<bufferManager/binaryFile.h>
recordManager::recordManager()
{
	catalog = new catalogManager();
	buffer = new bufferManager();
}

recordManager::~recordManager()
{
	delete catalog;
	delete buffer;
}

bool recordManager::createTable(string tableName)
{
	string filePath = "data/" + tableName + ".mdb";
	if (fopen(filePath.c_str(), "rb") == NULL)
		fopen(filePath.c_str(), "ab+");
	return true;
}

bool recordManager::dropTable(string tableName)
{
	string filePath = "data/" + tableName + ".mdb";
	remove(filePath.c_str());
	return true;
}

bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// 每次只需要添加一条表项，将值表列出来就行了。
{
	Table* table = catalog->getTable(tableName);
	int cnt = table->rowCnt;
	int recordPerBlock = table->instanceNumPerBlock;
	int sizePerRecord = table->sizePerInstance;
	table->rowCnt = ++cnt;
	int blockId = (cnt - 1) / recordPerBlock;
	int offset = (cnt - 1) % recordPerBlock;
	
	Block *block = buffer->getBlock(tableName, blockId);
	char * data = new char[blockSize];
	memcpy(data, block->data, blockSize);
	offset = offset * sizePerRecord;
	char *p = data + offset;
	binaryFile::writeTableValue(p, value, table);
	buffer->writeBlock(block, data);

	//catalog->updateTable(table);  //对catalog的更新还是留给API层干..
	return true;
}
bool recordManager::deleteTableInstance(string tableName, vector<int>*)                                //先查询再做删除？  策略？ 用一个链表维护替换？ 直接删？..
{
	Table* table = catalog->getTable(tableName);

}
vector<int>* recordManager::select(string tableName, vector<Logic>*)                              //一个condition表，查询，返回一个从小到大排序的行号表。 依赖索引。。。
{

}