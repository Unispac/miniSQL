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
	static bool create(string indexName, string target)
	{
		stringProcesser::trim(target);
		string tableName;
		string attrName;
		int head = target.find_first_of("(");
		int tail = target.find_last_of(")");
		if (head == -1 || tail == -1)
		{
			syntaxError::Error();
			return false;
		}
		tableName = target.substr(0, head);
		attrName = target.substr(head + 1, tail - head - 1);
		api->createIndex(indexName, tableName, attrName);
		return false;
	}
};
#endif // !createIndex_H
