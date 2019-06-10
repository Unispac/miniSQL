#ifndef CONFIG_H
#define CONFIG_H

#include<string>
#include<utils/errorReporter.h>
#include<Windows.h>
using namespace std;

#define blockSize 4096   // 4KB for each block
#define bufferMaxSize 1024  // size of buffer cache : 4MB 
#define DB_FLOAT 201900
#define DB_INT 201901
#define DB_CHAR 201902
#define maxTableAttr 32
#define maxCharLen 255



class Block    // data structure for a single block in the main memory.
{
public:
	string fileName;
	int id;
	bool pin;  // when a block is locked , it can't be delete from buffer.
	bool changed; //whether this block has been modified.
	char data[blockSize];
	Block(const string PATH, int ID) : fileName(PATH), id(ID)
	{
		pin = changed = false;
	}
};


#endif // !CONFIG_H
