#ifndef systemAPI_H
#define systemAPI_H

#include<config.h>
#include<table/Table.h>
#include<vector>
#include<logic/Logic.h>

/*
	�﷨������Ҫ��֤�﷨�Ϸ���
	API��������Լ�������������ԡ�
	�������߲�ģ���ڼ��鵽������Ϣ��ʱ���ͨ������ģ�鷵�ر�����Ϣ��

	�ײ�ģ��Ҳ����һ���ļ���,��ͨ��errorReporterִ�еġ�ԭ������˵��
	����ײ�ģ��û��bug���߲����û��bug���ײ㶼��ͨ�����顣��������ײ�
	���������������ֹ�����׳����ԵĴ�����Ϣ��
*/
class systemAPI  
{
public:
	static bool createTable(string tableName, vector<dbDataType*>*attr); 
	/*
		����һ������+���Ա�
		����catalogManager������Ϣ�ļ���������ͷ��Ϣ������������Ϣ��
		����recordManager�������ļ�(��ռ��)��Ϊ���ļ�д���ļ�ͷ��
		����indexManager���������ļ���Ϊ�����ļ�д���ļ�ͷ�����û���ļ�ͷ�����ˣ���
	*/
	static bool dropTable(string tableName);
	/*
		����һ������. ��������ɾ��.
		���catalogManager��ȡ�����Ϣ��
		���ݱ����Ϣ����recordManager������ļ�������indexManager��������ļ���
		����ٴ�catalogManager�н���ͷ��Ϣ��������Ϣȫ����ա�
	*/
	static bool createIndex(string indexName, string tableName, string attrName);
	/*
		������catalogManager�и��±�ͷ��������Ϣ��
		Ȼ���ٵ���indexManager���������ļ���
	*/
	static bool dropIndex(string indexName);
	/*
		ͬcreateIndex.
	*/
	

};

#endif