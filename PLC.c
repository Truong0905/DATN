#include "STL.h"

LinkList *First, *Last, *FirstFinal, *LastFinal; // First và Last dùng cho  TransferToList ()   ; FirstFinal , LastFinal  là chuỗi cuối cùng cần tìm

stringHashTable SaveIO[PrimeNumber];
FILE *pFileTimer = NULL;
int CountTimer = 0;
int main(void)
{

    FILE *pMainFile = NULL;
    LinkList *pTest;
    int row = 0;
    InitSaveDataIO();
    pMainFile = ReadTextFile(&row); // Tạo file PLC.txt
    TransferToList(pMainFile, row);
    fclose(pMainFile); // Đóng  file PLC.txt
    FinalList();
    SaveDataIO();
    SplitBranchesWithFirstFinalPointer();
    pTest = FirstFinal;
    while (pTest != NULL)
    {
        printf("%s ", pTest->data);
        pTest = pTest->next;
    }
    FileData(pMainFile);
    FinalTextFile(pMainFile); // Tạo file PLC_F.txt
    fclose(pMainFile);        // Đóng file PLC_F.txt
    // for (int i = 0; i < PrimeNumber; i++)
    // {
    //     if (strcmp(SaveIO[i], "vacant") != 0 && (strcmp(SaveIO[i], "delete") != 0))
    //         printf("%d .  %s  \n", i, SaveIO[i]);
    // }
    FileDefineData();
    return EXIT_SUCCESS;
}

void InitSaveDataIO(void)
{
    for (int i = 0; i < PrimeNumber; i++)
    {
        strcpy(SaveIO[i], "vacant");
    }
}

FILE *ReadTextFile(int *RowOfFile)
{
    FILE *pfile = NULL, *pFileFinal = NULL;

    char TempArray[30];
    int count = 0;
    int ReductionCount = 0;
    int CheckProgram = 0; // Kiểm tra xem đã vào chương trình chưa
    pFileFinal = fopen("PLC.txt", "w");
    // Mở file bằn hàm fopen
    pfile = fopen("PLC.awl", "r");
    if (pfile == NULL)
    {
        printf("Can not open file");
        return NULL;
    }
    //Đọc từng dòng từ file cho tới khi gặp NULL
    while (fgets(TempArray, 128, pfile) != NULL)
    {
        count = count + 1;
        if (CheckProgram == 0 && (strncmp(TempArray, "LD", 2) == 0))
        {
            ReductionCount = count;
            CheckProgram = 1;
        }

        if (CheckProgram == 1)
        {

            if (strncmp(TempArray, "Network", 7) == 0)
            {
                fputs("N\n", pFileFinal);
            }
            else if (strncmp(TempArray, "END", 3) == 0)
            {
                fputs("N\n", pFileFinal);
                break;
            }
            else
            {
                fputs(TempArray, pFileFinal);
            }
        }
    }
    *RowOfFile = count - ReductionCount;

    fclose(pfile);
    fclose(pFileFinal);
    return fopen("PLC.txt", "r");
}

void TransferToList(FILE *pFileFinal, int RowOfFile)
{
    LinkList *pMain;
    char TempArray[20];
    First = NULL;

    for (int i = 0; i < RowOfFile; i++)
    {
        fgets(TempArray, 20, pFileFinal);
        for (int CharacterOfRow = 0; CharacterOfRow < 20; CharacterOfRow++)
        {
            pMain = (LinkList *)malloc(sizeof(LinkList));
            pMain->next = NULL;
            pMain->data = (char *)calloc(20, sizeof(char));
            for (int CheckCharacter = 0; CheckCharacter < 20; CheckCharacter++)
            {

                if (TempArray[CharacterOfRow] == ',')
                {
                    CharacterOfRow++;
                    CheckCharacter = CheckCharacter - 1;
                    continue;
                }
                if (TempArray[CharacterOfRow] == '.')
                {
                    (pMain->data)[CheckCharacter] = '_';
                    CharacterOfRow++;
                    continue;
                }

                if (TempArray[CharacterOfRow] == '\n')
                {

                    break;
                }
                (pMain->data)[CheckCharacter] = TempArray[CharacterOfRow];
                CharacterOfRow++;
            }
            if (First == NULL)
            {
                CreateTheFirstPointerOfList(&pMain, &First, &Last);
            }
            else
            {
                CreateTheElementOfList(&pMain, &Last);
            }
            if (TempArray[CharacterOfRow] == '\n')
            {
                break;
            }
        }
    }
}

void SaveDataIO(void)
{
    LinkList *pMain, *pNext;
    pMain = FirstFinal;
    while (pMain != NULL)
    {
        if ((strcmp(pMain->data, "LD") == 0))
        {
            pMain = pMain->next;
            H_InsertFunction(pMain->data);
        }
        else if ((strcmp(pMain->data, "O") == 0))
        {
            pMain = pMain->next;
            H_InsertFunction(pMain->data);
        }
        else if ((strcmp(pMain->data, "A") == 0))
        {
            pMain = pMain->next;
            H_InsertFunction(pMain->data);
        }
        else if ((strcmp(pMain->data, "=") == 0))
        {
            pMain = pMain->next;
            H_InsertFunction(pMain->data);
        }
        else if ((strncmp(pMain->data, "CT", 2) == 0))
        {
            pMain = pMain->next;
            H_InsertFunction(pMain->data);
        }
        pMain = pMain->next;
    }
}

void FinalList(void)
{
    LinkList *pMainOfFirst, *pMainOfFirstFinal;
    pMainOfFirst = First;
    FirstFinal = NULL;
    int SizeOfToken = 0; // Đếm size token
    int CheckClose = 0;  // Xác định tiếp điểm thường đóng
    while (pMainOfFirst != NULL)
    {
        char *token = strtok(pMainOfFirst->data, " ");
        while (token != NULL)
        {
            // tính kích  thước token để tạo vùng nhớ cho  dữ liệu của phần tử mới
            SizeOfToken = strlen(token);
            // cấp phát vùng nhớ cho phần tử
            pMainOfFirstFinal = (LinkList *)malloc(sizeof(LinkList));

            pMainOfFirstFinal->next = NULL;
            if (CheckClose == 0) // Nếu ko phải là  tiếp điểm thường đóng  ta copy token vào phần tử
            {
                pMainOfFirstFinal->data = (char *)calloc(SizeOfToken, sizeof(char));
                strcpy(pMainOfFirstFinal->data, token);
            }
            else // Nếu là tiếp điểm thường  đóng
            {
                pMainOfFirstFinal->data = (char *)calloc(SizeOfToken, sizeof(char));
                strcpy(pMainOfFirstFinal->data, token);
                // Chèn tín hiệu thường đóng vào bảng
                H_InsertFunction(pMainOfFirstFinal->data);
                // Chèn tín hiệu thường đóng vào  Link list (!I0_0)
                pMainOfFirstFinal->data = StrAllocAndAppend("(!", pMainOfFirstFinal->data);
                pMainOfFirstFinal->data = StrAllocAndAppend(pMainOfFirstFinal->data, ")");
                CheckClose = 0;
            }
            /// Lưu địa chỉ con trỏ FirstFinal và con trỏ LastFinal
            if (FirstFinal == NULL)
            {
                CreateTheFirstPointerOfList(&pMainOfFirstFinal, &FirstFinal, &LastFinal);
            }
            else
            {
                CreateTheElementOfList(&pMainOfFirstFinal, &LastFinal);
            }
            // xác định xem có phải tiếp điểm thường đóng không bằng cách kiểm tra câu lệnh là sau đó gán b=1 để lần sau thêm 'H'
            if (strncmp(token, "LDN", 3) == 0 || strncmp(token, "AN", 2) == 0 || strncmp(token, "ON", 2) == 0)
            {
                CheckClose = 1;
            }

            // Nếu là xung sườn lên ta thêm "sl" vào sau biến bằng cách tạo 1 phần tử mới thế vào vị trí của phần tử cũ và xóa phần tử cũ đi
            if (strncmp(token, "EU", 2) == 0)
            {
                char *edge = "sl";
                InsertEdge(pMainOfFirstFinal, edge);
            }
            // Nếu là xung sườn xuống  ta thêm "sx" vào sau biến bằng cách tạo 1 phần tử mới thế vào vị trí của phần tử cũ và xóa phần tử cũ đi
            if (strncmp(token, "ED", 2) == 0)
            {
                char *edge = "sx";
                InsertEdge(pMainOfFirstFinal, edge);
            }

            token = strtok(NULL, " ");
        }
        pMainOfFirst = pMainOfFirst->next;
    }
}

void SplitBranchesWithFirstFinalPointer(void)
{
    LinkList *pCheck, *pMain, *pTemp;
    LinkList *pNext, *pNext1, *pPrev, *pPrev1;
    int CoutOpen = 0;  // Đếm số "("
    int CoutClose = 0; // Đếm số ")"
    pCheck = FirstFinal;
    while (pCheck != NULL)
    {
        if (pCheck == FirstFinal) // Thêm ký tự '(' vào đầu
        {
            pMain = (LinkList *)malloc(sizeof(LinkList));
            pMain->data = (char *)calloc(2, sizeof(char));
            pMain->data = "(\0";
            CoutOpen++;
            RecreateTheFirstElement(&pMain, &pCheck);
            FirstFinal = pMain;
            pNext = pCheck->next;
            pNext1 = pNext->next;
            if (strcmp(pNext1->data, "NOT") == 0)
            {
                pTemp = (LinkList *)malloc(sizeof(LinkList));
                pTemp->data = (char *)calloc(4, sizeof(char));
                pTemp->data = "not\0";
                RecreateTheFirstElement(&pTemp, &pMain);
                FirstFinal = pTemp;
            }
        }
        else
        {
            if ((strcmp(pCheck->data, "ALD") == 0) || (strcmp(pCheck->data, "OLD") == 0) || (strcmp(pCheck->data, "TON") == 0) || (strcmp(pCheck->data, "TONR") == 0) || (strcmp(pCheck->data, "TOF") == 0) || (strcmp(pCheck->data, "CTU") == 0) || (strcmp(pCheck->data, "CTD") == 0) || (strcmp(pCheck->data, "CTUD") == 0))
            {
                if ((strcmp(pCheck->data, "ALD") == 0) || (strcmp(pCheck->data, "OLD") == 0)) // not của 1 nhánh
                {
                    pNext = pCheck->next;
                    if (strcmp(pNext->data, "NOT") == 0)
                    {
                        pMain = (LinkList *)malloc(sizeof(LinkList));
                        pMain->data = (char *)calloc(2, sizeof(char));
                        pMain->data = "!\0";
                        InsertNextElement(&pCheck, &pMain, &pNext);
                    }
                }
                pPrev = pCheck->prev;
                if ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0)) // Đây ko phải nhánh lớn
                {
                    pMain = (LinkList *)malloc(sizeof(LinkList));
                    pMain->data = (char *)calloc(2, sizeof(char));
                    pMain->data = ")\0";
                    CoutClose++;
                    InsertPrevElement(&pCheck, &pMain);
                }
            }
            else if ((strcmp(pCheck->data, "=") == 0))
            {
                if (CoutOpen > CoutClose)
                {
                    pMain = (LinkList *)malloc(sizeof(LinkList));
                    pMain->data = (char *)calloc(2, sizeof(char));
                    pMain->data = ")\0";
                    CoutClose++;
                    InsertPrevElement(&pCheck, &pMain);
                }
            }
            else if ((strncmp(pCheck->data, "LD", 2) == 0) || (strncmp(pCheck->data, "LDN", 3) == 0))
            {
                pNext = pCheck->next;
                pNext1 = pNext->next;
                pPrev = pCheck->prev;
                pPrev1 = pPrev->prev;
                if ((strcmp(pPrev1->data, "=") == 0) || (strcmp(pPrev->data, "N") == 0) || (strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0))
                {
                    pMain = (LinkList *)malloc(sizeof(LinkList));
                    pMain->data = (char *)calloc(2, sizeof(char));
                    pMain->data = "(\0";
                    CoutOpen++;
                    InsertPrevElement(&pCheck, &pMain);
                }
                else
                {
                    if (CoutOpen == CoutClose)
                    {
                        pMain = (LinkList *)malloc(sizeof(LinkList));
                        pMain->data = (char *)calloc(2, sizeof(char));
                        pMain->data = "(\0";
                        CoutOpen++;
                        InsertPrevElement(&pCheck, &pMain);
                    }
                    else
                    {
                        pMain = (LinkList *)malloc(sizeof(LinkList));
                        pMain->data = (char *)calloc(2, sizeof(char));
                        pMain->data = ")\0";
                        CoutOpen++;
                        InsertPrevElement(&pCheck, &pMain);
                        pMain = (LinkList *)malloc(sizeof(LinkList));
                        pMain->data = (char *)calloc(2, sizeof(char));
                        pMain->data = "(\0";
                        CoutClose++;
                        InsertPrevElement(&pCheck, &pMain);
                    }
                }
                if (strcmp(pNext1->data, "NOT") == 0)
                {
                    pMain = (LinkList *)malloc(sizeof(LinkList));
                    pMain->data = (char *)calloc(4, sizeof(char));
                    pMain->data = "not\0";
                    InsertNextElement(&pPrev1, &pMain, &pPrev);
                }
            }
            else if ((strncmp(pCheck->data, "A",1) == 0) || (strncmp(pCheck->data, "O",1) == 0) || (strncmp(pCheck->data, "AN",2) == 0) || (strncmp(pCheck->data, "ON",2) == 0))
            {   
                if ((strncmp(pCheck->data, "AW",2) == 0) || (strncmp(pCheck->data, "OW",2) == 0))
                {
                    // Với trường hợp các câu lệnh so sánh OW , OW
                pNext = pCheck->next; // C1
                pNext1 = pNext->next; // 5
                pNext1 = pNext1->next ; // NOT
                pPrev = pCheck->prev; // 3
                }
                else
                {
                    // câu lệnh thường
                pNext = pCheck->next; // I0_1
                pNext1 = pNext->next; // NOT
                pPrev = pCheck->prev; // I0_0
                }

                if (strcmp(pNext1->data, "NOT") == 0) // NOT của 1 tập hợp các biến
                {
                    int check_nhanh = 0;
                    while (strncmp(pPrev->data, "(", 1) != 0) // Tìm vị trí đầu của nhánh
                    {
                        if ((strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0)) // nếu đâu là not của (    nhánh và các biến )
                        {
                            check_nhanh = 1;
                            break;
                        }
                        pPrev = pPrev->prev;
                    }
                    if (pPrev != FirstFinal)
                    {
                        if (check_nhanh == 0)
                        {
                            pPrev1 = pPrev->prev;
                            if (strncmp(pPrev1->data, ")", 1) != 0)
                            {
                                pMain = (LinkList *)malloc(sizeof(LinkList));
                                pMain->data = (char *)calloc(4, sizeof(char));
                                pMain->data = "not\0";
                                InsertPrevElement(&pPrev, &pMain);
                            }
                            else
                            {
                                pMain = (LinkList *)malloc(sizeof(LinkList));
                                pMain->data = (char *)calloc(5, sizeof(char));
                                pMain->data = "1not\0";
                                InsertPrevElement(&pPrev, &pMain);
                            }
                        }
                        else
                        {

                            pNext1->data = StrAllocAndAppend("T_", pNext1->data);
                        }
                    }
                    else
                    {
                        pMain = (LinkList *)malloc(sizeof(LinkList));
                        pMain->data = (char *)calloc(4, sizeof(char));
                        pMain->data = "not\0";
                        RecreateTheFirstElement(&pMain, &pPrev);
                        FirstFinal = pMain;
                    }
                }
            }
            else if ((strncmp(pCheck->data, "MOV", 3) == 0))
            {
                pPrev = pCheck->prev;
                if ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0) && (strcmp(pPrev->data, "AENO") != 0))
                {
                    pMain = (LinkList *)malloc(sizeof(LinkList));
                    pMain->data = (char *)calloc(2, sizeof(char));
                    pMain->data = ")\0";
                    CoutClose++;
                    InsertPrevElement(&pCheck, &pMain);
                }
            }
        }
        pCheck = pCheck->next;
    }
}

void FinalTextFile(FILE *pFile)
{
    LinkList *pMain, *pNext, *pNext1, *pPrev, *pPrev1;
    pFile = fopen("DataPLCc.txt", "a");
    fprintf(pFile, "{\n");
    fprintf(pFile, "while(1)\n{\n");
    char *OutString;
    char *InsertOpeningBracket = "(";
    char *InsertClosingBracket = ")";
    char *InsertMul = "*";
    char *Insertplus = "+";
    char *InsertQuestionMark = "?";
    int CheckEndNetWork = 0; // Kiểm tra xem hết 1 network chưa
    int CheckBigBranch = 0;
    char *OutCheckBigBranch = ""; // Kiểm tra xem có nhánh lớn trước đó không
    int CountQuestionMark = 0;    // Đếm số dấu "?"
    pMain = FirstFinal;
    int CountNetWork = 0;
    int checkTimer = 1;

    while (pMain != NULL)
    {

        if (CheckEndNetWork == 0) // Chưa kết thúc 1 network
        {
            char buffer[50];
            CountNetWork++;
            sprintf(buffer, "/*--------------NetWork %d -----------*/\n\n", CountNetWork);
            fputs(buffer, pFile);
            OutString = "";
            CheckEndNetWork = 1;
        }
        if (strcmp(pMain->data, "N") == 0)
        {

            CheckEndNetWork = 0;
            pMain = pMain->next;
            continue;
        }

        if (strncmp(pMain->data, "(", 1) == 0)
        {
            pPrev = pMain->prev;
            if (pPrev != NULL)
            {
                pPrev1 = pPrev->prev;
                if ((strncmp(pPrev->data, "not", 3) != 0) && (strncmp(pPrev->data, "1no", 3) != 0))
                {
                    if (strncmp(pPrev->data, ")", 1) == 0) // Nếu trước  đó sẽ có dấu ")" thì thêm "?" vào giữa 2 dấu
                    {
                        OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                        CountQuestionMark++;
                        OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                        pMain = pMain->next;
                        continue;
                    }
                    else
                    {
                        OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    }
                }
                else if ((strncmp(pPrev->data, "1not", 3) == 0) || (strncmp(pPrev->data, "not", 3) == 0))
                {
                    int search_not = 0;

                    if (pPrev1 == NULL) // Đầu chương trình
                    {
                        OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                        pMain = pMain->next;
                        continue;
                    }
                    else
                    {
                        while ((strncmp(pPrev->data, "1not", 3) == 0) || (strncmp(pPrev->data, "not", 3) == 0))
                        {

                            pPrev = pPrev->prev;
                            if (pPrev == NULL)
                            {
                                break;
                            }
                            if ((strncmp(pPrev->data, "ALD", 3) == 0) || (strncmp(pPrev->data, "OLD", 3) == 0))
                            {
                                search_not = 1;
                            }
                        }
                    }
                    if (search_not == 1)
                    {
                        OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                        CountQuestionMark++;
                    }
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    pMain = pMain->next;
                    continue;
                }
            }
            else
            {
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
            }
        }
        else if (strncmp(pMain->data, ")", 1) == 0)
        {
            pNext = pMain->next;
            pNext1 = pNext->next;
            if ((strncmp(pNext->data, "not", 3) != 0) && (strncmp(pNext->data, "1no", 3) != 0))
            {
                if (strncmp(pNext->data, "(", 1) == 0) // Nếu sau đó sẽ có dấu "(" thì thêm "?" vào giữa 2 dấu
                {
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                    OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                    CountQuestionMark++;
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    pMain = pNext->next;
                    continue;
                }
                else
                {
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                }
            }
            else if (strncmp(pNext->data, "1no", 3) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;

                while ((strncmp(pNext1->data, "1no", 3) == 0) || (strncmp(pNext1->data, "not", 3) == 0))
                {

                    pNext1 = pNext1->next; // Bỏ qua các phần tử 1not và not
                }

                pMain = pNext1;
                continue;
            }
        }
        else if ((strcmp(pMain->data, "LD") == 0) || (strcmp(pMain->data, "LDN") == 0))
        {
            pPrev = pMain->prev;
            pPrev1 = pPrev->prev;
            pNext = pMain->next;
            pNext1 = pNext->next;
            if (pPrev1 != NULL) // dảm bảo kể cả à đâu chương trình mà có not thì vẫn được xét thêm "!"
            {
                if ((strncmp(pPrev1->data, "not", 3) == 0) || (strncmp(pPrev1->data, "1no", 3) == 0))
                {
                    OutString = StrAllocAndAppend(OutString, "!");
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    pPrev = pPrev1->prev;
                    if (pPrev != NULL)
                    {
                        while ((strncmp(pPrev->data, "not", 3) == 0) || (strncmp(pPrev->data, "1no", 3) == 0))
                        {
                            OutString = StrAllocAndAppend(OutString, "!");
                            OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                            pPrev = pPrev->prev;
                            if (pPrev == NULL)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            OutString = StrAllocAndAppend(OutString, pNext->data);
            pMain = pNext1;
            continue;
        }
        else if ((strcmp(pMain->data, "EU") == 0) || (strcmp(pMain->data, "ED") == 0))
        {
            pMain = pMain->next;
            continue;
        }
        else if ((strcmp(pMain->data, "A") == 0) || (strcmp(pMain->data, "AN") == 0))
        {
            pPrev = pMain->prev; // LinkList *pPREV_temp = pPrev;
            pNext = pMain->next;
            pNext1 = pNext->next;
            CheckNOT(&pMain, pNext, pNext1, &OutString, InsertMul);

            while ((strcmp(pMain->data, "A") == 0) || (strcmp(pMain->data, "AN") == 0))
            {

                pNext = pMain->next;
                pNext1 = pNext->next;
                CheckNOT(&pMain, pNext, pNext1, &OutString, InsertMul);
            }
            if ((strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0)) // nhánh nhân hoặc cộng với biến
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            }
            if (strncmp(pMain->data, "(", 1) == 0)
            {
                if ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0))
                {
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                }

                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;
            }
            if (((strcmp(pMain->data, "O") == 0) || (strcmp(pMain->data, "ON") == 0)) && ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0)))
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            }

            if ((strncmp(pMain->data, ")", 1) == 0) && ((strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0)))
            {
                pMain = pMain->next;
            }
            if (((strcmp(pMain->data, "ALD") == 0) || (strcmp(pMain->data, "OLD") == 0)) && CheckBigBranch == 1)
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = "";
            }

            continue;
        }
        else if ((strcmp(pMain->data, "O") == 0) || (strcmp(pMain->data, "ON") == 0))
        {
            pPrev = pMain->prev;
            pNext = pMain->next;
            pNext1 = pNext->next;
            CheckNOT(&pMain, pNext, pNext1, &OutString, Insertplus);
            while ((strcmp(pMain->data, "O") == 0) || (strcmp(pMain->data, "ON") == 0))
            {

                pNext = pMain->next;
                pNext1 = pNext->next;
                CheckNOT(&pMain, pNext, pNext1, &OutString, Insertplus);
            }
            if ((strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0)) // nhánh nhân hoặc cộng với biến
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            }
            if (strncmp(pMain->data, "(", 1) == 0)
            {
                if ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0))
                {
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                }

                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;
            }
            if (((strcmp(pMain->data, "A") == 0) || (strcmp(pMain->data, "AN") == 0)) && ((strcmp(pPrev->data, "ALD") != 0) && (strcmp(pPrev->data, "OLD") != 0)))
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            }

            if ((strncmp(pMain->data, ")", 1) == 0) && ((strcmp(pPrev->data, "ALD") == 0) || (strcmp(pPrev->data, "OLD") == 0)))
            {
                pMain = pMain->next;
            }

            if (((strcmp(pMain->data, "ALD") == 0) || (strcmp(pMain->data, "OLD") == 0)) && CheckBigBranch == 1)
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = "";
            }

            continue;
        }
        else if (strcmp(pMain->data, "ALD") == 0)
        {
            CheckCountQuestionMark(&CountQuestionMark, &CheckBigBranch, &OutString, &OutCheckBigBranch);
            AddPlusToOutString(&pMain, &OutString, &CountQuestionMark, InsertMul);
            if (strncmp(pMain->data, "(", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;

                continue;
            }
            pNext = pMain->next;
            if ((CheckBigBranch == 1) && (((strncmp(pMain->data, "OLD", 1) == 0) || (strncmp(pMain->data, "ALD", 1) == 0)) || ((strncmp(pNext->data, "OLD", 1) == 0) || (strncmp(pNext->data, "ALD", 1) == 0))))
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = "";
            }
            continue;
        }
        else if (strcmp(pMain->data, "OLD") == 0)
        {
            CheckCountQuestionMark(&CountQuestionMark, &CheckBigBranch, &OutString, &OutCheckBigBranch);
            AddPlusToOutString(&pMain, &OutString, &CountQuestionMark, Insertplus);

            if (strncmp(pMain->data, "(", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;

                continue;
            }
            pNext = pMain->next;
            if ((CheckBigBranch == 1) && (((strncmp(pMain->data, "OLD", 1) == 0) || (strncmp(pMain->data, "ALD", 1) == 0)) || ((strncmp(pNext->data, "OLD", 1) == 0) || (strncmp(pNext->data, "ALD", 1) == 0))))
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = "";
            }
            continue;
        }
        else if (strcmp(pMain->data, "T_NOT") == 0)
        {
            OutString = StrAllocAndAppend(InsertOpeningBracket, OutString);
            OutString = StrAllocAndAppend("!", OutString);
            OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            pMain = pMain->next;
            ;
            if (strncmp(pMain->data, ")", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                pMain = pMain->next;
            }
            if (strncmp(pMain->data, "(", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;
                continue;
            }
            if (strncmp(pMain->data, "not", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
            }

            if (((strcmp(pMain->data, "ALD") == 0) || (strcmp(pMain->data, "OLD") == 0)) && CheckBigBranch == 1)
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = "";
            }
            continue;
        }
        else if (strcmp(pMain->data, "=") == 0) // q0.1=(A*B)\n
        {

            OutString = AddParenthesesIfMissing(OutString);
            OutString = StrAllocAndAppend(OutString, " ;\n");
            OutString = StrAllocAndAppend(pMain->data, OutString);
            pMain = pMain->next;
            OutString = StrAllocAndAppend(pMain->data, OutString);
            OutCheckBigBranch = "";
            fputs(OutString, pFile);
            fprintf(pFile, "if (%s>0)\n{\n%s = 1;\n}\nelse\n{\n%s = 0 ;\n}\n", pMain->data, pMain->data, pMain->data);
        }
        else if (strcmp(pMain->data, "CTU") == 0)
        {
            OutString = AddParenthesesIfMissing(OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            SetupCounterUpOrDown(&pMain, OutString, pFile);
            // pMAin : 5
            pPrev = pMain->prev; // C1
            fprintf(pFile, "volatile static uint8_t start%s = 1 ; \nvolatile static uint8_t check%s = 0 ;\n", pPrev->data, pPrev->data);
            fprintf(pFile, "if (reset%s) \n{\ncount%s = 0 ;\n%s = 0 ; \nstart%s = 1 ;\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else\n{\nif (vao%s)\n{\n ", pPrev->data);
            fprintf(pFile, "if ( ( (check%s ==1 ) || ( start%s ==1 ) ) && (count%s <= 4294967295)  )\n", pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "{\ncount%s ++ ;\nstart%s == 0 ;\n}\nif (count%s >= dat%s )\n{%s =1 ;\n}\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else \n{\n check%s = 1 ;\n}\n}\n", pPrev->data);
        }
        else if (strcmp(pMain->data, "CTD") == 0)
        {
            OutString = AddParenthesesIfMissing(OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            SetupCounterUpOrDown(&pMain, OutString, pFile);
            // pMAin : 12
            pPrev = pMain->prev; // c2
            fprintf(pFile, "volatile static uint32_t nho%s = dat%s ;\nvolatile static uint8_t start%s = 1 ;\nvolatile static uint8_t check%s = 0 ;\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "if (reset%s)\n{\nnho%s = dat%s ;\ncount%s = dat%s ;\n%s = 0 ;\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else \n{\nif (vao%s)\n{\n", pPrev->data);
            fprintf(pFile, "if ( ( (checkC2 ==1 ) || ( start%s ==1 ))  && ( nho%s > 0 ))\n{\nnho%s --  ;\ncount%s = nho%s ;\nstart%s == 0 ;\n}\n ", pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "if (count%s <= 0 )\n{\n%s = 1 ;\n}\n}\nelse \n{\ncheck%s = 1 ;\n}\n}\n", pPrev->data, pPrev->data, pPrev->data);
        }
        else if (strcmp(pMain->data, "CTUD") == 0)
        {
            OutString = AddParenthesesIfMissing(OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            SetupCounterUpDown(&pMain, OutString, pFile);
            // pMAin = 12 ;
            pPrev = pMain->prev; // C3
            fprintf(pFile, "volatile static uint8_t start%stang = 1 ;\nvolatile static uint8_t start%sgiam = 1 ;\nvolatile static uint8_t check%stang = 0 ;\nvolatile static uint8_t check%sgiam = 0 ;\nvolatile static uint32_t nho%s = 0 ;\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "if (reset%s)\n{\nnho%s = 0 ;\ncount%s = 0 ;\n%s = 0 ;\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else \n{\n\nif (tang%s)\n{\n", pPrev->data);
            fprintf(pFile, "if ( ( (check%stang ==1 ) || ( start%stang ==1 ))  && ( count%s <= 4294967295 ))\n ", pPrev->data);
            fprintf(pFile, "{\nnho%s ++  ;\ncount%s = nho%s ;\nstart%stang == 0 ;\n}\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else \n{\ncheck%stang = 1 ;\n}\n", pPrev->data);
            fprintf(pFile, "if (giam%s)\n{\n", pPrev->data);
            fprintf(pFile, "if ( ( (check%sgiam ==1 ) || ( start%sgiam ==1 ))  && ( nho%s > 0 )) \n", pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "{\nnho%s --  ;\ncount%s = nho%s ;\nstart%sgiam == 0 ;\n}\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
            fprintf(pFile, "else\n{\ncheck%sgiam = 1 ;\n}\n ", pPrev->data);
            fprintf(pFile, "if (count%s >=dat%s )\n{%s = 1 ;\n}\nelse\n{\n%s = 0 ;\n}\n}\n", pPrev->data, pPrev->data, pPrev->data, pPrev->data);
        }
        else if (strcmp(pMain->data, "TON") == 0)
        {
            if (checkTimer == 1)
            {
                pFileTimer = fopen("Timer.txt", "w");
                fprintf(pFileTimer, "void initTimer(void)\n");
                fprintf(pFileTimer, "{\n");
                checkTimer = 0;
            }
            OutString = AddParenthesesIfMissing(OutString);

            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            InsertTimer(pMain, &CountTimer, pFileTimer);
            // main -> data : TON
            char *equal = " = ";
            char *temparray = "";
            char *temparray1 = "";
            pMain = pMain->next;                                            // T97
            temparray = StrAllocAndAppend(temparray, pMain->data);          // T97
            temparray1 = StrAllocAndAppend(temparray1, pMain->data);        // T97
            pMain->data = StrAllocAndAppend("volatile uint8_t vao", pMain->data);    // int vaoT97
            pMain->data = StrAllocAndAppend(pMain->data, equal);            // int vaoT97 =
            OutString = StrAllocAndAppend(OutString, " ;\n");               // (I0_1&I0_1) ;\n
            OutString = StrAllocAndAppend(pMain->data, OutString);          // int vaoT97 = (I0_1&I0_1) ;\n
            pMain = pMain->next;                                            // 20
            temparray = StrAllocAndAppend("volatile const uint32_t dat", temparray); // int datT97
            temparray = StrAllocAndAppend(temparray, equal);                // int datT97 =
            temparray = StrAllocAndAppend(temparray, pMain->data);          // int datT97 = 20
            OutString = StrAllocAndAppend(OutString, temparray);            // int vaoT97_ON = (I0_1&I0_1) ;\n int datT97_ON = 20
            OutString = StrAllocAndAppend(OutString, " ;\n");
            fputs(OutString, pFile);
            fprintf(pFile, "volatile static uint8_t check%s = 0 ; \n ", temparray1);
            fprintf(pFile, "if ( vao%s  )\n{\n", temparray1);
            fprintf(pFile, "if ( count%s >= dat%s  ) \n%s = 1 ;\n", temparray1, temparray1, temparray1);
            fprintf(pFile, " \nif (%s == 0 )\n{\n", temparray1);
            fprintf(pFile, "xTimerStart (handle_timer[%d], portMAX_DELAY);\n", CountTimer - 1);
            fprintf(pFile, "check%s = 0;\n}\n}\n", temparray1);
            fprintf(pFile, "else \n");
            fprintf(pFile, "{\nif( checkvao%s = 0 )\n{\n", temparray1);
            fprintf(pFile, "xTimerStop(handle_timer[%d], portMAX_DELAY);\n", CountTimer - 1);
            fprintf(pFile, "check%s = 1 ;\n}\n%s=0;\ncount%s = 0 ;\n}\n", temparray1, temparray1, temparray1);
        }
        else if (strcmp(pMain->data, "TOF") == 0)
        {
            if (checkTimer == 1)
            {
                pFileTimer = fopen("Timer.txt", "w");
                fprintf(pFileTimer, "void initTimer(void)\n");
                fprintf(pFileTimer, "{\n");
                checkTimer = 0;
            }
            OutString = AddParenthesesIfMissing(OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            InsertTimer(pMain, &CountTimer, pFileTimer);
            char *equal = " = ";
            char *temparray = "";
            char *temparray1 = "";
            pMain = pMain->next;                                            // T97
            temparray = StrAllocAndAppend(temparray, pMain->data);          // T97
            temparray1 = StrAllocAndAppend(temparray1, pMain->data);        // T97
            pMain->data = StrAllocAndAppend("volatile uint8_t vao", pMain->data);    // int vaoT97
            pMain->data = StrAllocAndAppend(pMain->data, equal);            // int vaoT97 =
            OutString = StrAllocAndAppend(OutString, " ;\n");               // (I0_1&I0_1) ;\n
            OutString = StrAllocAndAppend(pMain->data, OutString);          // int vaoT97 = (I0_1&I0_1) ;\n
            pMain = pMain->next;                                            // 20
            temparray = StrAllocAndAppend("volatile const uint32_t dat", temparray); // int datT97
            temparray = StrAllocAndAppend(temparray, equal);                // int datT97 =
            temparray = StrAllocAndAppend(temparray, pMain->data);          // int datT97 = 20
            OutString = StrAllocAndAppend(OutString, temparray);            // int vaoT97_ON = (I0_1&I0_1) ;\n int datT97_ON = 20
            OutString = StrAllocAndAppend(OutString, " ;\n");
            fputs(OutString, pFile);
            fprintf(pFile, "volatile static uint8_t check%sOn  = 0 ;\nvolatile static uint8_t check%sOff  = 0 ;\n volatile static uint8_t start%s = 1 ;\n ", temparray1, temparray1, temparray1);
            fprintf(pFile, "if ( vao%s  )\n{\ncount%s = 0 ;\ncheck%sOn = 1 ;\n%s =1 ;\n if ( check%sOff == 1 ||  start%s == 1 )\n{\n", temparray1, temparray1, temparray1, temparray1, temparray1, temparray1);
            fprintf(pFile, "xTimerStop(handle_timer[%d], portMAX_DELAY);\ncheck%sOff = 0 ;\nstart%s ==0 \n }\n}\n", CountTimer - 1, temparray1, temparray1);
            fprintf(pFile, " else \n{\ncheck%sOff = 1 ;\nif ( check%sOn == 1 )\n{ \n", temparray1, temparray1);
            fprintf(pFile, "xTimerStart (handle_timer[%d], portMAX_DELAY); \ncheck%sOn = 0  ; \n}\nif ( count%s >= dat%s ) \n{\n%s = 0 ;\n}\n}\n", CountTimer - 1, temparray1, temparray1, temparray1, temparray1);
        }
        else if (strcmp(pMain->data, "TONR") == 0)
        {
            if (checkTimer == 1)
            {
                pFileTimer = fopen("Timer.txt", "w");
                fprintf(pFileTimer, "void initTimer(void)\n");
                fprintf(pFileTimer, "{\n");
                checkTimer = 0;
            }
            OutString = AddParenthesesIfMissing(OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            if (strlen(OutCheckBigBranch) != 0)
            {
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                OutCheckBigBranch = "";
            }
            InsertTimer(pMain, &CountTimer, pFileTimer);
            char *equal = " = ";
            char *temparray = "";
            char *temparray1 = "";
            pMain = pMain->next;                                     // T97
            temparray = StrAllocAndAppend(temparray, pMain->data);   // T97
            temparray1 = StrAllocAndAppend(temparray1, pMain->data); // T97
            fprintf(pFile, "if (!%sReset )\n{\n ", temparray1);
            pMain->data = StrAllocAndAppend("volatile uint8_t vao", pMain->data);    // int vaoT97
            pMain->data = StrAllocAndAppend(pMain->data, equal);            // int vaoT97 =
            OutString = StrAllocAndAppend(OutString, " ;\n");               // (I0_1&I0_1) ;\n
            OutString = StrAllocAndAppend(pMain->data, OutString);          // int vaoT97 = (I0_1&I0_1) ;\n
            pMain = pMain->next;                                            // 20
            temparray = StrAllocAndAppend("volatile const uint32_t dat", temparray); // int datT97
            temparray = StrAllocAndAppend(temparray, equal);                // int datT97 =
            temparray = StrAllocAndAppend(temparray, pMain->data);          // int datT97 = 20
            OutString = StrAllocAndAppend(OutString, temparray);            // int vaoT97_ON = (I0_1&I0_1) ;\n int datT97_ON = 20
            OutString = StrAllocAndAppend(OutString, " ;\n");
            fputs(OutString, pFile);
            fprintf(pFile, "volatile static uint8_t  check%sOn = 0 ;\nvolatile static uint8_t check%sOff = 0 ;\nvolatile static uint32_t checkCount%s = 0 ;\nvolatile static uint8_t start%s = 0 ;\n", temparray1, temparray1, temparray1, temparray1);
            fprintf(pFile, "if ( vao%s ) \n{\ncheck%sOn = 1 ;\nif ( check%sOff == 1 || start%s == 1 )\n", temparray1, temparray1, temparray1, temparray1);
            fprintf(pFile, "{ \nxTimerStart (handle_timer[%d], portMAX_DELAY); \ncheck%sOff = 0 ; \nstart%s = 0 ;\n}\n", CountTimer - 1, temparray1, temparray1);
            fprintf(pFile, "if ( (count%s-checkCount%s) >= dat%s) \n{%s = 1 ;\ncheckCount%s = 0 ;\nxTimerStop(handle_timer[%d], portMAX_DELAY);\ncount%s = 0 ;\n}\n}\n", temparray1, temparray1, temparray1, temparray1, temparray1, CountTimer - 1, temparray1);
            fprintf(pFile, "else\n{check%sOff = 1 ;\nif( (!count%s) && (check%sOn == 1 )  )\ncheckCount%s = count%s ;\ncheck%sOn = 0 ;\n}\n", temparray1, temparray1, temparray1, temparray1, temparray1, temparray1);
            fprintf(pFile, "\n}\n");
            fprintf(pFile, "else\n{\nxTimerStop(handle_timer[%d], portMAX_DELAY);\ncount%s = 0;\n%s = 0 ;}\n", CountTimer - 1, temparray1, temparray1);
        }
        else if (strcmp(pMain->data, "MOVB") == 0)
        {
            InsertMov(&pMain, OutString, CountQuestionMark, pFile, MOVB_CHECK);
            continue;
        }
        else if (strcmp(pMain->data, "MOVW") == 0)
        {
            InsertMov(&pMain, OutString, CountQuestionMark, pFile, MOVW_CHECK);
            continue;
        }
        else if (strcmp(pMain->data, "MOVDW") == 0)
        {
            InsertMov(&pMain, OutString, CountQuestionMark, pFile, MOVDW_CHECK);
            continue;
        }
        else if (strcmp(pMain->data, "R") == 0)
        {
            OutString = AddParenthesesIfMissing(OutString);
            OutString = StrAllocAndAppend(OutString, " ;\n");
            pMain = pMain->next;
            if (strncmp(pMain->data, "T", 1) == 0)
            {
                fprintf(pFile, "%sreset = %s", pMain->data, OutString);
                fprintf(pFile, "if (%sreset>0)\n{\n%sreset = 1;\n}\nelse\n{\n%sreset = 0 ;\n}\n", pMain->data, pMain->data, pMain->data);
            }
            pMain = pMain->next;
        }
        else if ((strncmp(pMain->data, "AW", 2) == 0) || (strncmp(pMain->data, "OW", 2) == 0) || (strncmp(pMain->data, "LDW", 2) == 0)) // AW=
        {
            char *token;
            pNext = pMain->next;
            pNext1 = pNext->next;
            token = strtok(pMain->data, "W"); // pMain : A
            token = strtok(NULL, "W");        // token : =
            if (strcmp(token, "=") == 0)
            {
                char *temp = "";
                temp = StrAllocAndAppend(temp, pNext->data);
                fprintf(pFile, "volatile uint8_t temp%s = 0 ;\n", pNext->data);
                fprintf(pFile, "if (count%s == %s )\n{\n", pNext->data, pNext1->data);
                fprintf(pFile, "temp%s = 1 ;\n}\n", pNext->data);
                // free(pNext->data);
                sprintf(pNext->data, "temp%s", temp);
                DeleteLinkList(&(pNext1), &(pNext1->prev), &(pNext1->next));
            }
            else if (strcmp(token, "<>") == 0)
            {
                char *temp = "";
                temp = StrAllocAndAppend(temp, pNext->data);
                fprintf(pFile, "volatile uint8_t temp%s = 0 ;\n", pNext->data);
                fprintf(pFile, "if (count%s !=%s )\n{\n", pNext->data, pNext1->data);
                fprintf(pFile, "temp%s = 1 ;\n}\n", pNext->data);
                // free(pNext->data);
                sprintf(pNext->data, "temp%s", temp);
                DeleteLinkList(&(pNext1), &(pNext1->prev), &(pNext1->next));
            }
            else
            {
                char *temp = "";
                temp = StrAllocAndAppend(temp, pNext->data);
                fprintf(pFile, "volatile uint8_t temp%s = 0 ;\n", pNext->data);
                fprintf(pFile, "if (count%s %s %s )\n{\n", pNext->data, token, pNext1->data);
                fprintf(pFile, "temp%s = 1 ;\n}\n", pNext->data);
                // free(pNext->data);
                sprintf(pNext->data, "temp%s", temp);
                DeleteLinkList(&(pNext1), &(pNext1->prev), &(pNext1->next));
            }

            continue;
        }

        pMain = pMain->next;
    }
    fprintf(pFile," xTaskNotifyWait(0,0,NULL,pdMS_TO_TICKS(%d));\n",SCANCYCLE);
    fprintf(pFile, "\n}\n}\n");
    fprintf(pFileTimer, "\n}\n");
    fclose(pFileTimer);
}

void FileDefineData(void)
{
    FILE *pFile = NULL;
    pFile = fopen("DataPLCh.txt", "w");
    // ghi tên file vào để tránh trùng lặp ở file khác
    if (!pFile)
        printf("Create file DataPLC.h failed \n");
    fprintf(pFile, "#ifndef INC_CSDL_H_\n#define INC_CSDL_H_\n");

    fprintf(pFile, "\n#include\"main.h\"\n");

    DefineRegionMemory(pFile, I_MEM, SUM_I);

    DefineRegionMemory(pFile, Q_MEM, SUM_Q);

    DefineRegionMemory(pFile, M_MEM, SUM_M);

    DefineIO(pFile, I_MEM, 2);

    DefineIO(pFile, Q_MEM, 2);

    fprintf(pFile, "\nvoid read_Pin_Input(void);\n");
    fprintf(pFile, "void write_Pin_Output(void);\n");
    fprintf(pFile, "void Main_task(void *param) ;\n");

    fprintf(pFile, "\n#endif /*INC_CSDL_H_*/");

    fclose(pFile);
}

void FileData(FILE *pFile)
{
    pFile = fopen("DataPLCc.txt", "w");
    if (!pFile)
        printf("Create file  DataPLC.c failed \n");
    fprintf(pFile, "#include\"DataPLC.h\"\n\n");

    fprintf(pFile, "volatile static uint8_t I[2][8]={};\nvolatile static uint8_t Q[2][8]={};\nvolatile static uint8_t M[10][2]={};\n");

    fprintf(pFile, "\nvoid read_Pin_Input()\n");
    readInputPin(pFile, SUM_I);

    fprintf(pFile, "void write_Pin_Output()\n");
    writeOutputPin(pFile, SUM_Q);
    fprintf(pFile, "void Main_task( void *param)\n");
    fclose(pFile);
}
