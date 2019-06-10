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
				binaryFile::writeInt(p, value);
				p += 4;
				break;
			case DB_FLOAT:
				float value = (*list)[i].FLOAT;
				binaryFile::writeFloat(p, value);
				p += 4;
				break;
			case DB_CHAR:
				int n = (*typeList)[i]->n;
				binaryFile::writeChar(p, (*list)[i].CHAR, n);
				p += n;
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
			binaryFile::readInt(p, &((*list)[i].INT));
			p += 4;
			break;
		case DB_FLOAT:
			binaryFile::readFloat(p, &((*list)[i].FLOAT));
			p += 4;
			break;
		case DB_CHAR:
			int n = (*typeList)[i]->n;
			binaryFile::readChar(p, (*list)[i].CHAR,n);
			p += n;
			break;
		}
	}
	return true;
}

bool binaryFile::writeInt(char *p, int x)
{
	memcpy(p, &x, 4);
	return true;
}
bool binaryFile::writeFloat(char *p, float x)
{
	memcpy(p, &x, 4);
	return true;
}
bool binaryFile::writeChar(char *p, char *data, int n)
{
	memcpy(p, data, n);
	return true;
}

bool binaryFile::readInt(char *p, int *x)
{
	memcpy(x, p, 4);
	return true;
}
bool binaryFile::readFloat(char *p, float *x)
{
	memcpy(x, p, 4);
	return true;
}
bool binaryFile::readChar(char *p, char *data, int n)
{
	memcpy(data, p, n);
	return true;
}