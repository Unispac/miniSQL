#include <iostream>
#include <config.h>
#include <index/Index.h>

using namespace std;

Index::Index(const char* data)
{
	name = data;
	tableName = data + maxTableAttr;
	colName = data + maxTableAttr * 2;
}

const char* Index::getName() const
{
	return name.c_str();
}

const char* Index::getTableName() const
{
	return tableName.c_str();
}

const char* Index::getColName() const
{
	return colName.c_str();
}