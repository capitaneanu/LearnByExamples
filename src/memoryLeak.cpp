#include <iostream>
#include <memory>

using namespace std; 
class car 
{ 
    string name; 
    int num; 
  
    public: 
        car(){};
        car(string a, int n) 
        { 
            cout << "Constructor called" << endl; 
            this ->name = a; 
            this ->num = n; 
        } 
  
        void enter() 
        { 
            cin>>name; 
            cin>>num; 
        } 
  
        void display() 
        { 
            cout << "Name: " << name << endl; 
            cout << "Num: " << num << endl; 
        } 
}; 
  
int main() 
{ 
    // Using new keyword 
    car p[] = {};
    while(1){
        
        car*p = new car; 
    
    }
} 