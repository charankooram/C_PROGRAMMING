//
// Created by charan on 11/30/22.
//

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char input_path[] = "login_page.html";
    char ch1 = 'c';
    char *buffer = &ch1;
    *buffer = 'd';
    printf("%c\n", *buffer);
    printf("%s\n", input_path);
    FILE *fp = fopen(input_path, "r");
    if (fp == NULL)
    {
        printf("Could not open %s", input_path);
        exit(-1);
    }
    char ch;
    int offset = 0;
    do
    {
        ch = fgetc(fp);
        printf("read: %c\t", ch);
        char* p;
        p = (buffer + offset);
        printf("address: %d\n", p);
        //*p = ch; // can pointers be negative integers ??
        //if (*p != ch)
        //{
        //    printf("Something went wrong! %c != %c\t", *p, ch);
        //    do
        //    {
        //        if (*p == ch)
        //        {
        //            break;
        //        }
        //        *p = ch;
        //        printf("retried copying: %c with %c\t", *p, ch);
        //    } while (*p != ch);
        //}
        //else
        //    printf("content: %c\n", *p);
        offset = offset + 1;
    } while (ch != EOF);
    printf("\n%d\n", offset);
    //printf("%s\n", *buffer);
    //fclose(fp);
    printf("%d\n", buffer);
    printf("%c\n", *buffer);
    //printf("%d\n", buffer);
    //printf("%d\n", *buffer);

    //printf("%s\n", *buffer);
    return 0;
}

