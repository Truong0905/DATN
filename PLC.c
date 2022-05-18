#include "STL.h"
int main(void)
{
    FILE *fptest = NULL;
    LinkList *p;
    int row = 0U;
    fptest = ReadTextFile(&row);
    TransferToList(fptest, row);
    FinalList();
    SplitBranchesWithFirstFinalPointer();
    p = FirstFinal;
    while (p != NULL)
    {
        printf("%s ", p->data);
        p = p->next;
    }
    FinalTextFile();

    fclose(fptest);
    return 0;
}
FILE *ReadTextFile(int *RowOfFile)
{
    FILE *pfile = NULL, *pFileFinal = NULL;

    char TempArray[20];
    int count;
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
            else // Nếu là tiếp điểm thường  đóng thì tăng size dữ liệu lên 1 nữa để lưu Token và ký tự 'H'
            {

                char *TempArray = "H";
                pMainOfFirstFinal->data = (char *)calloc(SizeOfToken, sizeof(char));
                strcpy(pMainOfFirstFinal->data, token);
                pMainOfFirstFinal->data = StrAllocAndAppend(pMainOfFirstFinal->data, TempArray);
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
    LinkList *pCheck, *pMain, *pTemp ;
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
            if ((strcmp(pCheck->data, "ALD") == 0) || (strcmp(pCheck->data, "OLD") == 0) || (strcmp(pCheck->data, "TON") == 0) || (strcmp(pCheck->data, "CTU") == 0) || (strcmp(pCheck->data, "CTD") == 0) || (strcmp(pCheck->data, "CTUD") == 0))
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
            else if ((strcmp(pCheck->data, "A") == 0) || (strcmp(pCheck->data, "O") == 0) || (strcmp(pCheck->data, "AN") == 0) || (strcmp(pCheck->data, "ON") == 0))
            {
                pNext = pCheck->next;
                pNext1 = pNext->next;
                pPrev = pCheck->prev;

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
                            char *arr_NEXT1 = "T_";
                            pNext1->data = StrAllocAndAppend(arr_NEXT1, pNext1->data);
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

void FinalTextFile(void)
{
    LinkList *pMain, *pNext, *pNext1, *pPrev, *pPrev1;
    FILE *pFile;
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
    pFile = fopen("PLC_F.txt", "w");
    pMain = FirstFinal;
    while (pMain != NULL)
    {

        if (CheckEndNetWork == 0) // Chưa kết thúc 1 network
        {
            OutString = "";
            CheckEndNetWork = 1;
        }
        if (strcmp(pMain->data, "N") == 0)
        {
            CheckEndNetWork = 0;
            char *OUTtemp_H = ""; // Kiểm tra xem có nhánh lớn trước đó không
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
                    char *TempArray = "!";
                    OutString = StrAllocAndAppend(OutString, TempArray);
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                    pPrev = pPrev1->prev;
                    if (pPrev != NULL)
                    {
                        while ((strncmp(pPrev->data, "not", 3) == 0) || (strncmp(pPrev->data, "1no", 3) == 0))
                        {
                            OutString = StrAllocAndAppend(OutString, TempArray);
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
           CheckNOT(&pMain,pNext,pNext1,&OutString,InsertMul) ;

            while ((strcmp(pMain->data, "A") == 0) || (strcmp(pMain->data, "AN") == 0))
            {

                pNext = pMain->next;
                pNext1 = pNext->next;
                CheckNOT(&pMain,pNext,pNext1,&OutString,InsertMul) ;
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
            CheckNOT(&pMain,pNext,pNext1,&OutString,Insertplus) ;
            while ((strcmp(pMain->data, "O") == 0) || (strcmp(pMain->data, "ON") == 0))
            {

                pNext = pMain->next;
                pNext1 = pNext->next;
                CheckNOT(&pMain,pNext,pNext1,&OutString,Insertplus) ;
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
                if ((strcmp(pPrev1->data, "A") == 0) || (strcmp(pPrev1->data, "AN") == 0))
                {
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                    OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
                    OutString = StrAllocAndAppend(InsertOpeningBracket, OutString);
                }
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
            CheckCountQuestionMark(&CountQuestionMark, &CheckBigBranch, OutString, OutCheckBigBranch);
            AddPlusToOutString(&pMain,&OutString,&CountQuestionMark,InsertMul);
            if (strncmp(pMain->data, "(", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;

                continue;
            }
            pNext = pMain->next;
            pNext1 = pNext->next;
            if ((CheckBigBranch == 1) && (((strncmp(pNext1->data, "OLD", 1) == 0) || (strncmp(pNext1->data, "ALD", 1) == 0)) || ((strncmp(pNext->data, "OLD", 1) == 0) || (strncmp(pNext->data, "ALD", 1) == 0))))
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
            CheckCountQuestionMark(&CountQuestionMark, &CheckBigBranch, OutString, OutCheckBigBranch);
            AddPlusToOutString(&pMain,&OutString,&CountQuestionMark,Insertplus);

            if (strncmp(pMain->data, "(", 1) == 0)
            {
                OutString = StrAllocAndAppend(OutString, InsertQuestionMark);
                CountQuestionMark++;
                OutString = StrAllocAndAppend(OutString, InsertOpeningBracket);
                pMain = pMain->next;

                continue;
            }
            pNext = pMain->next;
            pNext1 = pNext->next;
            if ((CheckBigBranch == 1) && (((strncmp(pNext1->data, "OLD", 1) == 0) || (strncmp(pNext1->data, "ALD", 1) == 0)) || ((strncmp(pNext->data, "OLD", 1) == 0) || (strncmp(pNext->data, "ALD", 1) == 0))))
            {
                OutString = AddParenthesesIfMissing(OutString);
                OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
                CheckBigBranch = 0;
                CountQuestionMark++;
                OutCheckBigBranch = " ";
            }
            continue;
        }
        else if (strcmp(pMain->data, "T_NOT") == 0)
        {
            char *arr_temp = "!";
            OutString = StrAllocAndAppend(InsertOpeningBracket, OutString);
            OutString = StrAllocAndAppend(arr_temp, OutString);
            OutString = StrAllocAndAppend(OutString, InsertClosingBracket);
            pMain = pMain->next;
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
            char *arr3 = " ;\n";
            OutString = StrAllocAndAppend(OutString, arr3);
            OutString = StrAllocAndAppend(pMain->data, OutString);
            pMain = pMain->next;
            OutString = StrAllocAndAppend(pMain->data, OutString);
            fputs(OutString, pFile);
        }
        else if (strcmp(pMain->data, "CTU") == 0)
        {
            // do some thing
            char *arr1 = "(";
            char *arr2 = ")";
            OutString = StrAllocAndAppend(arr1, OutString);
            OutString = StrAllocAndAppend(OutString, arr2);
            char *arr3 = "\n";
            OutString = StrAllocAndAppend(OutString, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(pMain->data, "CTD") == 0)
        {
            OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            free(OutCheckBigBranch);
            OutCheckBigBranch = (char *)calloc(1, sizeof(char));
            OutCheckBigBranch = "";
            // do some thing
            // do some thing
            char *arr3 = "\n";
            OutString = StrAllocAndAppend(OutString, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(pMain->data, "CTUD") == 0)
        {

            OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            free(OutCheckBigBranch);
            OutCheckBigBranch = (char *)calloc(1, sizeof(char));
            OutCheckBigBranch = "";
            // do some thing
            // do some thing
            char *arr3 = "\n";
            OutString = StrAllocAndAppend(OutString, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(pMain->data, "TON") == 0)
        {
            OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            free(OutCheckBigBranch);
            OutCheckBigBranch = (char *)calloc(1, sizeof(char));
            OutCheckBigBranch = "";
            // do some thing
            // do some thing
            char *arr3 = "\n";
            OutString = StrAllocAndAppend(OutString, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(pMain->data, "TOF") == 0)
        {
            OutString = StrAllocAndAppend(OutCheckBigBranch, OutString);
            CheckBigBranch = 0;
            CountQuestionMark = 0;
            free(OutCheckBigBranch);
            OutCheckBigBranch = (char *)calloc(1, sizeof(char));
            OutCheckBigBranch = "";
            // do some thing
            // do some thing
            char *arr3 = "\n";
            OutString = StrAllocAndAppend(OutString, arr3); // OUT : = (a*b)\n
        }
        pMain = pMain->next;
    }
    fclose(pFile);
}