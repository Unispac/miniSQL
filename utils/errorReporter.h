#ifndef errorReporter_H
#define errorReporter_H

#include<iostream>
#include<string>
#include<map>
using namespace std;

#define BUFFER_EXPLOSION 0    //block is full and can't be free.
#define BLOCK_READING_FAIL 1  //block file read fail..
#define BLOCK_WRITING_FAIL 2 //block file write fail..
#define NO_PRIMARYKEY 3 //no primary key.
#define NO_TABLE 4 //table doesn't exist.
#define TOO_MANY_ATTR 5 //number of attributes exceeds the limitation.
#define TOO_LARGE_CHAR 6  
#define NO_TABLE_LIST 7
#define TABLE_ALREADY_EXIST 8
#define ATTR_NAME_REDUNDANCY 9
#define CAN_NOT_SET_INDEX 10
#define ILLEGAL_DATA_TYPE 11
#define TOO_MANY_PRIMARY 12
#define NO_TABLE_TO_DROP 13


class errorReporter
{
public:
	map<int, string>errorTemplate;
	
	errorReporter()
	{
		errorTemplate[0] = " * error [0] : Unexpected buffer explosion, no resource in buffer can be released.";
		errorTemplate[1] = " * error [1] : Error when reading table block file.. Make sure the filepath is correct.";
		errorTemplate[2] = " * error [2] : Error when writing table block file.. Make sure the filepath is correct.";
		errorTemplate[3] = " * error [3] : Table must have a primary key.";
		errorTemplate[4] = " * error [4] : Error when get table info. Table doesn't exist.";
		errorTemplate[5] = " * error [5] : Too many attributes in the table.";
		errorTemplate[6] = " * error [6] : The size of varchar is too large. ";
		errorTemplate[7] = " * error [7] : Can't get the table list. There are damages on the DB FILES.";
		errorTemplate[8] = " * error [8] : Table already exists. Redundancy is not allowed.";
		errorTemplate[9] = " * error [9] : No redundancy is allowed among the attribute names in the same table. ";
		errorTemplate[10] = " * error [10] : Index is only allowed for key that is unique.";
		errorTemplate[11] = " * error [11] : Illigal Data Type!";
		errorTemplate[12] = " * error [12] : Too many primary keys! Only one primary key is allowed!";
		errorTemplate[13] = " * error [13] : Table doesn't exist, can not drop it.";
	}



	void reportErrorCode(int x)
	{
		if (errorTemplate.count(x) != 0)
		{
			cout << errorTemplate[x];
			system("pause");
			exit(0);
		}
	}


};


#endif // !errorReporter_H