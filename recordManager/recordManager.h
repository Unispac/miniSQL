#ifndef recordManager_H
#define recordManager_H
#include<config.h>
#include<table/Table.h>
#include<vector>
#include<set>
#include<logic/Logic.h>
#include<catlogManager/catalogManager.h>
#include<bufferManager/bufferManager.h>

class recordManager
{
public:
	recordManager();
	~recordManager();
	bool createTable(string tableName);
	bool dropTable(string tableName);
	bool insertTableInstance(string tableName, vector<tableValue>*); // ÿ��ֻ��Ҫ���һ�������ֵ���г��������ˡ�
	bool deleteTableInstance(string tableName,vector<int>*);                                //�Ȳ�ѯ����ɾ����  ���ԣ� ��һ������ά���滻�� ֱ��ɾ��..
	vector<int>* select(string tableName,vector<Logic>*);                              //һ��condition����ѯ��
private:
	catalogManager *catalog;
	bufferManager *buffer;
};

#endif // !recordManager_H
