#include "hashtable.h"

int H_HashFunction(stringHashTable node)
{
    int sumChar, sizeOfNode;
    sizeOfNode = strlen(node);
    sumChar = 0;
    for (int i = 0; i < sizeOfNode; i++)
        sumChar += (int)node[i];
    return  (sumChar % PrimeNumber);
}
void H_ProbeFunction(stringHashTable node, int *StopPosition, int *FirstInsert)
{
    int tempStopPosition, tempFirstInsert;
    int check = H_HashFunction(node);
    tempFirstInsert = check;
    if ((strcmp(SaveIO[check], node) == 0) || strcmp(SaveIO[check], "vacant") == 0)
        tempStopPosition = check;
    else
    {
        tempStopPosition = (check + 1) % PrimeNumber;

        while ((tempStopPosition != check) && (strcmp(SaveIO[tempStopPosition], node) != 0) && strcmp(SaveIO[tempStopPosition], "vacant") != 0)
        {
            if ((strcmp(SaveIO[tempStopPosition], "delete") == 0) && (strcmp(SaveIO[tempFirstInsert], "delete") != 0))
                tempFirstInsert = tempStopPosition;
            tempStopPosition = (tempStopPosition + 1) % PrimeNumber;
        }
        if (strcmp(SaveIO[tempStopPosition], "vacant") == 0 && (strcmp(SaveIO[tempFirstInsert], "delete") != 0))
            tempFirstInsert = tempStopPosition;
    }
    *StopPosition = tempStopPosition;
    *FirstInsert = tempFirstInsert;
}

void H_InsertFunction(stringHashTable node)
{
    int StopPosition, FirstInsert;
    H_ProbeFunction(node, &StopPosition, &FirstInsert);
    if (strcmp(SaveIO[StopPosition], node) != 0)
    {
        if (strcmp(SaveIO[FirstInsert], "vacant") == 0 || (strcmp(SaveIO[FirstInsert], "delete") == 0))
           {
               strcpy(SaveIO[FirstInsert], node);
           } 
        else
        {
            assert(1); // Bảng bị đầy 
        }
    }
}
int H_FindFunction(stringHashTable node)
{
    int StopPosition, FirstInsert;
    H_ProbeFunction(node, &StopPosition, &FirstInsert);
    if (strcmp(SaveIO[StopPosition], node) == 0)
    {
        return StopPosition;
    }
    return -1;
}
