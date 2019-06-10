#include<recordManager/recordManager.h>
#include<bufferManager/binaryFile.h>
#include<table/tableFile.h>

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
		char * data = new char[blockSize];
		binaryFile::writeInt(data, 0);  //header point
		binaryFile::writeInt(data + 4, 1);// blockNumber
		binaryFile::writeInt(data + 8, 0); // quantity of records.
		binaryFile::writeInt(data + 12, -1); // max ID ever occured.
		fwrite(data, blockSize, 1, file);
		fclose(file);
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
{
	tableFile * file = new tableFile(tableName);
	file->insertRecord(value);
	delete file;
	return true;
}
bool recordManager::deleteTableInstance(string tableName, vector<int>* list)                                //�Ȳ�ѯ����ɾ����  ���ԣ� ��һ������ά���滻�� Ҫ��list�����С��������
{
	tableFile * file = new tableFile(tableName);
	int len = list->size();
	if (len == 0)
	{
		delete file;
		return true;
	}
	else if (len == 1)
	{
		file->deleteRecord((*list)[0]);
		return true;
	}
	len--;
	int cId, nId;
	cId = (*list)[0];
	for (int i = 0; i < len; i++)
	{
		nId = (*list)[i + 1];
		if (file->getBlockId(cId) != file->getBlockId(nId))
			file->deleteRecord(cId, true);
		else
			file->deleteRecord(cId, false);
		cId = nId;
	}
	file->deleteRecord((*list)[len], true);
	delete file;
	return true;
}
vector<int>* recordManager::select(string tableName, vector<Logic>*)                              //һ��condition����ѯ������һ����С����������кű� ��������������
{
	return NULL;
}

bool recordManager::defragmentation(string tableName)  //��Ƭ�����������д
{
	return false;
}