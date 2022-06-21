#ifndef _HASHTABLE_
#define _HASHTABLE_

#include"STL.h"

#define sizeOfHash ((int)15)
#define PrimeNumber ((int)151)
typedef char stringHashTable[sizeOfHash] ;

int H_HashFunction (stringHashTable node) ;
void H_ProbeFunction ( stringHashTable node , int *StopPosition , int *FirstInsert ) ;
void H_InsertFunction(stringHashTable node) ;
int H_FindFunction(stringHashTable node) ;

extern stringHashTable SaveIO[PrimeNumber] ;

#endif  // _HASHTABLE_