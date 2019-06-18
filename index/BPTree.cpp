#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include <config.h>
#include <utils/errorReporter.h>
#include <bufferManager/bufferManager.h>
#include <bufferManager/binaryFile.h>
#include <index/BPTreeNode.h>
#include <index/BPTree.h>

using namespace std;

extern errorReporter* errorHandler;
void BPTree::createFile(const char* _filename, int _keyLength, int _dataType, int _fan_out)
{
	if (_fan_out == -1)
		_fan_out = (blockSize - 8) / (_keyLength + 4) + 1;
	FILE* file = fopen(("data/" + string(_filename) + ".mdb").c_str(), "wb");
	int header[] = { _fan_out, _keyLength, _dataType, 0, -1, -1 };
	fwrite(header, 4, 6, file);
	fclose(file);
}

BPTree::BPTree(const char* _filename) : filename(_filename)
{
	bufferManager* manager = new bufferManager();
	Block* header = manager->getBlock(_filename, 0);
	char* data = header->data;
	// Get header information
	binaryFile::readInt(data, &fan_out);
	binaryFile::readInt(data + 4, &keyLength);
	binaryFile::readInt(data + 8, &dataType);
	binaryFile::readInt(data + 12, &nodeCount);
	binaryFile::readInt(data + 16, &root);
	binaryFile::readInt(data + 20, &firstEmpty);

	key = new char[keyLength];
}

BPTree::~BPTree()
{
	delete[] key;
}

int BPTree::find(const char* _key)
{
	memcpy(key, _key, keyLength);
	if (root < 0)
	{
		//errorHandler->reportErrorCode(BPTREE_EMPTY);
		return BPTREE_FAILED;
	}
	else
		return find(root);
}

bool BPTree::add(const char* _key, int _value)
{
	memcpy(key, _key, keyLength);
	value = _value;
	int cond = root < 0 ? BPTREE_ADD : add(root);
	if (cond == BPTREE_ADD)
	{
		int newroot = getFirstEmpty();
		BPTreeNode* rt = new BPTreeNode(filename.c_str(), newroot, keyLength, dataType, root < 0, root < 0 ? -1 : root);
		rt->insert(0, key, value);
		delete rt;
		root = newroot;
	}
	updateHeader();

	return cond != BPTREE_FAILED;
}

bool BPTree::remove(const char* _key)
{
	memcpy(key, _key, keyLength);
	int cond = root < 0 ? BPTREE_FAILED : remove(root, 0, true, NULL);
	updateHeader();
	return cond != BPTREE_FAILED;
}


// private

int BPTree::find(int id)
{
	BPTreeNode* node = new BPTreeNode(filename.c_str(), id, keyLength, dataType);
	int pos = node->findPos(key);

	int cond = BPTREE_FAILED;
	if (node->isLeaf())
	{
		// Check if key is found
		if (pos > 0)
		{
			const char* k = node->getKey(pos);
			if (memcmp(key, k, keyLength) == 0)
				cond = node->getPtr(pos);
		}
	}
	else
		cond = find(node->getPtr(pos));

	delete node;
	return cond;
}

// Recursive function for adding key-value pair
int BPTree::add(int id)
{
	BPTreeNode* node = new BPTreeNode(filename.c_str(), id, keyLength, dataType);
	int pos = node->findPos(key);

	int cond = node->isLeaf() ? BPTREE_ADD : add(node->getPtr(pos));
	int ret = BPTREE_NORMAL;

	if (node->isLeaf() && pos > 0)
	{
		// Check for duplicate
		const char* k = node->getKey(pos);
		if (memcmp(key, k, keyLength) == 0)
			cond = BPTREE_FAILED;
	}

	if (cond == BPTREE_FAILED)
		// Duplicate key
		ret = BPTREE_FAILED;
	else if (cond == BPTREE_ADD)
	{
		// Add new key-value
		node->insert(pos, key, value);

		if (node->getSize() >= fan_out)
		{
			// Node full. Split
			int newId = getFirstEmpty();
			BPTreeNode* newNode = node->split(newId, key);
			value = newId;

			delete newNode;
			ret = BPTREE_ADD;
		}
	}
	// if cond == BPTree_NORMAL, then nothing to be done

	delete node;
	return ret;
}

// Recursive function for deleting key-value pair
int BPTree::remove(int id, int sibId, bool leftSib, const char* parentKey)
{
	BPTreeNode* node = new BPTreeNode(filename.c_str(), id, keyLength, dataType);
	BPTreeNode* sib = NULL;
	if (id != root)
		sib = new BPTreeNode(filename.c_str(), sibId, keyLength, dataType);
	int pos = node->findPos(key);

	int cond;
	if (node->isLeaf())
		cond = BPTREE_FAILED;
	else
	{
		int nxtId = node->getPtr(pos);
		// select right child only if key < everykey in this node
		int nxtSibId = node->getPtr(pos > 0 ? pos - 1 : pos + 1); 
		const char* nxtParentKey = node->getKey(pos > 0 ? pos : pos + 1);
		cond = remove(nxtId, nxtSibId, pos > 0, nxtParentKey);
	}

	if (node->isLeaf())
	{
		// Check if key is found
		if (pos > 0)
		{
			const char* k = node->getKey(pos);
			if (memcmp(key, k, keyLength) == 0)
				cond = BPTREE_REMOVE;
		}
	}

	int ret = BPTREE_NORMAL;
	if (cond == BPTREE_FAILED)
		// Key not found
		ret = BPTREE_FAILED;
	else if (cond == BPTREE_CHANGE)
		// Change key
		// if pos > 0, Sib (ptr[pos-1]) is left of id (ptr[pos]), thus key[pos] should be changed
		// otherwise Sib (ptr[pos+1]) is right of id (ptr[pos]), thus key[pos+1] should be changed
		node->placeKey(pos > 0 ? pos : pos + 1, key);
	else if (cond == BPTREE_REMOVE)
	{
		// Delete key
		node->remove(pos > 0 ? pos : pos + 1);

		if (id == root)
		{
			if (node->getSize() == 0)
			{
				root = node->getPtr(0);
				removeBlock(id);
				node->setRemoved();
			}
		}
		else
		{
			int lim = (fan_out + 1) / 2 - 1;
			if (node->getSize() < lim)
			{
				if (sib->getSize() > lim)
				{
					// Borrow key from sibling
					// Modify the parent node with parentKey
					const char* k = node->borrowFromSib(sib, leftSib, parentKey);
					memcpy(key, k, keyLength);
					ret = BPTREE_CHANGE;
				}
				else
				{
					// Merge sibling
					if (leftSib)
					{
						// id is merged (removed)
						sib->mergeRight(node, parentKey);
						removeBlock(id);
						node->setRemoved();
					}
					else
					{
						// sib is merged (removed)
						node->mergeRight(sib, parentKey);
						removeBlock(sibId);
						sib->setRemoved();
					}
					ret = BPTREE_REMOVE;
				}
			}
		}
	}

	delete node;
	if (sib != NULL)
		delete sib;
	return ret;
}

// Get first empty block id
int BPTree::getFirstEmpty()
{
	// the behavior of firstEmpty
	// seems like a linked list
	// when all empty blocks are used, a negative number will be passed to firstEmpty
	// and next time we will know that all blocks are full
	if (firstEmpty < 0)
		return ++nodeCount;

	int ret = firstEmpty;
	bufferManager* manager = new bufferManager();
	Block* block = manager->getBlock(filename.c_str(), firstEmpty);
	binaryFile::readInt(block->data, &firstEmpty);
	return ret;
}

// Remove block in file
void BPTree::removeBlock(int id)
{
	// when new block is available
	// change the firstEmpty pointer
	// and leave the current block for next time query the next Empty Block
	// when the first empty block appears, we leave a negative number there
	// next time we come back to here we know that blocks are full
	bufferManager* manager = new bufferManager();
	Block* block = manager->getBlock(filename.c_str(), id);
	memcpy(block->data, &firstEmpty, 4);
	firstEmpty = id;

	manager->writeBlock(block);
}

// Update header information
void BPTree::updateHeader()
{
	bufferManager* manager = new bufferManager();
	Block* block = manager->getBlock(filename.c_str(), 0);

	memcpy(block->data + 12, &nodeCount, 4);
	memcpy(block->data + 16, &root, 4);
	memcpy(block->data + 20, &firstEmpty, 4);

	manager->writeBlock(block);
}