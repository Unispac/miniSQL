#ifndef dropIndex_H
#define dropIndex_H

#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;

class dropIndex
{
public:
	static bool drop(string indexName)
	{
		return api->dropIndex(indexName);
	}
};
#endif // !1
