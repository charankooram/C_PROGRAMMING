//
// Created by charan on 12/4/22.
//
#include <stdlib.h>
#include <stdio.h>

char *create_character_block(int num, int size_of_cell)
{
    char *ptr = (char *) malloc(num*size_of_cell);
    return ptr;
}

char *recreate_character_block(char *ptr, int new_size)
{
    ptr = realloc(ptr, new_size);
    return ptr;
}