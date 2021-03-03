#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

typedef struct
{
    int32_t  target_pos ;
    int32_t  target_vel ;
    int16_t  target_tor ;
    int16_t  max_tor ;
    uint16_t control_word ;
    int32_t  vel_offset ;
    int16_t  tor_offset ;

    int32_t  actual_pos ;
    int32_t  actual_vel ;
    int16_t  actual_cur ;
    int16_t  actual_tor ;
    uint16_t status_word ;
    int8_t   mode_display ;
}param ;

class A
{
    public : 
    int b = 0;
    param P ; 
    void addition()
    {
        std::cout << "Hello from class A" << std::endl;
        b=b+2;
        std::cout << "b value is " << b << std::endl; 
    }

};
class C
{
public:
    pthread_t t, a;
    static A astatic ; 
    A x;

    void *hello(void)
    {
        std::cout << "Hello, world!" << std::endl;
        return 0;
    }

    void *GetValuesA(void)
    {
        this->x.addition();
        return 0;
    }
    static void *hello_helper(void *context)
    {
        return ((C *)context)->hello();
    }
    
    
    static void *helloAHelper(void *context)
    {
      return ((C *)context)->GetValuesA();
    }
    void startFunction(C c)
    {
        pthread_create(&this->t,NULL,&this->hello_helper,&c);
        pthread_create(&this->a,NULL,&this->helloAHelper, &c);
    }
};
A C::astatic = {};
int main (){
    A exp, data ;
    C test1,test2 ;
    test1.astatic.P.target_tor = 25 ; 
    test2.astatic.P.target_vel = 35;

    test1.astatic.addition() ;
    test2.astatic.addition() ;
    std::cout << test1.astatic.P.target_tor << std::endl ;
    std::cout << test2.astatic.P.target_tor << std::endl ;

    std::cout << test1.astatic.P.target_vel << std::endl ;
    std::cout << test2.astatic.P.target_vel << std::endl ;

return 0;
}