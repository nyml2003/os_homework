// DynamicPartitionAllocation.cpp: 定义应用程序的入口点。
//

#include "DynamicPartitionAllocation.h"

using namespace std;
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
        for (int i = 0; i < freeBlockList.size(); i++) {
            cout << "address:" << freeBlockList[i].getAddress() << " size:" << freeBlockList[i].getSize() << endl;
        }
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
    cout<<endl;
    for (int i = 0; i < sizeof(jobs) / sizeof(int); i++) {
        int address = dpa->allocate(jobs[i]);
        cout<<"allocate job size:"<<jobs[i]<<endl;
        if (address == -1) {
            cout << "allocate failed" << endl;
        }
        else {
            cout << "allocate success address:" << address << endl;
            cout << "after allocate" << endl;
            dpa->showFreeBlockList();
            cout<<endl;
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
        int address;
    };
    Job job[]={
            {300,Job::req, -1},
            {100,Job::req, -1},
            {300,Job::release,0},
            {150,Job::req, -1},
            {30,Job::req, -1},
            {40,Job::req, -1},
            {60,Job::req, -1},
    };
    for (int i=0; i<sizeof(job)/sizeof(Job); i++) {
        if (job[i].type == Job::req) {
            int address = dpa->allocate(job[i].size);
            cout<<"allocate job size:"<<job[i].size<<endl;
            if (address == -1) {
                cout << "allocate failed" << endl;
            }
            else {
                cout << "allocate success address:" << address << endl;
                cout << "after allocate" << endl;
                dpa->showFreeBlockList();
                cout<<endl;
            }
        }
        else if (job[i].type == Job::release) {
            Block block;
            block.setAddress(job[i].size).setSize(0);

            int address = dpa->recycle(block);
            cout<<"recycle job address:"<<job[i].size<<endl;
            if (address == -1) {
                cout << "recycle failed" << endl;
            }
            else {
                cout << "recycle success address:" << address << endl;
                cout << "after recycle" << endl;
                dpa->showFreeBlockList();
                cout<<endl;
            }
        }
    }
}
int main()
{
    DynamicPartitionAllocation* dpa = new DynamicPartitionAllocation(new BestFit());
    test2(dpa);
    dpa = new DynamicPartitionAllocation(new FirstFit());
    cout<<"-------------------------"<<endl;
    test2(dpa);
	return 0;
}
