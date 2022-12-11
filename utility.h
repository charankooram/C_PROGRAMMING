//
// Created by charan on 12/10/22.
//
#include <stdio.h>
#include <stdlib.h>

int number_of_characters(int input)
{
    if (input == 0)
    {
        return 0;
    }
    int answer = 1;
    while(input/10 != 0)
    {
        ++answer;
        input = input/10;
    }
    return answer;
}

char convert_to_char(int singlular_input)
{
    if(singlular_input == 0) return '0';
    if(singlular_input == 1) return '1';
    if(singlular_input == 2) return '2';
    if(singlular_input == 3) return '3';
    if(singlular_input == 4) return '4';
    if(singlular_input == 5) return '5';
    if(singlular_input == 6) return '6';
    if(singlular_input == 7) return '7';
    if(singlular_input == 8) return '8';
    if(singlular_input == 9) return '9';
    if(singlular_input < 0 || singlular_input > 9)
    {
        printf("invalid input\n");
        exit(-1);
    }

}

char* convert_to_string(int input){
    int count = number_of_characters(input);
    char *str = malloc(count*sizeof(char));
    int index = count-1;
    while(input/10 != 0)
    {
        if(index < 0)
        {
            printf("something went wrong!\n");
            exit(-1);
        }
        else
        {
            char c = convert_to_char(input%10);
            str[index--] = c;
        }
        input = input/10;
    }
    str[index] = convert_to_char(input%10);
    return str;
}

int get_number_of_characters_from_header_array(char **arr, int size)
{
    int sum = 0;
    for(int i=0;i<size;i++)
    {
        char *c = arr[i];
        int count = 0;
        while(c[count] != '\0')
        {
            count++;
        }
        sum += count;
    }
    return sum;
}

/*
int main()
{
    int size = 100;
    char* http_headers[] = {
            "HTTP/1.1",
            "200 OK",
            "Date:",
            "Mon, 27 Jul 2009 12:28:53 GMT",
            "Server:",
            "Apache/2.2.14 (Win32)",
            "Last-Modified:",
            "Wed, 22 Jul 2009 19:15:56 GMT",
            "Content-Length:",
            "98",
            "Content-Type:",
            "text/html"
    };
    char * second[] = {
            "HTTP/1.1",
            "200 OK"
    };
    //sprintf(http_headers[9],"%d", size);
    char *solution = convert_to_string(size);
    printf("%s\n", solution);
    http_headers[9] = solution;
    int p=0;
    while(p<12)
    {
        printf("%s %s\n", http_headers[p], http_headers[p+1]);
        p+=2;
    }

    printf("count %d\n", get_number_of_characters_from_header_array(second, 2));
}
*/