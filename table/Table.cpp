#include<table/Table.h>
extern errorReporter *errorHandler;

Table::Table(string tableName, vector<dbDataType*>*attr, vector<string> * index)
{
	name = tableName;
	attrList = attr;
	attributesHaveIndex = index;
	colCnt = attr->size();
	int i,temp;
	primaryKey = -1;
	
	sizePerInstance = 0;

	for (i = 0; i < colCnt; i++)
	{
		if ((*attrList)[i]->primary == true)
		{
			if (primaryKey != -1)errorHandler->reportErrorCode(TOO_MANY_PRIMARY);
			else primaryKey = i;
		}
		temp = (*attrList)[i]->dbType;
		if (temp == DB_FLOAT || temp == DB_INT)sizePerInstance += 4;
		else if (temp == DB_CHAR)sizePerInstance += (*attrList)[i]->n;
		else
		{
			errorHandler->reportErrorCode(ILLEGAL_DATA_TYPE);
		}
	}
	sizePerInstance += 1; // 每一个record的开头是一个标记符号，标记这是否是一个空位。

	if (sizePerInstance > blockSize)
		errorHandler->reportErrorCode(INSTANCE_TOO_LARGE);

	instanceNumPerBlock = floor(blockSize / sizePerInstance);

	if (primaryKey==-1)
		errorHandler->reportErrorCode(NO_PRIMARYKEY);
}