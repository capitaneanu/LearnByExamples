#include <iostream> 
#define MAX_NUM 5
class Test {
    public:
    Test()
    {
         counter_ = counter;
         counter++;
         if(counter == MAX_NUM)
         {
             doThis();
         }
    };
    ~Test()
    {        
    }
    int doThis()
    {
        std::cout << MAX_NUM << " is reached" << std::endl;
        return 0 ;
    }
    static int counter;
    int counter_;
};

int Test::counter = 0 ;
int main()
{
    Test tryIt[5] ; 
    std::cout << "Something is going on"<< std::endl ;
    std::cout << tryIt[4].counter_ << std::endl;
}