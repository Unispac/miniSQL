#include<table/Table.h>
extern errorReporter *errorHandler;

Table::Table(string tableName, vector<dbDataType*>*attr, vector<string> * index)
{
	name = tableName;
	attrList = attr;
	attributesHaveIndex = index;
	rowCnt = 0;
	colCnt = attr->size();
	int i;
	for (i = 0; i < colCnt; i++)
	{
		if ((*attrList)[i]->primary == true)
		{
			primaryKey = i;
			break;
		}
	}
	if (i == colCnt)
	{
		errorHandler->reportErrorCode(3);
	}
}