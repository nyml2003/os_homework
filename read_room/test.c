#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define record_max_num 100
#define reader_max_num 10000
int record_num = 0, reader_num = 0;
typedef struct
{
    char name[record_max_num][20];
    sem_t sem;
} protected_records;
protected_records records;
char name[reader_max_num][20];
pthread_t reader_thread[reader_max_num];
void print_record()
{
    printf("current readers:\n");
    printf("%4s%20s\n", "id", "name");
    for (int i = 0; i < record_num; i++)
    {
        printf("%4d%20s\n", i, records.name[i]);
    }
    printf("\n");
}
void *reader(void *arg)
{
    char *name = (char *)arg;
    int id;
    while (1)
    {
        sem_wait(&records.sem);
        printf("%s try to find a seat\n", name);
        id = -1;
        for (int i = 0; i < record_num; i++)
        {
            if (records.name[i][0] == '\0')
            {
                id = i;
                break;
            }
        }
        if (id == -1)
        {
            printf("%s can't find a seat\n\n", name);
            sem_post(&records.sem);
        }
        else
        {
            printf("%s sit at %d\n\n", name, id);
            sprintf(records.name[id], "%s", name);
            print_record();
            sem_post(&records.sem);
            break;
        }
        usleep(20);
    }
    usleep(20);
    sem_wait(&records.sem);
    printf("%s leave %d\n\n", name, id);
    records.name[id][0] = '\0';
    print_record();
    sem_post(&records.sem);
}

int main(int argc, char **argv)
{
    freopen("test.out", "w", stdout);
    if (argc != 3)
    {
        printf("usage: ./test record_num reader_num\n");
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        record_num = atoi(argv[1]);
        if (record_num > record_max_num)
        {
            printf("record number should be less than %d\n", record_max_num);
            return 0;
        }
        reader_num = atoi(argv[2]);
        if (reader_num > reader_max_num)
        {
            printf("reader number should be less than %d\n", reader_max_num);
            return 0;
        }
    }
    printf("record number: %d\n", record_num);
    printf("reader number: %d\n", reader_num);
    srand(time(NULL));
    sem_init(&records.sem, 0, 1);
    for (int i = 0; i < record_num; i++)
    {
        records.name[i][0] = '\0';
    }
    for (int i = 0; i < reader_num; i++)
    {
        char tname[20];
        sprintf(tname, "reader %d", i);
        strcpy(name[i], tname);
    }
    for (int i = 0; i < reader_num; i++)
    {
        pthread_create(&reader_thread[i], NULL, reader, (void *)name[i]);
    }
    for (int i = 0; i < reader_num; i++)
    {
        pthread_join(reader_thread[i], NULL);
    }
    sem_destroy(&records.sem);

    return 0;
}