#ifndef binaryFile_H
#define bianryFile_H
#include<table/Table.h>
#include<config.h>
#include<vector>
class binaryFile
{
public:
	static bool writeTableValue(char *p, vector<tableValue>* list, Table* table);
	static bool readTableValue(char *p, vector<tableValue>* list, Table *table);
};
#endif // !binaryFile_H
