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

struct file_contents
{
    char* buffer;
    int size;
};

/*
 *  parameters:
 *  fp: pointer to the source file
 *  buffer: memory pointer to the character array that will be filled with the file contents
 *  blob_factor: 1 keeping track of blob size to adjust buffer capacity dynamically
 *  blob: 500 default capacity of buffer
 *
 *  returns the number of characters read from the file contents after storing the contents
 *  to the memory location provided.
 * */
struct file_contents get_from(FILE* fp, char* buffer,  int blob_factor, int blob)
{
    //char * temp_buffer;
    //temp_buffer = malloc(500*sizeof(char));
    printf("inside file_reader/get_from\n");
    //temporary variable to store characters fetched from the file stream
    char ch;
    // array index in destination buffer
    int offset = 0;
    do
    {
        ch = fgetc(fp);
        //printf("%s\t", type_name(ch));
        //printf("read: %c\t", ch);
        buffer[offset] = ch;
        //printf("content: %c\n", buffer[offset]);
        // dynamic adjustment of buffer
        if (offset + 1 == blob*blob_factor)
        {
            printf("resizing buffer to %d units.\n", (blob_factor+1)*blob);
            buffer = realloc(buffer, ++blob_factor*blob*sizeof(char));
            printf("size of buffer : %d blob_factor %d\n", sizeof(buffer), blob_factor);
        }
        offset = offset + 1;
    } while (ch != EOF);
    printf("\n%d\n", offset);
    // to count the number of characters read
    int c_offset = 0;
    while (c_offset < offset)
    {
        printf("%c", buffer[c_offset]);
        c_offset = c_offset + 1;
    }
    printf("\n");
    printf("%s\n", buffer == &buffer[0] ? "true" : "false");
    printf("exiting file_reader/get_from\n");

    struct file_contents fc;
    fc.buffer = buffer;
    fc.size = offset;


    return fc;
}

/*
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
*/

