#pragma once
#include "PartitionAllocationInterface.h"
class FirstFit :public PartitionAllocationInterface
{
public:
    void introduce() {
        std::cout << "FirstFit" << std::endl;
    }
	int allocate(std::vector<Block>& freeBlockList, int jobSize) {
		//分配算法，传入参数为空闲区队列，和新作业的大小,返回新地址，如果分配失败返回-1
		for (int i = 0; i < freeBlockList.size(); i++) {
			if (freeBlockList[i].getSize() >= jobSize) {
				freeBlockList[i] = freeBlockList[i]
					.setAddress(
						freeBlockList[i].getAddress() + jobSize
					)
					.setSize(
						freeBlockList[i].getSize() - jobSize
					);
                int newAddress = freeBlockList[i].getAddress();
                if (freeBlockList[i].getSize() == 0) {
                    freeBlockList.erase(freeBlockList.begin() + i);
                }
				return newAddress;
			}
		}
		return -1;
	}
	int recycle(std::vector<Block>& freeBlockList, Block recycleBlock) {
		int index = 0;
		for (; index < freeBlockList.size(); index++) {
			if (freeBlockList[index].getAddress() < recycleBlock.getAddress()) {
				break;
			}
		}
		int preIndex = index - 1;
		int nextIndex = index;
		if (preIndex != -1) {
			if (freeBlockList[preIndex].getEndAddress() == recycleBlock.getAddress()) {
				freeBlockList[preIndex] = freeBlockList[preIndex]
                    .setSize(
                        freeBlockList[preIndex].getSize() + recycleBlock.getSize()
                    );
			}
            if (nextIndex != freeBlockList.size()) {
                if (freeBlockList[preIndex].getEndAddress() == freeBlockList[nextIndex].getAddress()) {
                    freeBlockList[preIndex] = freeBlockList[preIndex]
                        .setSize(
                            freeBlockList[preIndex].getSize() + freeBlockList[nextIndex].getSize()
                        );
                    freeBlockList.erase(freeBlockList.begin() + nextIndex);
                }
            }
		}
        if (nextIndex != freeBlockList.size()) {
            if (freeBlockList[nextIndex].getAddress() == recycleBlock.getEndAddress()) {
                freeBlockList[nextIndex] = freeBlockList[nextIndex]
                    .setAddress(
                        freeBlockList[nextIndex].getAddress() - recycleBlock.getSize()
                    )
                    .setSize(
                        freeBlockList[nextIndex].getSize() + recycleBlock.getSize()
                    );
            }
        }
        return 1;
	}
};