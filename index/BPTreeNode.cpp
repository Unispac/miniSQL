#include <iostream>
#include <cstring>
#include <algorithm>

#include <config.h>
#include <bufferManager/bufferManager.h>
#include <index/BPTreeNode.h>
#include <bufferManager/binaryFile.h>

BPTreeNode::BPTreeNode(const char* _filename, int _id, int _keyLength, int _dataType)
	:filename(_filename), id(_id), keyLength(_keyLength), dataType(_dataType)
{
	bufferManager* manager = new bufferManager();
	Block* block = manager->getBlock(_filename, _id);
	char* data = block->data;

	size = *(reinterpret_cast<int*>(data));
	keys.push_back(NULL);
	ptrs.push_back(*(reinterpret_cast<int*>(data + 4)));
	leaf = ptrs[0] < 0 ? true : false;
	removed = dirty = false;
	for (int i = 1, bias = 8; i <= size; i++, bias += keyLength + 4)
	{
		char* k = new char[keyLength];
		memcpy(k, data + bias, keyLength);
		keys.push_back(k);
		ptrs.push_back(*(reinterpret_cast<int*>(data + bias + keyLength)));
	}
}

BPTreeNode::BPTreeNode(const char* _filename, int _id, int _keyLength, int _dataType, bool _leaf, int firstPtr)
	:filename(_filename), id(_id), keyLength(_keyLength), leaf(_leaf), dataType(_dataType)
{
	size = 0;
	keys.push_back(NULL);
	ptrs.push_back(firstPtr);
	dirty = true;
	removed = false;
}

BPTreeNode::~BPTreeNode()
{
	if (dirty && !removed)
	{
		bufferManager* manager = new bufferManager();
		Block* block = manager->getBlock(filename.c_str(), id);
		char* data = block->data;
		binaryFile::writeInt(data, size);
		binaryFile::writeInt(data + 4, ptrs[0]);
		for (int i = 1, bias = 8; i <= size; i++, bias += keyLength + 4)
		{
			binaryFile::writeChar(data + bias, keys[i], keyLength);
			binaryFile::writeInt(data + bias + keyLength, ptrs[i]);
		}
		manager->writeBlock(block);
	}
	for (auto it : keys)
	{
		if (it != NULL) delete[] it;
	}
}

int BPTreeNode::getSize() const
{
	return size;
}

int BPTreeNode::getKeyLength() const
{
	return keyLength;
}

bool BPTreeNode::isLeaf() const
{
	return leaf;
}

const char* BPTreeNode::getKey(int pos) const
{
	if (pos > size || pos <= 0)
	{
		cout << "ERROR: BPTreeNode::getKey Position " << pos << " out of range, size = " << size << endl;
		return NULL;
	}
	return keys[pos];
}

int BPTreeNode::getPtr(int pos) const
{
	if (pos > size || pos < 0)
	{
		cout << "ERROR: BPTreeNode::getPre Position " << pos << " out of range, size = " << size << endl;
		return -1;
	}
	return ptrs[pos];
}

int BPTreeNode::findPos(const char* key) 
{
	return upper_bound(
		keys.begin() + 1, keys.end(), key,
		[&](const char* a, const char* b) { return nodecmp(a, b) < 0; }
	) - (keys.begin() + 1);
}

void BPTreeNode::placeKey(int pos, const char* key)
{
	if (pos > size || pos <= 0)
	{
		cout << "ERROR: BPTreeNode::placeKey Position " << pos << " out of range, size = " << size << endl;
		return;
	}

	dirty = true;
	memcpy(keys[pos], key, keyLength);
}

void BPTreeNode::placePtr(int pos, int ptr)
{
	if (pos > size || pos < 0)
	{
		cout << "ERROR: BPTreeNode::placePtr Position " << pos << " out of range, size = " << size << endl;
		return;
	}

	dirty = true;
	ptrs[pos] = ptr;
}

void BPTreeNode::setRemoved()
{
	removed = true;
}

void BPTreeNode::insert(int pos, const char* key, int ptr)
{
	if (pos > size || pos < 0)
	{
		cout << "ERROR: BPTreeNode::insert Position " << pos << " out of range, size = " << size << endl;
		return;
	}

	dirty = true;
	char* k = new char[keyLength];
	memcpy(k, key, keyLength);
	if (pos == size)
	{
		keys.push_back(k);
		ptrs.push_back(ptr);
	}
	else
	{
		keys.insert(keys.begin() + pos + 1, k);
		ptrs.insert(ptrs.begin() + pos + 1, ptr);
	}
	size++;
}

void BPTreeNode::remove(int pos)
{
	if (pos > size || pos <= 0)
	{
		cout << "ERROR: BPTreeNode::remove Position " << pos << " out of range, size = " << size << endl;
		return;
	}

	dirty = true;
	if (pos == size)
	{
		keys.pop_back();
		ptrs.pop_back();
	}
	else
	{
		keys.erase(keys.begin() + pos);
		ptrs.erase(ptrs.begin() + pos);
	}
	size--;
}

// newKey represents the key to be pushed to the parent
BPTreeNode* BPTreeNode::split(int newId, char* newKey)
{
	dirty = true;
	int pos = size / 2;
	if (!leaf) pos++;
	memcpy(newKey, keys[size / 2 + 1], keyLength);
	BPTreeNode* ret = new BPTreeNode(filename.c_str(), newId, keyLength, dataType, leaf, leaf ? -1 : ptrs[pos]);
	
	for (int i = pos + 1; i <= size; i++)
	{
		ret->insert(ret->getSize(), keys[i], ptrs[i]);
	}

	size /= 2;
	keys.resize(size + 1);
	ptrs.resize(size + 1);

	return ret;
}

// Borrow a key from sibling. Return new parent key
const char* BPTreeNode::borrowFromSib(BPTreeNode* sib, bool isLeft, const char* parentKey)
{
	dirty = true;

	if (isLeft)
	{
		// Borrow from left sibling
		int sibSize = sib->getSize();
		const char* sibKey = sib->getKey(sibSize);
		int sibPtr = sib->getPtr(sibSize);
		sib->remove(sibSize);

		if (leaf)
			insert(0, sibKey, sibPtr);
		else
		{
			int ptr = ptrs[0];
			ptrs[0] = sibPtr;
			insert(0, parentKey, ptr);
		}

		return sibKey;
	}
	else
	{
		// Borrow from right sibling
		const char* sibKey = sib->getKey(1);
		int sibPtr0 = sib->getPtr(0);
		int sibPtr1 = sib->getPtr(1);
		sib->remove(1);

		if (leaf)
		{
			insert(size, sibKey, sibPtr1);
			return sib->getKey(1);
		}
		else
		{
			sib->placePtr(0, sibPtr1);
			insert(size, parentKey, sibPtr0);
			return sibKey;
		}
	}
}

// Merge right sibling
void BPTreeNode::mergeRight(BPTreeNode* sib, const char* parentKey)
{
	dirty = true;

	int sibSize = sib->getSize();
	if (!leaf)
		insert(size, parentKey, sib->getPtr(0));
	for (int i = 1; i <= sibSize; i++)
		insert(size, sib->getKey(i), sib->getPtr(i));
}
