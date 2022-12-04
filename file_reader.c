//
// Created by charan on 11/30/22.
//

#include <stdio.h>
#include <stdlib.h>
#define BLOB 500

#define type_name(expr) \
    (_Generic((expr), \
              char: "char", unsigned char: "unsigned char", signed char: "signed char", \
              short: "short", unsigned short: "unsigned short", \
              int: "int", unsigned int: "unsigned int", \
              long: "long", unsigned long: "unsigned long", \
              long long: "long long", unsigned long long: "unsigned long long", \
              float: "float", \
              double: "double", \
              long double: "long double", \
              void*: "void*", \
              default: "?"))


int main()
{
    char input_path[] = "login_page.html";
    int blob_factor = 1;
    printf("initialize %d units\n", blob_factor*BLOB);
    char *buffer = (char *) calloc(BLOB*blob_factor, sizeof(char));
    if (buffer == NULL)
    {
        printf("memory for buffer not successfully instantiated!\n");
        exit(-1);
    }
    else
        printf("size of buffer: %d\n", sizeof(buffer));
    printf("%s\n", input_path);
    FILE *fp = fopen(input_path, "r");
    if (fp == NULL)
    {
        printf("Could not open %s\n", input_path);
        exit(-1);
    }
    char ch;
    int offset = 0;
    do
    {
        ch = fgetc(fp);
        printf("%s\t", type_name(ch));
        printf("read: %c\t", ch);
        buffer[offset] = ch;
        printf("content: %c\n", buffer[offset]);
        if (offset + 1 == BLOB)
        {
            printf("resizing buffer to %d units.\n", (blob_factor+1)*BLOB);
            buffer = realloc(buffer, ++blob_factor*BLOB);
            printf("size of buffer: %d\n", sizeof(buffer));
        }
        offset = offset + 1;
    } while (ch != EOF);
    printf("\n%d\n", offset);
    int c_offset = 0;
    while (c_offset < offset)
    {
        printf("%c", buffer[c_offset]);
        c_offset = c_offset + 1;
    }
    printf("\n");
    fclose(fp);
    free(buffer);
    printf("freeing buffer: %d\n", sizeof(buffer));
    return 0;
}

