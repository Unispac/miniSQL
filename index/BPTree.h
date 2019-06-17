#ifndef _BPTree_
#define _BPTree_

#include <vector>
#include <string>

#define BPTREE_FAILED -1
#define BPTREE_NORMAL 0
#define BPTREE_ADD    1
#define BPTREE_REMOVE 2
#define BPTREE_CHANGE 3

using namespace std;

class BPTree
{
public:

	static void createFile(const char* _filename, int _keyLength, int _dataType, int _fan_out = -1);

	BPTree(const char* _filename);

	~BPTree();

	int find(const char* _key);

	bool add(const char* _key, int _value);

	bool remove(const char* _key);

private:
	int fan_out;
	int keyLength;
	int dataType;
	int nodeCount;
	int root;
	int firstEmpty;
	string filename;

	// Key-value to deal with
	char* key;
	int value;

	int find(int id);
	int add(int id);
	int remove(int id, int sibId, bool leftSib, const char* parentKey);
	int getFirstEmpty();
	void removeBlock(int id);
	void updateHeader();

};

#endif