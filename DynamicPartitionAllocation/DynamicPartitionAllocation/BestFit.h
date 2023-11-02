#pragma once

#include "PartitionAllocationInterface.h"

class BestFit : public PartitionAllocationInterface {
public:
    ~BestFit() {
        fclose(stdout);
    }
    void introduce() {
        freopen("bestfit.md", "w", stdout);
        std::cout << "最佳适应法" << std::endl;
    }
    int allocate(std::vector<Block> &freeBlockList, int jobSize){
        //分配算法，传入参数为空闲区队列，和新作业的大小,返回新地址，如果分配失败返回-1
        int minIndex = -1;
        int minSize = 0x7fffffff;
        for (int i = 0; i < freeBlockList.size(); i++) {
            if (freeBlockList[i].getSize() >= jobSize) {
                if (freeBlockList[i].getSize() < minSize) {
                    minSize = freeBlockList[i].getSize();
                    minIndex = i;
                }
            }
        }
        if (minIndex == -1) {
            return -1;
        }
        int newAddress = freeBlockList[minIndex].getAddress();
        freeBlockList[minIndex] = freeBlockList[minIndex]
                .setAddress(
                        freeBlockList[minIndex].getAddress() + jobSize
                )
                .setSize(
                        freeBlockList[minIndex].getSize() - jobSize
                );
        if (freeBlockList[minIndex].getSize() == 0) {
            freeBlockList.erase(freeBlockList.begin() + minIndex);
        }
        return newAddress;
    }

    int recycle(std::vector<Block> &freeBlockList, Block recycleBlock)
    {
        int index = 0;
        for (; index < freeBlockList.size(); index++)
        {
            if (freeBlockList[index].getAddress() > recycleBlock.getAddress())
            {
                break;
            }
        }
        int preIndex = index - 1;
        int nextIndex = index;
        if (preIndex != -1)
        {
            if (freeBlockList[preIndex].getEndAddress() == recycleBlock.getAddress())
            {
                freeBlockList[preIndex] = freeBlockList[preIndex]
                                              .setSize(
                                                  freeBlockList[preIndex].getSize() + recycleBlock.getSize());
            }
            if (nextIndex != freeBlockList.size())
            {
                if (freeBlockList[preIndex].getEndAddress() == freeBlockList[nextIndex].getAddress())
                {
                    freeBlockList[preIndex] = freeBlockList[preIndex]
                                                  .setSize(
                                                      freeBlockList[preIndex].getSize() + freeBlockList[nextIndex].getSize());
                    freeBlockList.erase(freeBlockList.begin() + nextIndex);
                }
            }
            return 1;
        }
        else if (nextIndex != freeBlockList.size())
        {
            if (freeBlockList[nextIndex].getAddress() == recycleBlock.getEndAddress())
            {
                freeBlockList[nextIndex] = freeBlockList[nextIndex]
                                               .setAddress(
                                                   freeBlockList[nextIndex].getAddress() - recycleBlock.getSize())
                                               .setSize(
                                                   freeBlockList[nextIndex].getSize() + recycleBlock.getSize());
            }
            return 1;
        }
        return 0;
    }
};