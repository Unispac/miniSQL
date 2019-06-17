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
#define INSTANCE_TOO_LARGE 14
#define RECORD_DOES_NOT_EXIST 15
#define BPTREE_DUPLICATE_KEY 16
#define BPTREE_KEY_NOT_FOUND 17
#define BPTREE_EMPTY 18
#define NO_INDEX_LIST 19
#define NO_INDEX_NAME 20
#define INDEX_NAME_DUPLICATE 21
#define NOT_UNIQUE 22
#define MULTIPLE_INDEX 23
#define VALUE_TABLE_NOT_MATCH 24
#define CHAR_LEN_EXCEED 25

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
		errorTemplate[14] = " * error [14] : Record too large!";
		errorTemplate[15] = " * error [15] : Unexpected access to an unexisted record.";
		errorTemplate[16] = " * error [16] : Insertion causes duplicate key in a B+ Tree";
		errorTemplate[17] = " * error [17] : B+ Tree does not have the key to be deleted";
		errorTemplate[18] = " * error [18] : B+ Tree Empty and cause fatal error";
		errorTemplate[19] = " * error [19] : Can't get the index list. There are damages on the DB FILES. ";
		errorTemplate[20] = " * error [20] : Index Name Does Not Exist. ";
		errorTemplate[21] = " * error [21] : Duplicate index name, one name corresponds to one index. ";
		errorTemplate[22] = " * error [22] : The attribute on which index is built is not unique. ";
		errorTemplate[23] = " * error [23] : An index has already been built on the attribute. ";
		errorTemplate[24] = " * error [24] : The value can't match the relation schema. ";
		errorTemplate[25] = " * error [25] : The length of the varchar exceeds the upper bound.";	
	}



	void reportErrorCode(int x)
	{
		if (errorTemplate.count(x) != 0)
		{
			cout << errorTemplate[x] << endl;
		}
	}


};


#endif // !errorReporter_H