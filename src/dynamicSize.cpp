#include <iostream>
#include <memory>

using namespace std; 
int main() 
{ 
    // Using new keyword 
    int p[] = {};
    cout << sizeof(p) << endl;
    for(int i = 0 ; i < 100 ; i++){
        p[i] = i ;
    }

    for(int i = 0 ; i < 100 ; i++)
    {
        cout << p[i]  << std::endl;
    }
    
    cout << sizeof(p) << endl;

    return 0 ;
} 