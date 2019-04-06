//Program to read the values of clock
//if the variables are declared as global then they are not displayed in the variable window while debugging

#include "driverlib.h"

int main(void) {
    uint32_t myMCLK=0;
     uint32_t mySMCLK=0;
     uint32_t myACLK=0;
    WDT_A_hold(WDT_A_BASE);
while(1)
{

    myMCLK = CS_getMCLK ();
    mySMCLK= CS_getSMCLK();
    myACLK= CS_getACLK();
}
    return (0);
}
