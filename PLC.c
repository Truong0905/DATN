#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// Hello git ABC
FILE *ConvFrTxtFile(int *row); // Tạo và đọc địa chỉ file TXT đã xóa comment
struct Link
{
    char *data;
    struct Link *next;
    struct Link *prev;

}; // Cấu trúc 1 phần tử
typedef struct Link LinkList;
LinkList *First, *Last, *First_F, *Last_F;     // First và Last dùng cho  ConvFrList ()   ; First_F , Last_F  là chuỗi cuối cùng cần tìm
void ConvFrList(FILE *fopen, int Row_Of_File); // Chuyển về List 1 gồm tập hợp các ký tự có loại bỏ các ký tự ko cần thiết
void ListFinal(void);                          // Gom các ký tự câu lệnh (biến) vào 1 data , bổ sung H vào biến thường đóng , bổ xung "sl" nếu là suòn lên hoặc "sx" nếu là sườn xuống
void BranchWithFirst_F(void);                  // Tách nhánh
void Final_File_text(void);
char *str_alloc_and_insert(const char *str1, const char *str2);
int main(void)
{
    FILE *fptest = NULL;
    LinkList *p;
    int row = 0U;
    fptest = ConvFrTxtFile(&row);
    ConvFrList(fptest, row);
    ListFinal();
    BranchWithFirst_F();
    Final_File_text();
    p = First_F;
    while (p != NULL)
    {
        printf("%s ", p->data);
        p = p->next;
    }
    fclose(fptest);
    return 0;
}
FILE *ConvFrTxtFile(int *Row_Of_File)
{
    FILE *fp = NULL, *fp1 = NULL;

    char arr[20];
    int count;
    int check = 0;
    int redu = 0;
    fp1 = fopen("PLC.txt", "w");
    // Mở file bằn hàm fopen
    fp = fopen("PLC.awl", "r");
    if (fp == NULL)
    {
        printf("Can not open file");
        return NULL;
    }

    //Đọc từng dòng từ file cho tới khi gặp NULL
    while (fgets(arr, 128, fp) != NULL)
    {
        count = count + 1;

        if (check == 1)
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
        if (strncmp(arr, "Network 1", 9) == 0)
        {
            check = 1;
            redu = count;
        }
    }
    *Row_Of_File = count - redu;

    fclose(fp);
    fclose(fp1);
    return fopen("PLC.txt", "r");
}

void ConvFrList(FILE *fopen, int Row_Of_File)
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
    int b = 0; // Xác định tiếp điểm thường đóng
    while (p != NULL)
    {
        char *token = strtok(p->data, " ");
        while (token != NULL)
        {
            // tính kịc thước token để tạo vùng nhớ cho  dữ liệu của phần tử mới
            a = strlen(token);
            // cấp phát vùng nhớ cho phần tử
            q = (LinkList *)malloc(sizeof(LinkList));

            q->next = NULL;
            if (b == 0) // Nếu ko phải là  tiếp điểm thường đóng  ta copy token vào phần tử
            {
                q->data = (char *)calloc(a, sizeof(char));
                strcpy(q->data, token);
            }
            else // Nếu là tiếp điểm thuoơngf đóng thì tăng size dữ liệu lên 1 nữa để lưu Token và ký tự 'H'
            {

                char *arr = "H";
                char *arr1 = str_alloc_and_insert(token, arr);
                int size = strlen(arr1);
                q->data = (char *)calloc(size, sizeof(char));
                for (int i = 0; i <= size; i++)
                {
                    (q->data)[i] = arr1[i];
                }
                b = 0;
            }
            /// Lưu địa chỉ con trỏ First_ và con trỏ Last_F
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
            // xác định xem có phải tiếp điểm thường đóng không bằng cách kiểm tra câu lệnh là sau đó gán b=1 để lần sau thêm 'H'
            if (strncmp(token, "LDN", 3) == 0 || strncmp(token, "AN", 2) == 0 || strncmp(token, "ON", 2) == 0)
            {
                b = 1;
            }

            // Nếu là xung sườn lên ta thêm "sl" vào sau biến bằng cách tạo 1 phần tử mới thế vào vị trí của phần tử cũ và xóa phần tử cũ đi
            if (strncmp(token, "EU", 2) == 0)
            {
                LinkList *r1, *r2, *r3;
                r1 = q->prev;
                r2 = r1->prev; // r1 = i0.0 => r2 = ld
                char *arr1 = "sl";
                char *arr2 = str_alloc_and_insert(r1->data, arr1);
                int size = strlen(arr2);
                r3 = (LinkList *)malloc(sizeof(LinkList));
                // Thay r3 vào r1
                r3->next = r1->next;
                r3->prev = r1->prev;
                r2->next = r3;
                r3->data = (char *)calloc(size, sizeof(char));
                for (int i = 0; i <= size; i++)
                {
                    (r3->data)[i] = arr2[i];
                }
                free(r1);
            }
            // Nếu là xung sườn xuống  ta thêm "sx" vào sau biến bằng cách tạo 1 phần tử mới thế vào vị trí của phần tử cũ và xóa phần tử cũ đi
            if (strncmp(token, "ED", 2) == 0)
            {
                LinkList *r1, *r2, *r3;
                r1 = q->prev;
                r2 = r1->prev; // r1 = i0.0 => r2 = ld
                char *arr1 = "sx";
                char *arr2 = str_alloc_and_insert(r1->data, arr1);
                int size = strlen(arr2);
                r3 = (LinkList *)malloc(sizeof(LinkList));
                // Thay r3 vào r1
                r3->next = r1->next;
                r3->prev = r1->prev;
                r2->next = r3;
                r3->data = (char *)calloc(size, sizeof(char));
                for (int i = 0; i <= size; i++)
                {
                    (r3->data)[i] = arr2[i];
                }
                free(r1);
            }

            token = strtok(NULL, " ");
        }
        p = p->next;
    }
}

void BranchWithFirst_F(void)
{
    LinkList *p, *k, *r, *z; // p dungf duyệt danh sách , q là biến tạm thời lưu dữ liệu
    int cout1 = 0;           // Đếm số "("
    int cout2 = 0;           // Đếm số ")"
    p = First_F;
    while (p != NULL)
    {
        if (p == First_F) // Thêm ký tự '(' vào đầu
        {
            k = (LinkList *)malloc(sizeof(LinkList));
            k->data = (char *)calloc(1, sizeof(char));
            k->data[0] = '(';
            cout1++;
            k->next = p;
            p->prev = k;
            k->prev = NULL;
            First_F = k;
        }
        else
        {
            if ((strcmp(p->data, "ALD") == 0) || (strcmp(p->data, "OLD") == 0) || (strcmp(p->data, "TON") == 0) || (strcmp(p->data, "CTU") == 0) || (strcmp(p->data, "CTD") == 0) || (strcmp(p->data, "CTUD") == 0))
            {
                r = p->prev;
                k = (LinkList *)malloc(sizeof(LinkList));
                k->data = (char *)calloc(1, sizeof(char));
                k->data[0] = ')';
                cout2++;
                r->next = k;
                k->prev = r;
                k->next = p;
                p->prev = k;
            }
            else if ((strcmp(p->data, "=") == 0))
            {
                if (cout1 > cout2)
                {
                    r = p->prev;
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(1, sizeof(char));
                    k->data[0] = ')';
                    cout2++;
                    r->next = k;
                    k->prev = r;
                    k->next = p;
                    p->prev = k;
                }
            }
            else if ((strncmp(p->data, "LD", 2) == 0))
            {
                r = p->prev;
                z = r->prev;
                if ((strcmp(z->data, "=") == 0) || (strcmp(r->data, "N") == 0) || (strcmp(r->data, "ALD") == 0) || (strcmp(r->data, "OLD") == 0))
                {
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(1, sizeof(char));
                    k->data[0] = '(';
                    cout1++;
                    r->next = k;
                    k->prev = r;
                    k->next = p;
                    p->prev = k;
                }
                else
                {
                    if (cout1 == cout2)
                    {
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(1, sizeof(char));
                        k->data[0] = '(';
                        cout1++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;
                    }
                    else
                    {
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(1, sizeof(char));
                        k->data[0] = ')';
                        cout1++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;

                        r = p->prev;
                        z = r->prev;
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(1, sizeof(char));
                        k->data[0] = '(';
                        cout2++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;
                    }
                }
            }
            else if ((strncmp(p->data, "MOV", 3) == 0))
            {
                r = p->prev;
                if ((strcmp(r->data, "ALD") != 0) && (strcmp(r->data, "OLD") != 0) && (strcmp(r->data, "AENO") != 0))
                {
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(1, sizeof(char));
                    k->data[0] = ')';
                    cout2++;
                    r->next = k;
                    k->prev = r;
                    k->next = p;
                    p->prev = k;
                }
            }
        }
        p = p->next;
    }
}

void Final_File_text(void)
{
    LinkList *p, *pNEXT, *pNEXT1, *pPREV, *pPREV1;
    FILE *pFile;
    char *OUT;
    char *OUT_pre;
    int check = 0; // Kiểm tra xem hết 1 network chưa
    int count = 0; // Đếm số dấu "?"
    pFile = fopen("PLC_F.txt", "w");
    p = First_F;
    while (p != NULL)
    {
        if (count > 1)
        {
            char *OUTtemp = "";
            char *token = strtok(OUT, "?"); /// (...)@((.....)?(......))
            int a = strlen(token);
            char *arr = "@";
            char *arr1 = str_alloc_and_insert(token, arr);
            OUT_pre = str_alloc_and_insert(OUT_pre, arr1); // (....)@
            token = strtok(NULL, " ");
            OUTtemp = str_alloc_and_insert(OUTtemp, token);
            free(OUT);
            OUT = (char *)calloc(strlen(OUTtemp), sizeof(char));
            strcpy(OUT, OUTtemp);
            count = 1;
        }
        if (check == 0) // Chưa kết thúc 1 network
        {
            OUT = "";
            check = 1;
        }
        if (strcmp(p->data, "N") == 0)
        {
            fputs(OUT, pFile);
            check = 0;
            p = p->next;
            continue;
        }

        if (strncmp(p->data, "(", 1) == 0)
        {
            pNEXT = p->prev;
            if (pNEXT != NULL) // Nếu trước đó có dấu ")" thì thêm "?" vào giữa 2 dấu
            {
                int size_of_OUT = sizeof(OUT);

                if (strncmp(pNEXT->data, ")", 1) == 0)
                {
                    char *arr = "(";
                    OUT = str_alloc_and_insert(OUT, arr);
                    char *arr2 = "?";
                    count++;
                    OUT = str_alloc_and_insert(OUT, arr2);
                    char *arr1 = "(";
                    OUT = str_alloc_and_insert(OUT, arr1);
                    p = p->next;
                    continue;
                }
                else
                {
                    char *arr = "(";
                    OUT = str_alloc_and_insert(OUT, arr);
                }
            }
            else
            {
                char *arr = "(";
                OUT = str_alloc_and_insert(OUT, arr);
            }
        }
        else if (strncmp(p->data, ")", 1) == 0)
        {

            pNEXT = p->next;
            if (strncmp(pNEXT->data, "(", 1) == 0) // Nếu trước đó có dấu "(" thì thêm "?" vào giữa 2 dấu
            {
                char *arr = ")";
                OUT = str_alloc_and_insert(OUT, arr);
                char *arr2 = "?";
                count++;
                OUT = str_alloc_and_insert(OUT, arr2);
                char *arr1 = "(";
                OUT = str_alloc_and_insert(OUT, arr1);
                p = pNEXT->next;
                continue;
            }
            else
            {
                char *arr = ")";
                OUT = str_alloc_and_insert(OUT, arr);
            }
        }
        else if ((strcmp(p->data, "LD") == 0) || (strcmp(p->data, "LDN") == 0))
        {
            pNEXT = p->next;
            OUT = str_alloc_and_insert(OUT, pNEXT->data);
            pNEXT1 = pNEXT->next;
            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
                char *arr1 = "!";
                OUT = str_alloc_and_insert(arr1, OUT);
                char *arr2 = ")";
                OUT = str_alloc_and_insert(OUT, arr2);
                pNEXT = pNEXT1->next;
                p = pNEXT;
                continue;
            }

            p = pNEXT1;
            continue;
        }
        else if ((strcmp(p->data, "EU") == 0) || (strcmp(p->data, "ED") == 0))
        {
            p = p->next;
            continue;
        }
        else if ((strcmp(p->data, "A") == 0) || (strcmp(p->data, "AN") == 0))
        {
            char *arr = "*";
            char *arr_temp = "(";
            char *arr_temp1 = ")";
            pPREV = p->prev;
            pPREV1 = pPREV->prev;
            if ((strcmp(pPREV->data, "ALD") == 0) || (strcmp(pPREV->data, "OLD") == 0) || (strcmp(pPREV1->data, "A") == 0))
            {

                pNEXT->data = str_alloc_and_insert(arr_temp, pNEXT->data);
                pNEXT->data = str_alloc_and_insert(pNEXT->data, arr_temp1);
            }
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
                char *arr1 = "!(";
                char *arr_temp = str_alloc_and_insert(arr1, pNEXT->data);
                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, arr_temp);
                char *arr2 = ")";
                OUT = str_alloc_and_insert(OUT, arr2);
                pNEXT = pNEXT1;
                p = pNEXT->next;
            }
            else if (strcmp(pNEXT1->data, "NOT") != 0)
            {
                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }

            while ((strcmp(p->data, "A") == 0) || (strcmp(p->data, "AN") == 0))
            {
                char *arr = "*";

                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                pNEXT->data = str_alloc_and_insert(arr_temp, pNEXT->data);
                pNEXT->data = str_alloc_and_insert(pNEXT->data, arr_temp1);
                if (strcmp(pNEXT1->data, "NOT") == 0)
                {
                    char *arr1 = "!(";
                    char *arr_temp = str_alloc_and_insert(arr1, pNEXT->data);
                    OUT = str_alloc_and_insert(OUT, arr);
                    OUT = str_alloc_and_insert(OUT, arr_temp);
                    char *arr2 = ")";
                    OUT = str_alloc_and_insert(OUT, arr2);
                    pNEXT = pNEXT1;
                    p = pNEXT->next;
                    continue;
                }

                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            continue;
        }
        else if ((strcmp(p->data, "O") == 0) || (strcmp(p->data, "ON") == 0))
        {
            char *arr = "*";
            char *arr_temp = "(";
            char *arr_temp1 = ")";
            pPREV = p->prev;
            pPREV1 = pPREV->prev;
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            if ((strcmp(pPREV->data, "ALD") == 0) || (strcmp(pPREV->data, "OLD") == 0) || (strcmp(pPREV1->data, "O") == 0))
            {

                pNEXT->data = str_alloc_and_insert(arr_temp, pNEXT->data);
                pNEXT->data = str_alloc_and_insert(pNEXT->data, arr_temp1);
            }

            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
                char *arr1 = "!(";
                char *arr_temp = str_alloc_and_insert(arr1, pNEXT->data);
                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, arr_temp);
                char *arr2 = ")";
                OUT = str_alloc_and_insert(OUT, arr2);
                pNEXT = pNEXT1;
                p = pNEXT->next;
            }
            else if (strcmp(pNEXT1->data, "NOT") != 0)
            {
                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }

            while ((strcmp(p->data, "O") == 0) || (strcmp(p->data, "ON") == 0))
            {
                char *arr = "*";

                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                pNEXT->data = str_alloc_and_insert(arr_temp, pNEXT->data);
                pNEXT->data = str_alloc_and_insert(pNEXT->data, arr_temp1);
                if (strcmp(pNEXT1->data, "NOT") == 0)
                {
                    char *arr1 = "!(";
                    char *arr_temp = str_alloc_and_insert(arr1, pNEXT->data);
                    OUT = str_alloc_and_insert(OUT, arr);
                    OUT = str_alloc_and_insert(OUT, arr_temp);
                    char *arr2 = ")";
                    OUT = str_alloc_and_insert(OUT, arr2);
                    pNEXT = pNEXT1;
                    p = pNEXT->next;
                    if (strncmp(p->data, "(", 1) == 0)
                    {
                        char *arr = "?";
                        count = count + 1;
                        OUT = str_alloc_and_insert(OUT, arr);
                        char *arr1 = "(";
                        OUT = str_alloc_and_insert(OUT, arr1);
                        p = p->next;
                        continue;
                    }
                    continue;
                }

                OUT = str_alloc_and_insert(OUT, arr);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            if (strncmp(p->data, "(", 1) == 0)
            {
                char *arr = "?";
                count = count + 1;
                OUT = str_alloc_and_insert(OUT, arr);
                char *arr1 = "(";
                OUT = str_alloc_and_insert(OUT, arr1);
                p = p->next;
                continue;
            }
            continue;
        }
        else if (strcmp(p->data, "ALD") == 0)
        { // Xem giải thich của "OLD"
            char *arr = "*";
            char *arr_m = "(";
            OUT = str_alloc_and_insert(arr_m, OUT); // Mở ngoặc để chuẩn bị đóng cả cụm ALD này
            int size_of_arr = strlen(OUT);

            char *OUTtemp, *OUTtemp1;
            OUTtemp = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUTtemp, OUT);
            char *token = strtok(OUTtemp, "?");
            if (strlen(token) != size_of_arr) // phát hiện dấu ? suy ra đang OLD 2 nhánh thì thay dấu "?" bằng "+"
            {
                size_of_arr = strlen(token);
                OUTtemp1 = (char *)calloc(size_of_arr, sizeof(char));
                strcpy(OUTtemp1, token);
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, arr);
                token = strtok(NULL, " ");
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, token);
                free(OUT);
                count = count - 1;
                size_of_arr = strlen(OUTtemp1);
                OUT = (char *)calloc(size_of_arr, sizeof(char));
                char *arr_d = ")";
                OUT = str_alloc_and_insert(OUT, arr_d); // Đóng ngoặc cụm ALD này
                strcpy(OUT, OUTtemp1);
                free(OUTtemp1);
                p = p->next;
                if (strncmp(p->data, "(", 1) == 0)
                {
                    char *arr = "?";
                    count = count + 1;
                    OUT = str_alloc_and_insert(OUT, arr);
                    char *arr1 = "(";
                    OUT = str_alloc_and_insert(OUT, arr1);
                    p = p->next;
                    continue;
                }
                continue;
            }
            else
            {
                LinkList *search;
                search = p->prev;
                while (strncmp(search->data, "(", 1) != 0)
                {
                    search = search->prev;
                }
                search = search->prev;
                char *search_str = strstr(OUTtemp, search->data);
                char *token1 = strtok(search_str, "(");
                token1 = strtok(NULL, " ");
                OUTtemp1 = str_alloc_and_insert(token, arr);
                char *add = "(";
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, add);
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, token1);
                free(OUT);
                size_of_arr = strlen(OUTtemp1);
                OUT = (char *)calloc(size_of_arr, sizeof(char));
                strcpy(OUT, OUTtemp1);
                free(OUTtemp1);
                char *arr_d = ")";
                OUT = str_alloc_and_insert(OUT, arr_d); // Đóng ngoặc cụm ALD này
                p = p->next;
                if (strncmp(p->data, "(", 1) == 0)
                {
                    char *arr = "?";
                    count = count + 1;
                    OUT = str_alloc_and_insert(OUT, arr);
                    char *arr1 = "(";
                    OUT = str_alloc_and_insert(OUT, arr1);
                    p = p->next;
                    continue;
                }
                continue;
            }
        }
        else if (strcmp(p->data, "OLD") == 0)
        {
            char *arr = "+";
            char *arr_m = "(";
            OUT = str_alloc_and_insert(arr_m, OUT); // Mở ngoặc để chuẩn bị đóng cả cụm OLD này
            int size_of_arr = strlen(OUT);

            char *OUTtemp, *OUTtemp1;
            OUTtemp = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUTtemp, OUT);
            char *token = strtok(OUTtemp, "?");
            if (strlen(token) != size_of_arr) // phát hiện dấu ? suy ra đang OLD 2 nhánh thì thay dấu "?" bằng "+"
            {
                size_of_arr = strlen(token);
                OUTtemp1 = (char *)calloc(size_of_arr, sizeof(char));
                strcpy(OUTtemp1, token);

                OUTtemp1 = str_alloc_and_insert(OUTtemp1, arr);
                token = strtok(NULL, " ");
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, token);
                free(OUT);
                count = count - 1;
                size_of_arr = strlen(OUTtemp1);
                OUT = (char *)calloc(size_of_arr, sizeof(char));
                strcpy(OUT, OUTtemp1);
                free(OUTtemp1);
                char *arr_m = ")";
                OUT = str_alloc_and_insert(OUT, arr_m); // Đóng ngoặc để chuẩn bị đóng cả cụm OLD này
                p = p->next;
                if (strncmp(p->data, "(", 1) == 0) // Nếu phần tử tiếp theo là "(" thì thêm ?
                {
                    char *arr = "?";
                    count = count + 1;
                    OUT = str_alloc_and_insert(OUT, arr);
                    char *arr1 = "(";
                    OUT = str_alloc_and_insert(OUT, arr1);
                    p = p->next;
                    continue;
                }
                continue;
            }
            else // ko phát hiện dấu ? nên đang OLD một nhánh với 1 cụm tính toán phía trước
            {    // ta phải xác định phần tử trước nhánh có nghĩa là nằm trưỡc dấu "(" ta đắt tên là A
                LinkList *search;
                search = p->prev;
                while (strncmp(search->data, "(", 1) != 0)
                {
                    search = search->prev;
                }
                search = search->prev;
                char *search_str = strstr(OUTtemp, search->data); // lấy địa chỉ từ phần tử A trở đi trong chuỗi OUT
                char *token1 = strtok(search_str, "(");           // chuỗi mới search_str = "A(...)" nên ta tìm "("
                token1 = strtok(NULL, " ");                       // để phân tách A với (...)
                OUTtemp1 = str_alloc_and_insert(token, arr);      // chưa chuỗi từ đầu đến phần tử A và thêm "+"
                char *add = "(";
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, add);    // thêm "("
                OUTtemp1 = str_alloc_and_insert(OUTtemp1, token1); // thêm "..)"
                free(OUT);
                size_of_arr = strlen(OUTtemp1);
                OUT = (char *)calloc(size_of_arr, sizeof(char)); // chuỗi mới đã thêm dấu +
                strcpy(OUT, OUTtemp1);
                free(OUTtemp1);
                char *arr_m = ")";
                OUT = str_alloc_and_insert(OUT, arr_m); // Đóng ngoặc để chuẩn bị đóng cả cụm OLD này
                p = p->next;
                if (strncmp(p->data, "(", 1) == 0) // Nếu phần tử tiếp theo là "(" thì thêm ?
                {
                    char *arr = "?";
                    count = count + 1;
                    OUT = str_alloc_and_insert(OUT, arr);
                    char *arr1 = "(";
                    OUT = str_alloc_and_insert(OUT, arr1);
                    p = p->next;
                    continue;
                }
                continue;
            }
        }
        else if (strcmp(p->data, "NOT") == 0)
        {
            char *arr1 = "!(";
            OUT = str_alloc_and_insert(arr1, OUT);
            char *arr2 = ")";
            OUT = str_alloc_and_insert(OUT, arr2);
        }
        else if (strcmp(p->data, "=") == 0) // q0.1=(A*B)\n
        {
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = "\n";
            OUT = str_alloc_and_insert(OUT, arr3);
            OUT = str_alloc_and_insert(p->data, OUT);
            p = p->next;
            OUT = str_alloc_and_insert(p->data, OUT);
        }
        else if (strcmp(p->data, "CTU") == 0)
        {
            // do some thing
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = "\n";
            OUT = str_alloc_and_insert(OUT, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(p->data, "CTD") == 0)
        {
            // do some thing
            // do some thing
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = "\n";
            OUT = str_alloc_and_insert(OUT, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(p->data, "TON") == 0)
        {
            // do some thing
            // do some thing
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = "\n";
            OUT = str_alloc_and_insert(OUT, arr3); // OUT : = (a*b)\n
        }
        else if (strcmp(p->data, "TOF") == 0)
        {
            // do some thing
            // do some thing
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = "\n";
            OUT = str_alloc_and_insert(OUT, arr3); // OUT : = (a*b)\n
        }

        p = p->next;
    }

    fclose(pFile);
}

char *str_alloc_and_insert(const char *str1, const char *str2)
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
