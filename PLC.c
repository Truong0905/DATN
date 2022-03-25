#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

FILE *ConvFrTxtFile(uint32_t *row); // Tạo và đọc địa chỉ file TXT đã xóa comment
struct Link
{
    char *data;
    struct Link *next;
    struct Link *prev;

}; // Cấu trúc 1 phần tử
typedef struct Link LinkList;

LinkList *First, *Last, *First_F, *Last_F;

LinkList *GetLink(char *data)
{
    LinkList *p = (LinkList *)malloc(sizeof(struct Link));
    strcpy(p->data, data);
    return p;
}

FILE *ConvFrTxtFile(uint32_t *Row_Of_File)
{
    FILE *fp = NULL, *fp1 = NULL;

    char arr[20];

    fp1 = fopen("PLC.txt", "w");
    // Mở file hàm fopen
    fp = fopen("PLC.awl", "r");
    if (fp == NULL)
    {
        printf("Cannot open file");
        return NULL;
    }

    //Đọc từng dòng từ file cho tới khi gặp NULL
    fp = fopen("PLC.awl", "r");
    while (fgets(arr, 128, fp) != NULL)
    {
        *Row_Of_File = *Row_Of_File + 1;

        if (*Row_Of_File > 5)
        {

            if (strncmp(arr, "Network", 7) == 0)
            {
                fputs("N\n", fp1);
            }
            else if (strncmp(arr, "END", 3) == 0)
            {
                fputs("N\n", fp1);
                break;
            }
            else
            {
                fputs(arr, fp1);
            }
        }
    }
    *Row_Of_File = *Row_Of_File - 5;

    fclose(fp);
    fclose(fp1);
    return fopen("PLC.txt", "r");
}

void ConvFrList(FILE *fopen, uint32_t Row_Of_File)
{
    LinkList *p, *q;
    char arr[20];
    First = NULL;

    for (int i = 0; i < Row_Of_File; i++)
    {
        fgets(arr, 20, fopen);
        for (int k = 0; k < 20; k++)
        {
            p = (LinkList *)malloc(sizeof(LinkList));
            p->next = NULL;
            p->data = (char *)calloc(20, sizeof(char));
            for (int x = 0; x < 20; x++)
            {

                if (arr[k] == ',')
                {
                    k++;
                    x = x - 1;
                    continue;
                }
                if (arr[k] == '.')
                {
                    (p->data)[x] = '_';
                    k++;
                    continue;
                }

                if (arr[k] == '\n')
                {

                    break;
                }
                (p->data)[x] = arr[k];
                k++;
            }
            if (First == NULL)
            {
                p->prev = NULL;
                First = p;
                Last = p;
            }
            else
            {
                q = Last;
                q->next = p;
                p->prev = q;
                Last = p;
            }
            if (arr[k] == '\n')
            {
                break;
            }
        }
    }
}

void ListFinal(void)
{
    LinkList *p, *q, *k;
    p = First;
    First_F = NULL;
    int a = 0; // Đếm size token
    int b = 0; // Xác định tiếp điểm thường đóng , sườn lên , sườn xuống
    while (p != NULL)
    {
        char *token = strtok(p->data, " ");
        while (token != NULL)
        {
            a = strlen(token);

            q = (LinkList *)malloc(sizeof(LinkList));

            q->next = NULL;
            if (b == 0)
            {
                q->data = (char *)calloc(a, sizeof(char));
                strcpy(q->data, token);
            }
            else
            {
                q->data = (char *)calloc(a + 1, sizeof(char));
                for (int i = 0; i < a; i++)
                {
                    (q->data)[i] = token[i];
                }
                (q->data)[a] = 'H';
                b = 0;
            }

            if (First_F == NULL)
            {
                q->prev = NULL;
                First_F = q;
                Last_F = q;
            }
            else
            {
                k = Last_F;
                k->next = q;
                q->prev = k;
                Last_F = q;
            }

            if (strncmp(token, "LDN", 3) == 0 || strncmp(token, "AN", 2) == 0 || strncmp(token, "ON", 2) == 0)
            {
                b = 1;
            }
            if (strncmp(token, "EU", 2) == 0)
            {
                LinkList *r1, *r2, *r3;
                r1 = q->prev;
                r2 = r1->prev; // r1 = i0.0 => r2 = ld
                if (r2->prev == NULL)
                {
                    r3 = (LinkList *)malloc(sizeof(LinkList));
                    r3->next = r1->next;
                    r3->prev = r1->prev;
                    r2->next = r3;
                    r3->data = (char *)calloc(a + 1, sizeof(char));
                    for (int i = 0; i < a; i++)
                    {
                        (r3->data)[i] = (r1->data)[i];
                    }
                    (r3->data)[a] = 's';
                    (r3->data)[a + 1] = 'l';
                    free(r1);
                }
                else
                {
                    r1 = r2->prev;
                    r2 = r1->prev;
                    r3 = (LinkList *)malloc(sizeof(LinkList));
                    r3->next = r1->next;
                    r3->prev = r1->prev;
                    r2->next = r3;
                    r3->data = (char *)calloc(a + 1, sizeof(char));
                    for (int i = 0; i < a; i++)
                    {
                        (r3->data)[i] = (r1->data)[i];
                    }
                    (r3->data)[a] = 's';
                    (r3->data)[a + 1] = 'l';
                    free(r1);
                }
            }
            if (strncmp(token, "ED", 2) == 0)
            {
                LinkList *r1, *r2, *r3;
                r1 = q->prev;
                r2 = r1->prev;

                if (r2->prev == NULL)
                {
                    r3 = (LinkList *)malloc(sizeof(LinkList));
                    r3->next = r1->next;
                    r3->prev = r1->prev;
                    r2->next = r3;
                    r3->data = (char *)calloc(a + 1, sizeof(char));
                    for (int i = 0; i < a; i++)
                    {
                        (r3->data)[i] = (r1->data)[i];
                    }
                    (r3->data)[a] = 's';
                    (r3->data)[a + 1] = 'l';
                    free(r1);
                }
                else
                {
                    r1 = r2->prev;
                    r2 = r1->prev;
                    r3 = (LinkList *)malloc(sizeof(LinkList));
                    r3->next = r1->next;
                    r3->prev = r1->prev;
                    r2->next = r3;
                    r3->data = (char *)calloc(a + 1, sizeof(char));
                    for (int i = 0; i < a; i++)
                    {
                        (r3->data)[i] = (r1->data)[i];
                    }
                    (r3->data)[a] = 's';
                    (r3->data)[a + 1] = 'x';
                    free(r1);
                }
            }

            token = strtok(NULL, " ");
        }
        p = p->next;
    }
}

void Branch(FILE *listxl)
{
    int numNP = 0;
    int numNT = 0;
    LinkList *p = First_F;

    while (p != NULL)
    {
        if (strcmp(p->data, "ALD") == 0 || strcmp(p->data, "OLD") == 0 || strcmp(p->data, "TON") == 0 || strcmp(p->data, "CTU") == 0)
        {
            if (numNT > numNP)
            {
                fprintf(listxl, "'%c'", ')');
                fprintf(listxl, "%c", ',');
                fprintf(listxl, "'%s'", p->data);
                fprintf(listxl, "%c", ',');
                numNP++;
            }
            else
            {
                fprintf(listxl, "'%s'", p->data);
            }
        }
        else if (strcmp(p->data, "=") == 0)
        {
            if (numNT > numNP)
            {
                 fprintf(listxl, "'%c'", ')');
                fprintf(listxl, "%c", ',');
                
                fprintf(listxl, "'%s'", p->data);
                fprintf(listxl, "%c", ',');
            }
            else
            {
                fprintf(listxl, "'%s'", p->data);
            }
        }
        else if (strcmp(p->data, "LD") == 0 || strcmp(p->data, "LDN") == 0)
        {

            if (p == First_F || strcmp(p->prev->data, "N") == 0 || strcmp(p->prev->data, "ALD") == 0 || strcmp(p->prev->data, "OLD") == 0 || strcmp(p->prev->prev->data, "=") == 0)
            {
                fprintf(listxl, "'%c'", '(');
                fprintf(listxl, "'%s'", p->data);
                numNT++;
            }
            else
            {
                if (numNT == numNP)
                {
                    fprintf(listxl, "'%c'", '(');
                    fprintf(listxl, "'%s'", p->data);
                    numNT++;
                }
                else
                {
                    fprintf(listxl, "'%c'", ')');
                    fprintf(listxl, "'%c'", '(');
                    fprintf(listxl, "'%s'", p->data);
                    numNP++;
                    numNT++;
                }
            }
        }
        else
        {
            fprintf(listxl, "'%s'", p->data);
        }

        p = p->next;
    }
}


int main(void)
{
    uint32_t row = 0U;
    FILE *fptest = ConvFrTxtFile(&row);
    FILE *listxl = fopen("listxl.txt", "w");

    if (listxl == NULL)
    {
        printf("open file not succesfully");
        return -1;
    }

    ConvFrList(fptest, row);
    ListFinal();
    Branch(listxl);

    fclose(listxl);
    fclose(fptest);
    return 0;
}