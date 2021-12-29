#include <stdio.h>
#include"Backend.h"

int main()
{
    printf("Hello world!");

    BackendInit();

    BackendRun();

    BackendExit();

    return 0;
}