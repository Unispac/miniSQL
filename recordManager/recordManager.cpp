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
	if (fopen(filePath.c_str(), "rb") == NULL)  //�������ļ������ͷ�ı�
	{
		FILE * file = fopen(filePath.c_str(), "ab+");
		char * data = new char[tableFileHeaderSize];
		binaryFile::writeInt(data, 0);  //header point
		binaryFile::writeInt(data + 4, 1);// new block?
		fwrite(data, tableFileHeaderSize, 1, file);
	}
	return true;
}

bool recordManager::dropTable(string tableName) // recorderֻ������������.��ͷ��Ϣ��catalog�ܣ�������Ϣ��index�ܡ�
{
	string filePath = "data/" + tableName + ".mdb";
	remove(filePath.c_str());
	return true;
}

bool recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// ÿ��ֻ��Ҫ���һ�������ֵ���г��������ˡ�
{ //��ָ�봦��ӱ�����޸�
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
bool recordManager::deleteTableInstance(string tableName, vector<int>*)                                //�Ȳ�ѯ����ɾ����  ���ԣ� ��һ������ά���滻�� 
{
	Table* table = catalog->getTable(tableName);

}
vector<int>* recordManager::select(string tableName, vector<Logic>*)                              //һ��condition����ѯ������һ����С����������кű� ��������������
{

}