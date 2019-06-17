#ifndef _BPTreeNode_
#define _BPTreeNode_

#include <string>
#include <vector>
#include <config.h>
using namespace std;

class BPTreeNode
{
public:
	// from file
	BPTreeNode(const char* _filename, int _id, int _keyLength, int _dataType);
	// empty
	BPTreeNode(const char* _filename, int _id, int _keyLength, int _dataType, bool _leaf, int firstPtr);

	~BPTreeNode();

	int getSize() const;
	int getKeyLength() const;
	bool isLeaf() const;
	const char* getKey(int pos) const;
	int getPtr(int pos) const;
	int findPos(const char* key);
	void placeKey(int pos, const char* key);
	void placePtr(int pos, int ptr);
	void setRemoved();
	void insert(int pos, const char* key, int ptr);
	void remove(int pos);
	BPTreeNode* split(int newId, char* newKey);
	const char* borrowFromSib(BPTreeNode* sib, bool isLeft, const char* parentKey);
	void mergeRight(BPTreeNode* sib, const char* parentKey);
private:
	string filename;
	int id;
	int size;
	int keyLength;
	int dataType;
	bool leaf;
	bool dirty;
	bool removed;
	vector<char*> keys;
	vector<int> ptrs;

	int nodecmp(const char* a, const char* b)
	{
		if (dataType == DB_INT)
		{
			int ta, tb;
			binaryFile::readInt(a, &ta);
			binaryFile::readInt(b, &tb);
			return ta - tb;
		}
		else if (dataType == DB_CHAR)
		{
			return memcmp(a, b, keyLength);
		}
		else if (dataType == DB_FLOAT)
		{
			float ta, tb;
			binaryFile::readFloat(a, &ta);
			binaryFile::readFloat(b, &tb);
			if (ta - tb < 0) return -1;
			else if (ta - tb > 0) return 1;
			else return 0;
		}
		cerr << "ERROR: DataType Error, Not any of DB_INT/FLOAT/CHAR" << endl;
		return 0;
	}
};

#endif