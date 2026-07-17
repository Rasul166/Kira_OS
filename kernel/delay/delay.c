#include<tick.h>
void delayms(int no_of_ms)
{
    unsigned int start = ticks;
    while (ticks - start < no_of_ms)
    {
    }
}
