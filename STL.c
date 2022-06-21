#include "STL.h"

//-----------------------------------FinalList--And---SplitBranchesWithFirstFinalPointer-------------------------------------------------------------------------------------------//

void InsertEdge(LinkList *pMainOfFirstFinal, const char *const edge)
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

void CreateTheFirstPointerOfList(LinkList *(*pMain), LinkList *(*F), LinkList *(*L))
{
    (*pMain)->prev = NULL;
    *F = *pMain;
    *L = *pMain;
}

void CreateTheElementOfList(LinkList *(*pMain), LinkList *(*L))
{
    LinkList *pTemp;
    pTemp = *L;
    pTemp->next = *pMain;
    (*pMain)->prev = pTemp;
    *L = *pMain;
}

void RecreateTheFirstElement(LinkList *(*pNew), LinkList *(*pOld))
{
    (*pNew)->next = *pOld;
    (*pOld)->prev = *pNew;
    (*pNew)->prev = NULL;
}

void InsertNextElement(LinkList *(*CurrentElement), LinkList *(*Insert), LinkList *(*NextElement))
{
    (*CurrentElement)->next = *Insert;
    (*Insert)->prev = *CurrentElement;
    (*Insert)->next = *NextElement;
    (*NextElement)->prev = *Insert;
}
void InsertPrevElement(LinkList *(*CurrentElement), LinkList *(*Insert))
{
    LinkList *PreElement;
    PreElement = (*CurrentElement)->prev;
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
    char *result = (char *)malloc(sizeof(char) * result_size);
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
char *StrAllocAndInsert(const char *str1, size_t pos, const char *str2) // Chèn tại vị trí bất kỳ
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // Kiểm tra vị trí chèn có nằm trong chuỗi str1 không
    assert(len1 >= pos);

    size_t result_size = len1 + len2 + 1;
    char *result = (char *)malloc(sizeof(char) * result_size);
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
char *AddParenthesesIfMissing(char *arr)
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
            right++;
        }
        else if (left < right)
        {
            arr = StrAllocAndAppend(insert_str_pre, arr);
            left++;
        }
    }
    return arr;
}

void CheckCountQuestionMark(int *CountQuestionMark, int *CheckBigBranch, char *(*OutString), char *(*OutCheckBigBranch))
{
    char *InsertQuestionMark = "?";
    if (*CountQuestionMark > 1)
    {
        *OutString = AddParenthesesIfMissing(*OutString);
        int SizeToken = 0;
        int SizeTokenTemp = 0;
        int CountTemp = 0;
        int size_O = strlen(*OutString);
        char *OutStringTemp = "";
        OutStringTemp = StrAllocAndAppend(OutStringTemp, *OutString); // Lưu tạm thời OUT vào OUT_T
        char *token = strtok(*OutString, "?");                        /// (...)?((.....)?(......))
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
            *OutCheckBigBranch = StrAllocAndAppend(*OutCheckBigBranch, token1); /// (...)?
            *OutCheckBigBranch = StrAllocAndAppend(*OutCheckBigBranch, InsertQuestionMark);
            token1 = strtok(NULL, "?");
        }
        *OutString = "";
        *OutString = StrAllocAndAppend(*OutString, token1);
        *OutString = StrAllocAndAppend(*OutString, InsertQuestionMark);
        token1 = strtok(NULL, "?");
        *OutString = StrAllocAndAppend(*OutString, token1);
        *CheckBigBranch = 1;
        *CountQuestionMark = 1;
    }
}

char *CheckQuestionMask(int CountQuestionMark, char *OutString)
{
    if (CountQuestionMark > 0)
    {
        char *tempString;
        char *InsertQuestionMark = "?";
        int SizeToken = 0;
        int SizeTokenTemp = 0;
        int CountTemp = 0;
        int size_O = strlen(OutString);
        char *OutStringTemp = "";
        char *OutStringTemp_1 = "";
        OutStringTemp_1 = StrAllocAndAppend(OutStringTemp_1, OutString);
        OutStringTemp = StrAllocAndAppend(OutStringTemp, OutString); // Lưu tạm thời OUT vào OUT_T
        char *token = strtok(OutStringTemp_1, "?");                  /// (...)?((.....)?(......))
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
        for (int i = 0; i < CountTemp - 1; i++)
        {
            token1 = strtok(NULL, "?");
        }
        tempString = "";
        tempString = StrAllocAndAppend(tempString, token1);

        return tempString;
    }
    else
    {
        return OutString;
    }
}

void DeleteLinkList(LinkList *(*pMain), LinkList *(*pPREV), LinkList *(*pNEXT))
{
    (*pPREV)->next = (*pNEXT);
    (*pNEXT)->prev = (*pPREV);
    free((*pMain));
}

void CheckNOT(LinkList *(*pMain), LinkList *pNext, LinkList *pNext1, char *(*OutString), char *Insert)
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

void AddPlusToOutString(LinkList *(*pMain), char *(*OutString), int *CountQuestionMark, char *Insert)
{
    char *InsertOpeningBracket = "(";
    char *InsertClosingBracket = ")";
    LinkList *pNext;
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
    (*CountQuestionMark)--; // Thay đấu "?" bằng dấu "+"
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

void SetupTimer(LinkList *(*pMain), char *(*OutString), char *NameTimer, FILE *pFileTimer,int CountTimer)
{
    char *temparray = "";
    char *buffer="" ;
     buffer = StrAllocAndAppend(buffer, (*pMain)->data);
    *pMain = (*pMain)->next;
     buffer = StrAllocAndAppend(buffer, (*pMain)->data);
    H_InsertFunction(buffer) ;
    (*pMain)->data = StrAllocAndAppend((*pMain)->data, NameTimer);
    temparray = StrAllocAndAppend(temparray, (*pMain)->data);
    (*pMain)->data = StrAllocAndAppend("vao", (*pMain)->data);
    *OutString = StrAllocAndAppend(*OutString, " ;\n");
    *OutString = StrAllocAndAppend((*pMain)->data, *OutString);
    *pMain = (*pMain)->next;
    fprintf(pFileTimer," handle_timer[%d] =  xTimerCreate(\"timer%s\", pdMS_TO_TICKS(%s),pdTRUE,(void *)(%d+1),%s) ;\n",CountTimer,NameTimer,(*pMain)->next,CountTimer,buffer);
    temparray = StrAllocAndAppend("dat", temparray);
    temparray = StrAllocAndAppend(temparray, (*pMain)->data);
    *OutString = StrAllocAndAppend(*OutString, temparray);
    *OutString = StrAllocAndAppend(*OutString, " ;\n");
   
}
void SetupCounterUpOrDown(LinkList *(*pMain), char *OutString, char *NameCounter, FILE *pFile)
{
    char *TempArry = "";
    char *token = strtok(OutString, "?");
    *pMain = (*pMain)->next;
    TempArry = StrAllocAndAppend((*pMain)->data, NameCounter);
    TempArry = StrAllocAndAppend("vao", TempArry);
    TempArry = StrAllocAndAppend(TempArry, token);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
    token = strtok(NULL, "?");
    TempArry = StrAllocAndAppend((*pMain)->data, NameCounter);
    TempArry = StrAllocAndAppend("reset", TempArry);
    TempArry = StrAllocAndAppend(TempArry, token);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
    TempArry = StrAllocAndAppend((*pMain)->data, NameCounter);
    TempArry = StrAllocAndAppend("dat", TempArry);
    *pMain = (*pMain)->next;
    TempArry = StrAllocAndAppend(TempArry, (*pMain)->data);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
}
void SetupCounterUpDown(LinkList *(*pMain), char *OutString, FILE *pFile) // "_CTU ="
{
    char *TempArry = "";
    char *token = strtok(OutString, "?");
    *pMain = (*pMain)->next;
    TempArry = StrAllocAndAppend((*pMain)->data, "_CTUD = ");
    TempArry = StrAllocAndAppend("tang", TempArry);
    TempArry = StrAllocAndAppend(TempArry, token);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
    token = strtok(NULL, "?");
    TempArry = StrAllocAndAppend((*pMain)->data, "_CTUD = ");
    TempArry = StrAllocAndAppend("giam", TempArry);
    TempArry = StrAllocAndAppend(TempArry, token);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
    token = strtok(NULL, "?");
    TempArry = StrAllocAndAppend((*pMain)->data, "_CTUD = ");
    TempArry = StrAllocAndAppend("reset", TempArry);
    TempArry = StrAllocAndAppend(TempArry, token);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
    TempArry = StrAllocAndAppend((*pMain)->data, "_CTUD = ");
    TempArry = StrAllocAndAppend("dat", TempArry);
    *pMain = (*pMain)->next;
    TempArry = StrAllocAndAppend(TempArry, (*pMain)->data);
    TempArry = StrAllocAndAppend(TempArry, " ;\n");
    fputs(TempArry, pFile);
}

///////////////////////////////////////////////////////////////////////////////

extern stringHashTable SaveIO[PrimeNumber];

void DefineRegionMemory(FILE *pFile, int memoryRegion, int sumOfmem)
{
    char check[2];
    if (memoryRegion == I_MEM)
    {
        fprintf(pFile, "\n\n// Define I\n");
        strcpy(check, "I");
    }
    else if (memoryRegion == Q_MEM)
    {
        fprintf(pFile, "\n\n// Define Q\n");
        strcpy(check, "Q");
    }
    else if (memoryRegion == M_MEM)
    {
        fprintf(pFile, "\n\n// Define M\n");
        strcpy(check, "M");
    }

    for (int x = 0; x < sumOfmem; x++)
    {
        int bytedef = 1; // Định nghĩa 1 byte , dW , W
        for (int i = 0; i < 8; i++)
        {
            if (bytedef == 1)
            {
                fprintf(pFile, "#define %sB%d %s[%d][%d]\n", check, x, check, x, i);
                fprintf(pFile, "#define %sDW%d %s[%d][%d]\n", check, x, check, x, i);
                fprintf(pFile, "#define %sW%d %s[%d][%d]\n", check, x, check, x, i);
                bytedef = 0;
            }
            fprintf(pFile, "#define %s%d_%d %s[%d][%d]\n", check, x, i, check, x, i);
        }
    }
}

void DefineIO(FILE *pFile, int memoryRegion, int sumOfmem)
{
    char check[2];
    if (memoryRegion == I_MEM)
    {
        fprintf(pFile, "\n\n// Define Input Pin\n");
        strcpy(check, "I");
    }
    else if (memoryRegion == Q_MEM)
    {
        fprintf(pFile, "\n\n// Define Output pin\n");
        strcpy(check, "Q");
    }
    for (int x = 0; x < sumOfmem; x++)
    {
        char *compare;
        for (int i = 0; i < 8; i++)
        {
            char buffer[15];
            sprintf(buffer, "%s%d_%d", check, x, i);
            compare = buffer;
            int Search = H_FindFunction(compare);
            if (Search >= 0)
                fprintf(pFile, "#define %s_PIN GPIO_PIN_\n", SaveIO[Search]);
        }
    }

    if (memoryRegion == I_MEM)
    {
        fprintf(pFile, "\n\n// Define Input Port\n");
    }
    else if (memoryRegion == Q_MEM)
    {
        fprintf(pFile, "\n\n// Define Output Port\n");
    }
    for (int x = 0; x < sumOfmem; x++)
    {
        char *compare;

        for (int i = 0; i < 8; i++)
        {
            char buffer[15];
            sprintf(buffer, "%s%d_%d", check, x, i);
            compare = buffer;
            int Search = H_FindFunction(compare);
            if (Search >= 0)
                fprintf(pFile, "#define %s_PORT GPIO\n", SaveIO[Search]);
        }
    }
}

void readInputPin(FILE *pFile, int sumOfmem)
{
    fprintf(pFile, "{\n");
    for (int x = 0; x < sumOfmem; x++)
    {
        char *compare;
        for (int i = 0; i < 8; i++)
        {
            char buffer[15];
            sprintf(buffer, "I%d_%d", x, i);
            compare = buffer;
            int Search = H_FindFunction(compare);
            if (Search >= 0)
                fprintf(pFile, "%s = !HAL_GPIO_ReadPin(%s_PORT, %s_PIN);\n", SaveIO[Search], SaveIO[Search], SaveIO[Search]);
        }
    }
    fprintf(pFile, "}\n");
}

void writeOutputPin(FILE *pFile, int sumOfmem)
{
    fprintf(pFile, "{\n");
    for (int x = 0; x < sumOfmem; x++)
    {
        char *compare;
        for (int i = 0; i < 8; i++)
        {
            char buffer[15];
            sprintf(buffer, "Q%d_%d", x, i);
            compare = buffer;
            int Search = H_FindFunction(compare);
            if (Search >= 0)
            {
                fprintf(pFile, "if(%s >= 1){\n", SaveIO[Search]);
                fprintf(pFile, "		HAL_GPIO_WritePin(%s_PORT, %s_PIN, ENABLE);\n", SaveIO[Search], SaveIO[Search]);
                fprintf(pFile, "		}\n");
                fprintf(pFile, "	else{\n");
                fprintf(pFile, "		HAL_GPIO_WritePin(%s_PORT, %s_PIN, DISABLE);\n", SaveIO[Search], SaveIO[Search]);
                fprintf(pFile, "	}\n");
            }
        }
    }
    fprintf(pFile, "}\n");
}

void InsertMov(LinkList *(*pMain), char *OutString, int CountQuestionMark, FILE *pFile, int checkMov)
{
    int check;
    if (checkMov == MOVB_CHECK)
        check = 1;
    if (checkMov == MOVW_CHECK)
        check = 2;
    if (checkMov == MOVDW_CHECK)
        check = 4;
    LinkList *pNext = (*pMain)->next;
    LinkList *pNext1 = pNext->next;
    pNext = pNext1->next;
    LinkList *temp;
    temp = (*pMain);
    (*pMain) = (*pMain)->next;
    DeleteLinkList(&temp, &(temp->prev), &(temp->next));
    temp = (*pMain)->next;
    if (pNext != NULL)
    {
        if (strcmp(pNext->data, "AENO") == 0)
        {

            char *InsertString = CheckQuestionMask(CountQuestionMark, OutString);
            InsertString = AddParenthesesIfMissing(InsertString);
            // printf("\n%s\n", InsertString);
            fprintf(pFile, "if(%s)\n ", InsertString);
            fprintf(pFile, " (memcpy(&%s,&%s,%d)) ;\n", temp->data, (*pMain)->data,check);
            DeleteLinkList(&(*pMain), &((*pMain)->prev), &((*pMain)->next));
            DeleteLinkList(&temp, &(temp->prev), &(temp->next));
            (*pMain) = pNext->next;
            DeleteLinkList(&pNext, &(pNext->prev), &(pNext->next));
        }
        else
        {
            OutString = AddParenthesesIfMissing(OutString);
            fprintf(pFile, "if(%s)\n ", OutString);
            fprintf(pFile, " (memcpy(&%s,&%s,1)) ; \n", temp->data, (*pMain)->data);
            DeleteLinkList(&(*pMain), &((*pMain)->prev), &((*pMain)->next));
            DeleteLinkList(&temp, &(temp->prev), &(temp->next));
            (*pMain) = pNext->next;
            DeleteLinkList(&pNext, &(pNext->prev), &(pNext->next));
        }
    }
    else
    {
        OutString = AddParenthesesIfMissing(OutString);
        fprintf(pFile, "if(%s)\n ", OutString);
        fprintf(pFile, " (memcpy(&%s,&%s,1)) ; \n", temp->data, (*pMain)->data);
    }
}

