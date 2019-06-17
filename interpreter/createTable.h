#ifndef createTable_H
#define createTable_H
#include<config.h>
#include<systemAPI\systemAPI.h>
#include<string>
#include<utils\stringProcesser.h>
#include<interpreter\syntaxError.h>

class createTable
{
public:
	static bool create(string tableName, string element)
	{
		stringProcesser::trim(element);
		vector<string> temp = stringProcesser::split(element,",");
		//bool systemAPI::createTable(string tableName, vector<dbDataType*>*attr)
		//dbDataType(int inType, int inN, string inName,bool inUnique = false, bool inPrimary = false,bool inIndex=false)
		
		vector<dbDataType *>* attr = new vector<dbDataType*>;

		dbDataType * tempAttr;
		int tempType, tempN;
		string tempName;
		bool tempUnique, tempPrimary, tempIndex;
		string primaryKey;

		int size = temp.size();
		
		for (int i = 0; i < size ; i++)
		{  
			vector<string> att = stringProcesser::split(temp[i], " ");
			int cnt = att.size();
			if (cnt > 3 || cnt <2) { syntaxError::Error(); return false; }


			if (att[0] == "primary")
			{
				if (cnt !=2) { syntaxError::Error(); return false; }
				int head = att[1].find_first_of("(");
				int tail = att[1].find_last_of(")");
				if (head == -1 || tail == -1) { syntaxError::Error(); return false; }
				if (att[1].substr(0, head) != "key") { syntaxError::Error(); return false;}
				primaryKey = att[1].substr(head + 1, tail - head - 1);
				stringProcesser::trim(primaryKey);
				continue;
			}
			
			
			
			if (cnt==3) 
			{
				if (att[2] != "unique") { syntaxError::Error(); return false; }
				else tempUnique = TRUE;
			}
			else tempUnique = FALSE;
			
			tempIndex = tempPrimary = false;
			stringProcesser::trim(att[0]);
			tempName = att[0];
			
			stringProcesser::trim(att[1]);
			int head = att[1].find_first_of('(');
			if (head == -1) 
			{
				string type = att[1];
				if (type == "int")tempType = DB_INT;
				else if (type == "float")tempType = DB_FLOAT;
				else { syntaxError::Error(); return false; }
				tempN = 0;
			}
			else
			{
				string type = att[1].substr(0, head);
				if (type != "char") { syntaxError::Error(); return false;}
				else
				{
					tempType = DB_CHAR;
					int tail = att[1].find_last_of(')');
					if (tail == -1) { syntaxError::Error(); return false; }
					type = att[1].substr(head + 1, tail - head - 1);
					stringProcesser::trim(type);
					if (!stringProcesser::stringToInt(type, tempN)) { syntaxError::Error(); return false; }
				}
				//dbDataType(int inType, int inN, string inName,bool inUnique = false, bool inPrimary = false,bool inIndex=false)
			}	
			tempAttr = new dbDataType(tempType, tempN, tempName, tempUnique, tempPrimary, tempIndex);
			attr->push_back(tempAttr);
			cout << i << endl;
		}

		size = attr->size();
		cout << size << endl;
		int i;
		for (i = 0; i < size; i++)
		{
			cout << (*attr)[i]->name << " " << primaryKey << endl;
			if ((*attr)[i]->name == primaryKey)
			{
				(*attr)[i]->primary = true;
				break;
			}
		}

		if (i == size) { syntaxError::Error(); return false; }
		

		for (i = 0; i < size; i++)
		{
			cout << "name : " << (*attr)[i]->name << " type : " << (*attr)[i]->dbType << " unique : " << (*attr)[i]->unique << " primary :" << (*attr)[i]->primary << " index : " << (*attr)[i]->hasIndex << " charLen : " << (*attr)[i]->n << endl;
		}

		/*
		bool result=systemAPI::createTable(tableName, attr);
		for (int i = 0; i < size; i++)
		{
			delete (*attr)[i];
		}
		delete attr;
		return result;
		*/
	}

};

#endif // !
