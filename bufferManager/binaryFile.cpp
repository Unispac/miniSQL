#include<bufferManager/binaryFile.h>
#define LOW_8BITS 255
bool binaryFile::writeTableValue(char *p, vector<tableValue>* list, Table* table)
{
	int len = table->colCnt;
	vector<dbDataType*> * typeList = table->attrList;
	for (int i = 0; i < len; i++)
	{
		switch ((*typeList)[i]->dbType)
		{
			case DB_INT:
				int value = (*list)[i].INT;
				for (int j = 0; j < 4; j++)
				{
					*p = value & LOW_8BITS;
					p++;
					value >>= 8;
				}
				break;
			case DB_FLOAT:
				float value = (*list)[i].FLOAT;
				for (int j = 0; j < 4; j++)
				{
					*p = value & LOW_8BITS;
					p++;
					value >>= 8;
				}
				break;
			case DB_CHAR:
				int n = (*typeList)[i]->n;
				for (int j = 0; j < n; j++)
				{
					*p = (*list)[i].CHAR[j];
					p++;
				}
				break;
		}
	}
	return true;
}
bool binaryFile::readTableValue(char *p, vector<tableValue>* list, Table *table)
{
	int len = table->colCnt;
	vector<dbDataType*> * typeList = table->attrList;
	for (int i = 0; i < len; i++)
	{
		switch ((*typeList)[i]->dbType)
		{
		case DB_INT:
			int value = 0;
			for (int j = 0; j < 4; j++)
			{
				value |= *p;
				value <<= 8;
				p++;
			}
			(*list)[i].INT = value;
			break;
		case DB_FLOAT:
			float value = 0;
			for (int j = 0; j < 4; j++)
			{
				value |= *p;
				value <<= 8;
				p++;
			}
			(*list)[i].FLOAT = value;
			break;
		case DB_CHAR:
			int n = (*typeList)[i]->n;
			for (int j = 0; j < n; j++)
			{
				(*list)[i].CHAR[j] = *p;
				p++;
			}
			break;
		}
	}
	return true;
}