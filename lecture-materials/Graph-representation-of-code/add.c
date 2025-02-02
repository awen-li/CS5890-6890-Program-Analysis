#include <stdio.h>

#define HW_MACRO "Hello, World!"

int main(int argc, char* argv[]) 
{
    int a = atoi (argv[1]);
    int b = atoi (argv[2]);

    return (a + b);
}

