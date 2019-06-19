#include <recordManager/recordManager.h>
#include <bufferManager/binaryFile.h>
#include <table/tableFile.h>
#include <utils/errorReporter.h>
#include <table/Table.h>
#include <config.h>

extern errorReporter * errorHandler;

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
		delete data;
	}
	return true;
}

bool recordManager::dropTable(string tableName) // recorderֻ�������������.��ͷ��Ϣ��catalog�ܣ�������Ϣ��index�ܡ�
{
	string filePath = "data/" + tableName + ".mdb";
	buffer->clearBuffer();
	/*if (fopen(filePath.c_str(), "rb") == NULL)
	{
		errorHandler->reportErrorCode(NO_TABLE_TO_DROP);
		return false;
	}*/
	remove(filePath.c_str());
	return true;
}

int recordManager::insertTableInstance(string tableName, vector<tableValue>* value)// ÿ��ֻ��Ҫ����һ�������ֵ���г��������ˡ�
{
	tableFile * file = new tableFile(tableName);
	int ret = file->insertRecord(value);
	delete file;
	return ret;
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

bool recordManager::checkRecord(Table* table, vector<tableValue>* record, vector<Logic>* conditions)
{	
	for(auto logic: *conditions)
	{
		int pos = table->findPosByName(logic.valName);
		dbDataType* attr = table->attrList->at(pos);
		if (attr->dbType == DB_INT)
		{
			int l = (*record)[pos].INT;
			int r = logic.immediate.INT;
			if (!logic.checkCondition(Logic::compareInt(l, r)))
				return false;
		}
		else if (attr->dbType == DB_FLOAT)
		{
			float l = (*record)[pos].FLOAT;
			float r = logic.immediate.FLOAT;
			if (!logic.checkCondition(Logic::compareFloat(l, r)))
				return false;
		}
		else 
		{
			int len = attr->n;
			char* l = (*record)[pos].CHAR;
			char* r = logic.immediate.CHAR;
			if (!logic.checkCondition(Logic::compareChar(l, r, len)))
				return false;
		}
	}
	return true;
}

// the validity of conditions is ensured in systemAPI
vector<int>* recordManager::select(string tableName, vector<Logic>* conditions)                              //һ��condition������ѯ������һ����С����������кű��� ��������������
{
	if (conditions == NULL)
		conditions = new vector<Logic>;
	vector<int>* ret = new vector<int>;
	Table* table = catalog->getTable(tableName);
	tableFile* file = new tableFile(tableName);
	int maxId = file->getMaxId();
	for(int i = 0; i <= maxId; i++)
	{
		vector<tableValue> * record = file->getRecord(i, false);
		if (record == NULL) continue;
		if (checkRecord(table, record, conditions)) 
			ret->push_back(i);
	}
	delete table;
	delete file;
	return ret;
}

bool recordManager::defragmentation(string tableName)  //��Ƭ�������������д
{
	return false;
}

vector<tableValue> * recordManager::getRecordById(string tableName,int id)  
{
	tableFile * file = new tableFile(tableName);
	vector<tableValue> * temp = file->getRecord(id);
	delete file;
	return temp;
}

bool recordManager::checkDuplicate(string tableName, vector<tableValue>* record)
{
	Table* table = catalog->getTable(tableName);
	tableFile* file = new tableFile(tableName);
	int maxId = file->getMaxId();
	bool noDuplicate = true;
	for (int i = 0; i <= maxId; i++)
	{
		vector<tableValue> * tmp = file->getRecord(i, false);
		if (tmp == NULL) continue;
		for (int j = 0; j < (*(table->attrList)).size(); j++)
		{
			dbDataType* attr = table->attrList->at(j);
			if (!attr->primary && !attr->unique) continue;
			if (attr->dbType == DB_INT)
			{
				if ((*record)[j].INT == (*tmp)[j].INT)
					noDuplicate = false;
			}
			else if (attr->dbType == DB_FLOAT)
			{
				if ((*record)[j].FLOAT == (*tmp)[j].FLOAT)
					noDuplicate = false;
			}
			else
			{
				int len = attr->getKeyLength();
				if (Logic::compareChar(record->at(j).CHAR, tmp->at(j).CHAR, len) == EQUAL)
					noDuplicate = false;
			}
			if (!noDuplicate) break;
		}
		if (!noDuplicate) break;
	}
	delete table; 
	delete file;
	return noDuplicate;
}