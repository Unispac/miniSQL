#ifndef recordManager_H
#define recordManager_H
#include<config.h>
#include<table/Table.h>
#include<vector>
#include<set>
#include<logic/Logic.h>
#include<catalogManager/catalogManager.h>
#include<bufferManager/bufferManager.h>
#include<table/tableFile.h>

class recordManager
{
public:
	recordManager();
	~recordManager();
	bool createTable(string tableName);
	bool dropTable(string tableName);
	int insertTableInstance(string tableName, vector<tableValue>*); // ÿ��ֻ��Ҫ����һ�������ֵ���г��������ˡ�
	bool deleteTableInstance(string tableName,vector<int>*);                                //�Ȳ�ѯ����ɾ����  ���ԣ� ��һ������ά���滻�� ֱ��ɾ��..
	bool defragmentation(string tableName);   //��Ƭ����
	vector<int>* select(string tableName,vector<Logic>* conditions);                              //һ��condition������ѯ��
	bool checkRecord(Table* table, vector<tableValue>* record, vector<Logic>* conditions);
	vector<tableValue> * getRecordById(string tableName,int id);
private:
	catalogManager *catalog;
	bufferManager *buffer;
};

#endif // !recordManager_H
