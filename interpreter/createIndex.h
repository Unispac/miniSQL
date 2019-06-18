#ifndef createIndex_H
#define createIndex_H

#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;

class createIndex
{
public:
	static bool create(string indexName, string tableName,string target)
	{
	
		stringProcesser::trim(target);
		stringProcesser::trim(tableName);
		return api->createIndex(indexName, tableName, target);
	}
};
#endif // !createIndex_H
