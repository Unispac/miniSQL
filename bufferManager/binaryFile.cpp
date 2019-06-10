#include<bufferManager/binaryFile.h>
#define LOW_8BITS 255
bool binaryFile::writeTableValue(char *p, vector<tableValue>* list, Table* table)
{
	int len = table->colCnt;

	vector<dbDataType*> * typeList = table->attrList;

	*p = 1;
	p++;

	for (int i = 0; i < len; i++)
	{
		switch ((*typeList)[i]->dbType)
		{
			case DB_INT:
				binaryFile::writeInt(p, (*list)[i].INT);
				p += 4;
				break;
			case DB_FLOAT:
				binaryFile::writeFloat(p, (*list)[i].FLOAT);
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
	p++; //不需要读有效位标记
	for (int i = 0; i < len; i++)
	{
		tableValue temp;
		switch ((*typeList)[i]->dbType)
		{
		case DB_INT:
			binaryFile::readInt(p, &(temp.INT));
			p += 4;
			break;
		case DB_FLOAT:
			binaryFile::readFloat(p, &(temp.FLOAT));
			p += 4;
			break;
		case DB_CHAR:
			int n = (*typeList)[i]->n;
			temp.CHAR = new char[n];
			binaryFile::readChar(p, temp.CHAR,n);
			p += n;
			break;
		}
		list->push_back(temp);
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