#include<iostream>
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
    if ( fork() == 0 ) {
        std::cout << "Child Process" << std::endl;
        execlp("cat","cat","cloneProcess.cpp", NULL);
        std::cout << "Let's see what's going on " << std::endl;
    }
    
    return 0;
}
