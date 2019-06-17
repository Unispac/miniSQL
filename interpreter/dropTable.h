#ifndef dropTable_H
#define dropTable_H
#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;

class dropTable
{
public:
	static bool drop(string tableName)
	{
		return api->dropTable(tableName);
	}
};
#endif // !dropTable_H
