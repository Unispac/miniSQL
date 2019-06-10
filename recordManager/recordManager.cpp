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

bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// ÿ��ֻ��Ҫ���һ�������ֵ���г��������ˡ�
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

	//catalog->updateTable(table);  //��catalog�ĸ��»�������API���..
	return true;
}
bool recordManager::deleteTableInstance(string tableName, vector<int>*)                                //�Ȳ�ѯ����ɾ����  ���ԣ� ��һ������ά���滻�� ֱ��ɾ��..
{
	Table* table = catalog->getTable(tableName);

}
vector<int>* recordManager::select(string tableName, vector<Logic>*)                              //һ��condition����ѯ������һ����С����������кű� ��������������
{

}