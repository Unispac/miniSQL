#ifndef Select_H
#define Select_H

#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;

class Select
{
public:
	static vector<vector<tableValue>*> * get(string tableName, string conditionString)
	{
		stringProcesser::trim(tableName);
		stringProcesser::trim(conditionString);
		cout << conditionString << endl;
		Table * table = api->getTable(tableName);
		//vector<dbDataType*> * attrList;

		vector<dbDataType*> *attrList = table->attrList;
		vector<string> sCondition = stringProcesser::split(conditionString, "and");
		int size = sCondition.size();
		Logic * tCondition;
		vector<Logic> * condition = new vector<Logic>;
		for (int i = 0; i < size; i++)
		{
			int type = stringProcesser::getConditionType(sCondition[i]);
			if (type == -1) { syntaxError::Error(); return NULL; }
			tCondition = stringProcesser::getLogic(sCondition[i], type, (*attrList)[i]);
			if (tCondition == NULL)
			{
				syntaxError::Error(); 
				int s = condition->size();
				delete condition;
				return NULL;
			}
			condition->push_back(*tCondition);
			delete tCondition;
		}
		vector<vector<tableValue>*> * result = api->select(tableName, condition);
		delete condition;
		return result;
	}
};

#endif // !Select_H
