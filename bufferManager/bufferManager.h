#ifndef bufferManager_H
#define bufferManager_H

#include<config.h>
#include<string>
#include<map>
using namespace std;

struct bufferNode  //buffer is organized as a queue.
{
	Block * block;
	bufferNode * last, *next;
	bufferNode (Block *content) :block(content) 
	{
		last = next = NULL;
	}
	void remove()
	{
		if(last!=NULL) last->next = next;
		if(next!=NULL) next->last = last;
	}
	~bufferNode() { remove(); }
};


class bufferManager  //organize the buffer, delete the least recently used node from the buffer when buffer is full.
{
public: 
	static const int maxBlockNum;  // the buffer can hold at most maxBlockNum blocks.
	bufferManager();   
	~bufferManager();
	Block * getBlock(const string fileName,int id);   
	bool writeBlock(const string fileName, int id);
	bool writeBlock(Block * block);
	bool acessNode(bufferNode *node);
	Block * appendBlock(const string fileName);

private:
	int blockCnt;
	bufferNode *pilarHead, *pilarTail;
	map<string, bufferNode*>nodeMap;
	Block * loadBlock(const string fileName,int id); 
	bool deleteNode(bufferNode *node);
};


#endif // !bufferManager_H

