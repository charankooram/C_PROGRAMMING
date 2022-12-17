//
// Created by charan on 12/4/22.
//
#include "DynamicMemoryAllocationTest.h"
#include "file_reader.h"
#include <unistd.h>

int test_scenario_1()
{
    char *buffer = malloc(500*sizeof(char));
    printf("created buffer with 500 units. \n");
    FILE *fp = fopen("login_page.html", "r");
    struct file_contents fc = get_from(fp, buffer, 1, 500);
    int another_count = 0;
    while (another_count < fc.size) {
        printf("%c", fc.buffer[another_count++]);
    }
    printf("\n");


    free(fc.buffer);
    fc.buffer = NULL;
    fc.size = 0;
    printf("clearing buffer\n");

    printf("testing after clearing buffer\n");
    int another_count2 = 0;
    while (another_count2 < fc.size) {
        printf("%c", fc.buffer[another_count2++]);
    }
    printf("\n");
}

int test_scenario_2()
{
    char *buffer, *buffer1;
    buffer = malloc(10*sizeof(char));
    buffer1 = buffer;
    printf("allocated some memory to buffer\n");
    buffer[0] = 'h';buffer[1]='i';
    for(int i=0;i<10;i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
    printf("assigned string to buffer\n");

    for(int i=0; i<10; i++)
    {
        printf("%c", buffer1[i]);
    }
    printf("\n");
    printf("test assignment to buffer1\n");
    free(buffer);
    printf("freeing buffer\n");
    for(int i=0; i<10; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
    printf("printed after freeing\n");
    free(buffer1);
    printf("Should you free buffer1 too in this scenario ? Freeing buffer1\n");
    for(int i=0;i<10;i++)
    {
        printf("%c", buffer1[i]);
    }
    printf("\n");
    printf("printed buffer1 after freeing\n");
}

int main()
{
    test_scenario_1();
}