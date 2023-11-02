// DynamicPartitionAllocation.cpp: 定义应用程序的入口点。
//

#include "DynamicPartitionAllocation.h"

class DynamicPartitionAllocation
{
public:
    DynamicPartitionAllocation(PartitionAllocationInterface *partitionAllocationInterface) {
        this->partitionAllocationInterface = partitionAllocationInterface;
    }
    int allocate(int jobSize) {
        return partitionAllocationInterface->allocate(freeBlockList, jobSize);
    }
    int recycle(Block recycleBlock) {
        return partitionAllocationInterface->recycle(freeBlockList, recycleBlock);
    }
    void showFreeBlockList() {
        std::cout << "|" << "address" << "|" << "size" << "|" << std::endl;
        std::cout << "|" << "-------" << "|" << "-------" << "|" << std::endl;
        for (int i = 0; i < freeBlockList.size(); i++) {
            std::cout << "|" << freeBlockList[i].getAddress() << "|" << freeBlockList[i].getSize() << "|" << std::endl;
        }
        std::cout << std::endl;
    }
    std::vector<Block> freeBlockList;
    PartitionAllocationInterface *partitionAllocationInterface;
};
void test1(DynamicPartitionAllocation* dpa){
    dpa->partitionAllocationInterface->introduce();
    Block block1;
    block1.setAddress(100).setSize(10);
    Block block2;
    block2.setAddress(200).setSize(30);
    Block block3;
    block3.setAddress(300).setSize(15);
    int jobs[] = { 15,16,10};
    dpa->freeBlockList.push_back(block1);
    dpa->freeBlockList.push_back(block2);
    dpa->freeBlockList.push_back(block3);
    dpa->showFreeBlockList();
    std::cout<<std::endl;
    for (int i = 0; i < sizeof(jobs) / sizeof(int); i++) {
        int address = dpa->allocate(jobs[i]);
        std::cout<<"allocate job size:"<<jobs[i]<<std::endl;
        if (address == -1) {
            ;
        }
        else {
            dpa->showFreeBlockList();
        }
    }

}
void test2(DynamicPartitionAllocation* dpa){
    dpa->partitionAllocationInterface->introduce();
    Block block;
    block.setAddress(0).setSize(512);
    dpa->freeBlockList.push_back(block);
    dpa->showFreeBlockList();
    struct Job {
        int size;
        enum Type {
            req,
            release
        } type;
        int id;
    };
    Job job[]={
            {300,Job::req, 1},
            {100,Job::req, 2},
            {300,Job::release,1},
            {150,Job::req, 3},
            {30,Job::req, 4},
            {40,Job::req, 5},
            {60,Job::req, 6},
    };
    Block store_address[10];
    for (int i=0; i<sizeof(job)/sizeof(Job); i++) {
        if (job[i].type == Job::req) {
            std::cout << "分配作业 " << "大小为" << job[i].size << std::endl;
            int address = dpa->allocate(job[i].size);
            if (address == -1) {
                ;
            }
            else {
                store_address[job[i].id].setAddress(address).setSize(job[i].size);
                dpa->showFreeBlockList();
            }
        }
        else if (job[i].type == Job::release) {
            std::cout << "回收作业 "  << "大小为" << job[i].size << std::endl;
            int address = dpa->recycle(store_address[job[i].id]);
            if (address == -1) {
                ;
            }
            else {
                dpa->freeBlockList.push_back(store_address[job[i].id]);
                //内部排序
                std::sort(dpa->freeBlockList.begin(), dpa->freeBlockList.end(), [](Block a, Block b) {
                    return a.getAddress() < b.getAddress();
                });
                dpa->showFreeBlockList();
            }
        }
    }
}
void test3(DynamicPartitionAllocation *dpa)
{
    dpa->partitionAllocationInterface->introduce();
    Block block;
    block.setAddress(0).setSize(55);
    dpa->freeBlockList.push_back(block);
    dpa->showFreeBlockList();
    struct Job
    {
        int size;
        enum Type
        {
            req,
            release
        } type;
        int id;
    };
    Job job[] = {
        {15, Job::req, 1},
        {30, Job::req, 2},
        {15,Job::release,1},
        {8, Job::req, 3},
        {6, Job::req, 4},
    };
    Block store_address[10];
    for (int i = 0; i < sizeof(job) / sizeof(Job); i++)
    {
        if (job[i].type == Job::req)
        {
            std::cout << "分配作业 "
                      << "大小为" << job[i].size << std::endl;
            int address = dpa->allocate(job[i].size);
            if (address == -1)
            {
                ;
            }
            else
            {
                store_address[job[i].id].setAddress(address).setSize(job[i].size);
                dpa->showFreeBlockList();
            }
        }
        else if (job[i].type == Job::release)
        {
            std::cout << "回收作业 "
                      << "大小为" << job[i].size << std::endl;
            int address = dpa->recycle(store_address[job[i].id]);
            if (address == -1)
            {
                ;
            }
            else
            {
                dpa->freeBlockList.push_back(store_address[job[i].id]);
                // 内部排序
                std::sort(dpa->freeBlockList.begin(), dpa->freeBlockList.end(), [](Block a, Block b)
                          { return a.getAddress() < b.getAddress(); });
                dpa->showFreeBlockList();
            }
        }
    }
}

int main()
{
    //重定向输出
    // DynamicPartitionAllocation* dpa1 = new DynamicPartitionAllocation(new FirstFit());
    // test2(dpa1);
    DynamicPartitionAllocation *dpa2 = new DynamicPartitionAllocation(new BestFit());
    test3(dpa2);
	return 0;
}
