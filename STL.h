#ifndef _STL_H_
#define _STL_H_
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

FILE *ReadTextFile(int *RowOfFile); // Tạo và đọc địa chỉ file TXT đã xóa comment
struct Link
{
    char *data;
    struct Link *next;
    struct Link *prev;

}; // Cấu trúc 1 phần tử
typedef struct Link LinkList;
LinkList *First, *Last, *FirstFinal, *LastFinal;     // First và Last dùng cho  TransferToList ()   ; FirstFinal , LastFinal  là chuỗi cuối cùng cần tìm
void TransferToList(FILE *pFileFinal, int RowOfFile); // Chuyển về List 1 gồm tập hợp các ký tự có loại bỏ các ký tự ko cần thiết
void FinalList(void);                          // Gom các ký tự câu lệnh (biến) vào 1 data , bổ sung H vào biến thường đóng , bổ xung "sl" nếu là suòn lên hoặc "sx" nếu là sườn xuống
void SplitBranchesWithFirstFinalPointer(void);                  // Tách nhánh
void FinalTextFile(void);
char *StrAllocAndAppend(const char *str1, const char *str2) ;















//-----------------------------------FinalList--And---SplitBranchesWithFirstFinalPointer-------------------------------------------------------------------------------------------//

void InsertEdge (LinkList *pMainOfFirstFinal , char *edge)
{
                LinkList *temp1, *temp2, *temp3;
                temp1 = pMainOfFirstFinal->prev;
                temp2 = temp1->prev; // temp1 = i0.0 => temp2 = ld
                char *TempArray2 = StrAllocAndAppend(temp1->data, edge);
                int SizeOfTempArray2 = strlen(TempArray2);
                temp3 = (LinkList *)malloc(sizeof(LinkList));
                // Thay r3 vào r1
                temp3->next = temp1->next;
                temp3->prev = temp1->prev;
                temp2->next = temp3;
                temp3->data = (char *)calloc(SizeOfTempArray2, sizeof(char));
                for (int i = 0; i <= SizeOfTempArray2; i++)
                {
                    (temp3->data)[i] = TempArray2[i];
                }
                free(temp1);
}

void CreateTheFirstPointerOfList(LinkList *(*pMain) ,LinkList *(*F) , LinkList *(*L) )
{
                 (*pMain)->prev = NULL;
                *F = *pMain;
                *L = *pMain;
}

void CreateTheElementOfList(LinkList *(*pMain) , LinkList *(*L) )
{               LinkList *pTemp ;
                 pTemp = *L;
                pTemp->next = *pMain;
                (*pMain)->prev = pTemp;
                *L = *pMain;
}

void RecreateTheFirstElement(LinkList *(*pNew) , LinkList *(*pOld))
{
           (*pNew)->next = *pOld;
            (*pOld)->prev = *pNew;
            (*pNew)->prev = NULL;
}

void InsertNextElement(LinkList *(*CurrentElement), LinkList *(*Insert) , LinkList *(*NextElement) )
{
                        (*CurrentElement)->next = *Insert;
                        (*Insert)->prev = *CurrentElement;
                        (*Insert)->next = *NextElement;
                        (*NextElement)->prev = *Insert;
}
void InsertPrevElement (LinkList *(*CurrentElement), LinkList *(*Insert)  )
{                   
                    LinkList *PreElement ;
                    PreElement =  (*CurrentElement)->prev ;
                    PreElement->next = *Insert;
                    (*Insert)->prev = PreElement;
                    (*Insert)->next = *CurrentElement;
                    (*CurrentElement)->prev = *Insert;
}











//--------------------------------------------FinalTextFile----------------------------------------------------------------------------------------------------------------------//

char *StrAllocAndAppend(const char *str1, const char *str2) // chèn tại cuối chuỗi 
{

    /*
    Chèn chuỗi trong C

    Tham số
        str1:      Chuỗi ban đầu (không chấp nhận Null)
        pos:       Vị trí chèn (tính từ đầu chuỗi str1)
        str2:      Chuỗi chèn vào (không chấp nhận Null pointer)
    Giá trị trả về
        Con trỏ mảng chứa kết quả. Nếu thực thi thất bại thì trả về Null
*/
    size_t pos;
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    pos = len1;
    // Kiểm tra vị trí chèn có nằm trong chuỗi str1 không

    size_t result_size = len1 + len2 + 1;
    char *result = malloc(sizeof(char) * result_size);
    if (result == NULL)
    {
        return NULL;
    }

    // Copy phạm vi từ đầu chuỗi ban đầu tới trước vị trí cần chèn vào chuỗi kết quả
    memcpy(result, str1, pos);

    // Copy chuỗi cần chèn vào cuối kết quả
    memcpy(&result[pos], str2, len2);

    // Copy phạm vi từ vị trí cần chèn tới cuối chuỗi ban đầu vào chuỗi kết quả
    // Thêm ký tự kết thúc chuỗi `\0` vào kết quả
    strcpy(&result[pos + len2], &str1[pos]);

    return result;
}
char* StrAllocAndInsert(const char* str1, size_t pos, const char* str2) // Chèn tại vị trí bất kỳ
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // Kiểm tra vị trí chèn có nằm trong chuỗi str1 không
    assert(len1 >= pos);


    size_t result_size = len1 + len2 + 1;
    char* result = malloc(sizeof(char) * result_size);
    if (result == NULL) {
        return NULL;
    }

    // Copy phạm vi từ đầu chuỗi ban đầu tới trước vị trí cần chèn vào chuỗi kết quả
    memcpy(result, str1, pos);

    // Copy chuỗi cần chèn vào cuối kết quả
    memcpy(&result[pos], str2, len2);

    // Copy phạm vi từ vị trí cần chèn tới cuối chuỗi ban đầu vào chuỗi kết quả
    // Thêm ký tự kết thúc chuỗi `\0` vào kết quả
    strcpy(&result[pos + len2], &str1[pos]); 

    return result;
}
char* AddParenthesesIfMissing(char *arr )
{   

    char *insert_str_pre = "(";
    char *insert_str_next = ")";
    int size_arr = strlen(arr);
            int left = 0;
            int right = 0;
            for (int i = 0; i < size_arr; i++)
            {
                if (arr[i] == '(')
                {
                    left++;
                }
                if (arr[i] == ')')
                {
                    right++;
                }
            }
            while (left != right)
            {
                        if (left > right)
            {
                arr = StrAllocAndAppend(arr, insert_str_next);
                right ++ ;
            }
            else if (left < right)
            {
                arr = StrAllocAndAppend(insert_str_pre, arr);
                left ++ ;
            }
            }
        return arr ;
}

void CheckCountQuestionMark (int *CountQuestionMark , int *CheckBigBranch , char *OutString , char *OutCheckBigBranch)
{   
    char *InsertQuestionMark ="?" ;
    if (*CountQuestionMark > 1)
            {
                OutString = AddParenthesesIfMissing(OutString);
                int SizeToken = 0;
                int SizeTokenTemp = 0;
                int CountTemp = 0;
                int size_O = strlen(OutString);
                char *OutStringTemp = "";
                OutStringTemp = StrAllocAndAppend(OutStringTemp, OutString); // Lưu tạm thời OUT vào OUT_T
                char *token = strtok(OutString, "?");           /// (...)?((.....)?(......))
                // thêm 1 vòng while để biết timer , couter
                while (token != NULL)
                {
                    CountTemp++;
                    SizeToken = strlen(token); // tính size token cuối cùng
                    if (SizeTokenTemp == SizeToken)
                    {
                        break;
                    }

                    SizeTokenTemp = SizeToken;

                    token = strtok(NULL, "?");
                }
                char *token1 = strtok(OutStringTemp, "?");
                for (int i = 0; i < CountTemp - 2; i++)
                {
                    OutCheckBigBranch = StrAllocAndAppend(OutCheckBigBranch, token1); /// (...)?
                    OutCheckBigBranch = StrAllocAndAppend(OutCheckBigBranch, InsertQuestionMark);
                    token1 = strtok(NULL, "?");
                }
                OutString = "";
                OutString = StrAllocAndAppend(OutString, token1);
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                token1 = strtok(NULL, "?");
                OutString = StrAllocAndAppend(OutString, token1);
                *CheckBigBranch = 1;
                *CountQuestionMark = 1;
            }
}

void CheckNOT(LinkList *(*pMain) ,LinkList *pNext, LinkList *pNext1 , char *(*OutString),char *Insert)
{
      char *InsertClosingBracket = ")";
            if (strcmp(pNext1->data, "NOT") == 0)
            {

                *OutString = StrAllocAndAppend(*OutString, Insert);
                *OutString = StrAllocAndAppend(*OutString, pNext->data);
                *OutString = StrAllocAndAppend(*OutString, InsertClosingBracket);
                *pMain = pNext1->next;
            }
            else if (strcmp(pNext1->data, "NOT") != 0)
            {
                *OutString = StrAllocAndAppend(*OutString, Insert);
                *OutString = StrAllocAndAppend(*OutString, pNext->data);
                *pMain = pNext1;
            }
}

void AddPlusToOutString(LinkList *(*pMain), char *(*OutString) , int *CountQuestionMark , char* Insert )
{
            char *InsertOpeningBracket = "("; 
            char *InsertClosingBracket = ")";
            LinkList *pNext ;
           *OutString = StrAllocAndAppend(InsertOpeningBracket, *OutString); // Mở ngoặc cả cụm ALD này
            pNext = (*pMain)->next;
            if (strncmp(pNext->data, "!", 1) == 0)
            {
                char *arr_temp = "!";
                *OutString = StrAllocAndAppend(arr_temp, *OutString);
                *pMain = (*pMain)->next;
            }
            int size_of_arr = strlen(*OutString);

            char *OUTtemp, *OUTtemp1;
            OUTtemp = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUTtemp, *OutString);
            char *token = strtok(OUTtemp, "?");
            OUTtemp = StrAllocAndAppend(OUTtemp, Insert);
            (*CountQuestionMark)-- ;  // Thay đấu "?" bằng dấu "+"
            token = strtok(NULL, " ");
            OUTtemp1 = StrAllocAndAppend(OUTtemp, token);
            free(*OutString);
            size_of_arr = strlen(OUTtemp1);
            *OutString = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(*OutString, OUTtemp1);
            *OutString = StrAllocAndAppend(*OutString, InsertClosingBracket); // Đóng ngoặc cụm ALD này
            free(OUTtemp1);
            *pMain = (*pMain)->next;
}


#endif