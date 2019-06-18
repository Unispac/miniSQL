#ifndef deleteRecord_H
#define deleteRecord_H

#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>
extern systemAPI * api;


class deleteRecord
{
public:
	static int Delete(string tableName, string conditionString)
	{
		stringProcesser::trim(tableName);
		stringProcesser::trim(conditionString);
		Table * table = api->getTable(tableName);
		if (table == NULL)return -1;
		//vector<dbDataType*> * attrList;
		vector<dbDataType*> *attrList = table->attrList;
		vector<Logic> * condition = new vector<Logic>;

		if (!conditionString.empty())
		{
			vector<string> sCondition = stringProcesser::split(conditionString, "and");
			int size = sCondition.size();
			Logic * tCondition;

			for (int i = 0; i < size; i++)
			{
				int type = stringProcesser::getConditionType(sCondition[i]);

				if (type == -1) { syntaxError::Error(); return -1; }

				tCondition = stringProcesser::getLogic(sCondition[i], type, attrList);
				if (tCondition == NULL)
				{
					syntaxError::Error();
					int s = condition->size();
					delete condition;
					return -1;
				}
				condition->push_back(*tCondition);
				delete tCondition;
			}
		}

		/*int kk = condition->size();
		for(int i = 0;i<kk;i++)
		{
			cout << (*condition)[i].valName << " " << (*condition)[i].opcode << " ";
			cout << (*condition)[i].immediate.INT<< endl;
		}*/
		int result = api->remove(tableName, condition);
		delete condition;
		return result;
	}
};

#endif // !deleteRecord_H
