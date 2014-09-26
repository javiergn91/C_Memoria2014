#include "utils.h"

unsigned long long Utils::GetDecimalRepresentation(vector<int>* binaryRepresentation)
{
    unsigned long long multBy = 1;
    unsigned long long sum = 0;

    for(int i = 0; i < (int)binaryRepresentation->size(); i++)
    {
        int currDig = binaryRepresentation->at(i);
        sum += (multBy * currDig);
        multBy *= 2;
    }

    return sum;
}

int Utils::GetBytesRequired(int n)
{
    int wordSize = 32;
    int bytesRequired = n / wordSize;

    if(n % wordSize != 0)
    {
        bytesRequired++;
    }

    return bytesRequired;
}
