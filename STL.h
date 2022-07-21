#pragma once
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"

struct Link
{
    char *data;
    struct Link *next;
    struct Link *prev;
}; // Cấu trúc 1 phần tử
typedef struct Link LinkList;
extern LinkList *First;
extern LinkList *Last;
extern LinkList *FirstFinal;
extern LinkList *LastFinal;

void CreatFileNoComment(int *RowOfFile); // Tạo và đọc địa chỉ file TXT đã xóa comment
void InitSaveDataIO(void);
void CreatList(int RowOfFile); // Chuyển về List 1 gồm tập hợp các ký tự có loại bỏ các ký tự ko cần thiết
void EditList(void);           // Gom các ký tự câu lệnh (biến) vào 1 data , bổ sung H vào biến thường đóng , bổ xung "sl" nếu là suòn lên hoặc "sx" nếu là sườn xuống
void SaveDataIO(void);
void SplitBranch(void); // Tách nhánh
void InsertListToFileData(void);

//-----------------------------------EditList--And---SplitBranch-------------------------------------------------------------------------------------------//

void InsertEdge(LinkList *pMainOfFirstFinal, const char *const edge);
void CreateTheFirstPointerOfList(LinkList *(*pMain), LinkList *(*F), LinkList *(*L));
void CreateTheElementOfList(LinkList *(*pMain), LinkList *(*L));
void CreateTheElementOfList(LinkList *(*pMain), LinkList *(*L));
void RecreateTheFirstElement(LinkList *(*pNew), LinkList *(*pOld));
void InsertNextElement(LinkList *(*CurrentElement), LinkList *(*Insert), LinkList *(*NextElement));
void InsertPrevElement(LinkList *(*CurrentElement), LinkList *(*Insert));

//-----------------------------------InsertListToFileData-------------------------------------------------------------------------------------------//

char *StrAllocAndAppend(const char *str1, const char *str2);             // chèn tại cuối chuỗi
char *StrAllocAndInsert(const char *str1, size_t pos, const char *str2); // Chèn tại vị trí bất kỳ
char *AddParenthesesIfMissing(char *arr);
void CheckCountQuestionMark(int *CountQuestionMark, int *CheckBigBranch, char *(*OutString), char *(*OutCheckBigBranch));
void CheckNOT(LinkList *(*pMain), LinkList *pNext, LinkList *pNext1, char *(*OutString), char *Insert);
void AddPlusToOutString(LinkList *(*pMain), char *(*OutString), int *CountQuestionMark, char *Insert);
void InsertTimer(LinkList *pMain, int *CountTimer, FILE *pFileTimer);
void SetupCounterUpOrDown(LinkList *(*pMain), char *OutString, FILE *pFile);
void SetupCounterUpDown(LinkList *(*pMain), char *OutString, FILE *pFile);
char *CheckQuestionMask(int CountQuestionMark, char *OutString);
void DeleteLinkList(LinkList *(*pMain), LinkList *(*pPREV), LinkList *(*pNEXT));
void InsertMov(LinkList *(*pMain), char *OutString, int CountQuestionMark, FILE *pFile, int checkMov);
int getNumber(char *p, int len);

#define MOVB_CHECK 0
#define MOVW_CHECK 1
#define MOVDW_CHECK 2

//------------------------------------------------- define memory region --------------------------------------------
// Lựa chọn vùng nhớ
#define I_MEM 0
#define Q_MEM 1
#define M_MEM 2

// Chọn số lượng biến cho vùng nhớ (kiểu byte)
#define SUM_I 3
#define SUM_Q 3
#define SUM_M 3

//----------------------------------------------------------------FileDefineData----------------------------------------
void FileDefineData(void);

void DefineRegionMemory(FILE *pFile, int memoryRegion, int sumOfmem);
void DefineIO(FILE *pFile, int memoryRegion, int sumOfmem);

//-------------------------------------------------------------FileData-------------------------------------------------
void FileData(void);

void readInputPin(FILE *pFile, int sumOfmem);
void writeOutputPin(FILE *pFile, int sumOfmem);
void AddTimerFuntion(void);