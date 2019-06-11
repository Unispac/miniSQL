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
	bool insertTableInstance(string tableName, vector<tableValue>*); // 每次只需要添加一条表项，将值表列出来就行了。
	bool deleteTableInstance(string tableName,vector<int>*);                                //先查询再做删除？  策略？ 用一个链表维护替换？ 直接删？..
	bool defragmentation(string tableName);   //碎片整理
	vector<int>* select(string tableName,vector<Logic>*);                              //一个condition表，查询。
	vector<tableValue> * getRecordById(string tableName,int id);
private:
	catalogManager *catalog;
	bufferManager *buffer;
};

#endif // !recordManager_H
