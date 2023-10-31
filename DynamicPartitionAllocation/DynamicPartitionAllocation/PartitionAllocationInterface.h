#pragma once
#include <vector>
#include "Block.h"
class PartitionAllocationInterface {

public:
	virtual int allocate(std::vector<Block>& freeBlockList, int jobSize) = 0;
	virtual int recycle(std::vector<Block>& freeBlockList,Block recycleBlock) = 0;
    virtual void introduce() = 0;
};