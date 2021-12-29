#include <stdio.h>
#include"Backend.h"

int main(int argc, char* args[])
{
    BackendInit();

    //Read rom from command line, load it

    BackendRun();

    BackendExit();

    return 0;
}