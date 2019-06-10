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
	if (fopen(filePath.c_str(), "rb") == NULL)  //创建表文件及其表头文本
	{
		FILE * file = fopen(filePath.c_str(), "ab+");
		char * data = new char[tableFileHeaderSize];
		binaryFile::writeInt(data, 0);  //header point
		binaryFile::writeInt(data + 4, 1);// new block?
		fwrite(data, tableFileHeaderSize, 1, file);
	}
	return true;
}

bool recordManager::dropTable(string tableName) // recorder只负责管理表内容.表头信息由catalog管，索引信息由index管。
{
	string filePath = "data/" + tableName + ".mdb";
	remove(filePath.c_str());
	return true;
}

bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// 每次只需要添加一条表项，将值表列出来就行了。
{ //在指针处添加表项，待修改
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
bool recordManager::deleteTableInstance(string tableName, vector<int>*)                                //先查询再做删除？  策略？ 用一个链表维护替换？ 
{
	Table* table = catalog->getTable(tableName);

}
vector<int>* recordManager::select(string tableName, vector<Logic>*)                              //一个condition表，查询，返回一个从小到大排序的行号表。 依赖索引。。。
{

}