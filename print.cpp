
#include "elmoObjectDictionary.h"
#include "od.h"
#include <iostream>
int main()
{
    OD myObjectDictionary; 
    printf("Analog input mapped value is 0x%x \n",myObjectDictionary.\
    TxPDO1A00h.positionActualValue);

    return 0;
}