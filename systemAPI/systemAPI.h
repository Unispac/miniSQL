#ifndef systemAPI_H
#define systemAPI_H

#include<config.h>
#include<table/Table.h>
#include<vector>
#include<logic/Logic.h>
#include<recordManager\recordManager.h>
#include<catalogManager\catalogManager.h>
#include<indexManager\indexManager.h>

/*
	语法解析层要保证语法合法。
	API层则会检验约束条件的满足性。
	这两个高层模块在检验到错误信息的时候会通过辅助模块返回报错信息。

	底层模块也会有一定的检验,是通过errorReporter执行的。原则上来说，
	如果底层模块没有bug，高层检验没有bug，底层都能通过检验。所以如果底层
	出错，程序会立刻终止，并抛出简略的错误信息。
*/
class systemAPI  
{
public:
	systemAPI();
	~systemAPI();
	bool createTable(string tableName, vector<dbDataType*>*attr); 
	/*
		接收一个表名+属性表。
		调用catalogManager创建信息文件，包括表头信息和索引定义信息。
		调用recordManager创建表文件(先占坑)，为表文件写好文件头。
		调用indexManager创建索引文件，为索引文件写好文件头（如果没有文件头就算了）。
	*/
	bool dropTable(string tableName);
	/*
		接收一个表名. 对它进行删除.
		会从catalogManager获取表的信息。
		根据表的信息调用recordManager清除表文件，调用indexManager清除索引文件。
		最后再从catalogManager中将表头信息和索引信息全部清空。
	*/
	bool createIndex(string indexName, string tableName, string attrName);
	/*
		会先在catalogManager中更新表头和索引信息。
		然后再调用indexManager创建索引文件。
	*/
	bool dropIndex(string indexName);
	/*
		同createIndex.
	*/
	vector<vector<tableValue>*> * selet(string tableName,vector<Logic>* conditions);

	bool insert(string tableName, vector<tableValue>* value);
	bool remove(string tableName, vector<Logic>* conditions);

private:
	bufferManager * buffer;
	recordManager * recorder;
	catalogManager * catalog;
	indexManager * indexer;
};

#endif