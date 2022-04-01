#include "STL.h"
int main(void)
{
    FILE *fptest = NULL;
    LinkList *p;
    int row = 0U;
    fptest = ConvFrTxtFile(&row);
    ConvFrList(fptest, row);
    ListFinal();
    BranchWithFirst_F();
    p = First_F;
    while (p != NULL)
    {
        printf("%s ", p->data);
        p = p->next;
    }
    Final_File_text();

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
    int a = 0; // Kiểm tra xem đã vào chương trình chưa
    //Đọc từng dòng từ file cho tới khi gặp NULL
    while (fgets(arr, 128, fp) != NULL)
    {
        count = count + 1;
        if (a == 0 && (strncmp(arr, "LD", 2) == 0))
        {
            check = 1;
            redu = count;
            a = 1;
        }

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
    LinkList *pNEXT, *pNEXT1, *pPREV, *pPREV1;
    int cout1 = 0; // Đếm số "("
    int cout2 = 0; // Đếm số ")"
    p = First_F;
    while (p != NULL)
    {
        if (p == First_F) // Thêm ký tự '(' vào đầu
        {
            k = (LinkList *)malloc(sizeof(LinkList));
            k->data = (char *)calloc(2, sizeof(char));
            k->data = "(\0";
            cout1++;
            k->next = p;
            p->prev = k;
            k->prev = NULL;
            First_F = k;
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
                z = (LinkList *)malloc(sizeof(LinkList));
                z->data = (char *)calloc(4, sizeof(char));
                z->data = "not\0";
                z->next = k;
                k->prev = z;
                z->prev = NULL;
                First_F = z;
            }
        }
        else
        {
            if ((strcmp(p->data, "ALD") == 0) || (strcmp(p->data, "OLD") == 0) || (strcmp(p->data, "TON") == 0) || (strcmp(p->data, "CTU") == 0) || (strcmp(p->data, "CTD") == 0) || (strcmp(p->data, "CTUD") == 0))
            {
                if ((strcmp(p->data, "ALD") == 0) || (strcmp(p->data, "OLD") == 0)) // not của 1 nhánh
                {
                    pNEXT = p->next;
                    if (strcmp(pNEXT->data, "NOT") == 0)
                    {
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(2, sizeof(char));
                        k->data = "!\0";
                        p->next = k;
                        k->prev = p;
                        k->next = pNEXT;
                        pNEXT->prev = k;
                    }
                }
                pPREV = p->prev;
                if ((strcmp(pPREV->data, "ALD") != 0) && (strcmp(pPREV->data, "OLD") != 0)) // Đây ko phải nhánh lớn
                {
                    r = p->prev;
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(2, sizeof(char));
                    k->data = ")\0";
                    cout2++;
                    r->next = k;
                    k->prev = r;
                    k->next = p;
                    p->prev = k;
                }
            }
            else if ((strcmp(p->data, "=") == 0))
            {
                if (cout1 > cout2)
                {
                    r = p->prev;
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(2, sizeof(char));
                    k->data = ")\0";
                    cout2++;
                    r->next = k;
                    k->prev = r;
                    k->next = p;
                    p->prev = k;
                }
            }
            else if ((strncmp(p->data, "LD", 2) == 0) || (strncmp(p->data, "LDN", 3) == 0))
            {
                r = p->prev;
                z = r->prev;
                if ((strcmp(z->data, "=") == 0) || (strcmp(r->data, "N") == 0) || (strcmp(r->data, "ALD") == 0) || (strcmp(r->data, "OLD") == 0))
                {
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(2, sizeof(char));
                    k->data = "(\0";
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
                        k->data = (char *)calloc(2, sizeof(char));
                        k->data = "(\0";
                        cout1++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;
                    }
                    else
                    {
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(2, sizeof(char));
                        k->data = ")\0";
                        cout1++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;

                        r = p->prev;
                        z = r->prev;
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(2, sizeof(char));
                        k->data = "(\0";
                        cout2++;
                        r->next = k;
                        k->prev = r;
                        k->next = p;
                        p->prev = k;
                    }
                }
                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                pPREV = p->prev;
                pPREV1 = pPREV->prev;
                if (strcmp(pNEXT1->data, "NOT") == 0)
                {
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(4, sizeof(char));
                    k->data = "not\0";
                    pPREV1->next = k;
                    k->prev = pPREV1;
                    k->next = pPREV;
                    pPREV->prev = k;
                }
            }
            else if ((strcmp(p->data, "A") == 0) || (strcmp(p->data, "O") == 0) || (strcmp(p->data, "AN") == 0) || (strcmp(p->data, "ON") == 0))
            {
                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                pPREV = p->prev;

                if (strcmp(pNEXT1->data, "NOT") == 0) // NOT của 1 tập hợp các biến
                {
                    int check_nhanh = 0;
                    while (strncmp(pPREV->data, "(", 1) != 0) // Tìm vị trí đầu của nhánh
                    {
                        if ((strcmp(pPREV->data, "ALD") == 0) || (strcmp(pPREV->data, "OLD") == 0)) // nếu đâu là not của (    nhánh và các biến )
                        {
                            check_nhanh = 1;
                            break;
                        }
                        pPREV = pPREV->prev;
                    }

                    if (pPREV != First_F)
                    {
                        if (check_nhanh == 0)
                        {
                            pPREV1 = pPREV->prev;
                            if (strncmp(pPREV1->data, ")", 1) != 0)
                            {
                                r = pPREV->prev;
                                k = (LinkList *)malloc(sizeof(LinkList));
                                k->data = (char *)calloc(4, sizeof(char));
                                k->data = "not\0";
                                r->next = k;
                                k->prev = r;
                                k->next = pPREV;
                                pPREV->prev = k;
                            }
                            else
                            {
                                r = pPREV->prev;
                                k = (LinkList *)malloc(sizeof(LinkList));
                                k->data = (char *)calloc(5, sizeof(char));
                                k->data = "1not\0";
                                r->next = k;
                                k->prev = r;
                                k->next = pPREV;
                                pPREV->prev = k;
                            }
                        }
                        else
                        {
                            char *arr_NEXT1 = "T_";
                            pNEXT1->data = str_alloc_and_insert(arr_NEXT1, pNEXT1->data);
                        }
                    }
                    else
                    {
                        k = (LinkList *)malloc(sizeof(LinkList));
                        k->data = (char *)calloc(4, sizeof(char));
                        k->data = "not\0";
                        k->next = pPREV;
                        pPREV->prev = k;
                        k->prev = NULL;
                        First_F = k;
                    }
                }
            }
            else if ((strncmp(p->data, "MOV", 3) == 0))
            {
                r = p->prev;
                if ((strcmp(r->data, "ALD") != 0) && (strcmp(r->data, "OLD") != 0) && (strcmp(r->data, "AENO") != 0))
                {
                    k = (LinkList *)malloc(sizeof(LinkList));
                    k->data = (char *)calloc(2, sizeof(char));
                    k->data = ")\0";
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
    char *insert_str_pre = "(";
    char *insert_str_next = ")";
    char *insert_str_mul = "*";
    char *insert_str_add = "+";
    char *insert_str_H = "?";
    int check = 0; // Kiểm tra xem hết 1 network chưa
    int check_H = 0;
    char *OUTtemp_H = ""; // Kiểm tra xem có nhánh lớn trước đó không
    int count = 0;        // Đếm số dấu "?"
    pFile = fopen("PLC_F.txt", "w");
    p = First_F;
    while (p != NULL)
    {

        if (check == 0) // Chưa kết thúc 1 network
        {
            OUT = "";
            check = 1;
        }
        if (strcmp(p->data, "N") == 0)
        {
            check = 0;
            char *OUTtemp_H = ""; // Kiểm tra xem có nhánh lớn trước đó không
            p = p->next;
            continue;
        }

        if (strncmp(p->data, "(", 1) == 0)
        {
                
            
                char *arr_temp = "!";
                pPREV = p->prev;

               
            if (pPREV != NULL ) 
            {    pPREV1 = pPREV->prev;
                  if ((strncmp(pPREV->data, "not", 3) != 0) && (strncmp(pPREV->data, "1no", 3) != 0))
                {
                    if (strncmp(pNEXT->data, "(", 1) == 0) // Nếu sau đó sẽ có dấu "(" thì thêm "?" vào giữa 2 dấu
                    {
                        OUT = str_alloc_and_insert(OUT, insert_str_next);
                        OUT = str_alloc_and_insert(OUT, insert_str_H);
                        count++;
                        OUT = str_alloc_and_insert(OUT, insert_str_pre);
                        p = pNEXT->next;
                        continue;
                    }
                    else
                    {
                        OUT = str_alloc_and_insert(OUT, insert_str_pre);
                    }
                }
                else if ((strncmp(pPREV->data, "1not", 3) == 0) || (strncmp(pPREV->data, "not", 3) == 0))
                {
                    int search_not = 0;
                   
                    if (pPREV1 == NULL)
                    {
                        p = p->next;
                        continue;
                    }
                    else
                    {
                        while ((strncmp(pPREV->data, "1not", 3) == 0) || (strncmp(pPREV->data, "not", 3) == 0))
                        {
                     
                            pPREV = pPREV ->prev ;
                                    if (pPREV == NULL)
                            {
                                break;
                            }
                                   if ((strncmp(pPREV->data, "ALD", 3) == 0) || (strncmp(pPREV->data, "OLD", 3) == 0))
                            {
                                search_not = 1;
                            }
                    
                        }
                    }
                    if (search_not == 1)
                    {
                        OUT = str_alloc_and_insert(OUT, insert_str_H);
                        count++;
                    }
                     OUT = str_alloc_and_insert(OUT, arr_temp);
                    OUT = str_alloc_and_insert(OUT, insert_str_pre);
                    p = p->next;
                    continue;
                }
            }
            else
            {
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
            }
            
                          
        }
        else if (strncmp(p->data, ")", 1) == 0)
        {
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            if ((strncmp(pNEXT->data, "not", 3) != 0) && (strncmp(pNEXT->data, "1no", 3) != 0))
            {
                if (strncmp(pNEXT->data, "(", 1) == 0) // Nếu sau đó sẽ có dấu "(" thì thêm "?" vào giữa 2 dấu
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                    OUT = str_alloc_and_insert(OUT, insert_str_H);
                    count++;
                    OUT = str_alloc_and_insert(OUT, insert_str_pre);
                    p = pNEXT->next;
                    continue;
                }
                else
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                }
            }
            else if (strncmp(pNEXT->data, "not", 3) == 0)
            {
                char *arr_temp = "!";
                OUT = str_alloc_and_insert(OUT, arr_temp);
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
                p = pNEXT1->next;
                continue;
            }
            else if (strncmp(pNEXT->data, "1no", 3) == 0)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_next);
                OUT = str_alloc_and_insert(OUT, insert_str_H);
                count++;

                while ((strncmp(pNEXT1->data, "1no", 3) == 0) || (strncmp(pNEXT1->data, "not", 3) == 0))
                {

                    pNEXT1 = pNEXT1->next;
                }

                p = pNEXT1;
                continue;
            }
        }
        else if ((strcmp(p->data, "LD") == 0) || (strcmp(p->data, "LDN") == 0))
        {
            pPREV = p->prev;
            pPREV1 = pPREV->prev;
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            if (pPREV1 != NULL) // dảm bảo kể cả à đâu chương trình mà có not thì vẫn được xét thêm "!"
            {
                if ((strncmp(pPREV1->data, "not", 3) == 0) || (strncmp(pPREV1->data, "1no", 3) == 0))
                {
                    char *arr = "!";
                    OUT = str_alloc_and_insert(OUT, arr);
                    OUT = str_alloc_and_insert(OUT, insert_str_pre);
                    pPREV = pPREV1->prev;
                    while ((strncmp(pPREV->data, "not", 3) == 0) || (strncmp(pPREV->data, "1no", 3) == 0))
                    {
                        OUT = str_alloc_and_insert(OUT, arr);
                        OUT = str_alloc_and_insert(OUT, insert_str_pre);
                        pPREV = pPREV->prev;
                        if (pPREV == NULL)
                        {
                            break;
                        }
                    }
                }
            }

            if (strcmp(pNEXT1->data, "NOT") == 0) // not cho 1 biến duy nhất
            {
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1->next;
                if ((strcmp(p->data, "A") != 0) && (strcmp(p->data, "AN") != 0) && (strcmp(p->data, "O") != 0) && (strcmp(p->data, "ON") != 0))
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                }

                continue;
            }
            OUT = str_alloc_and_insert(OUT, pNEXT->data);
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
            pPREV = p->prev;
            pPREV1 = pPREV->prev;
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            LinkList *pPREV_temp = pPREV;
            if ((strcmp(pPREV->data, "ALD") == 0) || (strcmp(pPREV->data, "OLD") == 0) || (strcmp(pPREV1->data, "A") == 0) || (strcmp(pPREV1->data, "O") == 0) || (strcmp(pPREV1->data, "AN") == 0) || (strcmp(pPREV1->data, "ON") == 0))
            {
                int size_OUT = strlen(OUT);
                int left = 0;
                int right = 0;
                for (int i = 0; i < size_OUT; i++)
                {
                    if (OUT[i] == '(')
                    {
                        left++;
                    }
                    if (OUT[i] == ')')
                    {
                        right++;
                    }
                }
                if ((strcmp(pNEXT1->data, "NOT") != 0) && (left < right))
                {
                    pNEXT->data = str_alloc_and_insert(insert_str_pre, pNEXT->data);
                }
            }

            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
               
                OUT = str_alloc_and_insert(OUT, insert_str_mul);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                OUT = str_alloc_and_insert(OUT, insert_str_next);
                p = pNEXT1->next;
            }
            else if (strcmp(pNEXT1->data, "NOT") != 0)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_mul);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            while ((strcmp(p->data, "A") == 0) || (strcmp(p->data, "AN") == 0))
            {

                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                if (strcmp(pNEXT1->data, "NOT") == 0)
                {
                    
                    OUT = str_alloc_and_insert(OUT, insert_str_mul);
                    OUT = str_alloc_and_insert(OUT, pNEXT->data);
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                    p = pNEXT1->next;
                    continue;
                }

                OUT = str_alloc_and_insert(OUT, insert_str_mul);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            if ((strcmp(pPREV_temp->data, "ALD") == 0) || (strcmp(pPREV_temp->data, "OLD") == 0)) // nhánh nhân hoặc cộng với biến
            {
                OUT = str_alloc_and_insert(OUT, insert_str_next);
            }
            if (strncmp(p->data, "(", 1) == 0)
            {
                if ((strcmp(pPREV_temp->data, "ALD") != 0) && (strcmp(pPREV_temp->data, "OLD") != 0)) // nhánh nhân hoặc cộng với biến
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                }

                OUT = str_alloc_and_insert(OUT, insert_str_H);
                count++;
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
                p = p->next;
            }

            if ((strncmp(p->data, ")", 1) == 0) && ((strcmp(pPREV_temp->data, "ALD") == 0) || (strcmp(pPREV_temp->data, "OLD") == 0)))
            {
                p = p->next;
            }
            if (((strcmp(p->data, "ALD") == 0) || (strcmp(p->data, "OLD") == 0)) && check_H == 1)
            {
                OUT = str_alloc_and_insert(OUTtemp_H, OUT);
                check_H = 0;
                count++;
                free(OUTtemp_H);
                OUTtemp_H = (char *)calloc(1, sizeof(char));
                OUTtemp_H = "";
            }

            continue;
        }
        else if ((strcmp(p->data, "O") == 0) || (strcmp(p->data, "ON") == 0))
        {
            pPREV = p->prev;
            pPREV1 = pPREV->prev;
            pNEXT = p->next;
            pNEXT1 = pNEXT->next;
            LinkList *pPREV_temp = pPREV;
            if ((strcmp(pPREV->data, "ALD") == 0) || (strcmp(pPREV->data, "OLD") == 0) || (strcmp(pPREV1->data, "A") == 0) || (strcmp(pPREV1->data, "O") == 0) || (strcmp(pPREV1->data, "ON") == 0) || (strcmp(pPREV1->data, "AN") == 0))
            {
                if (strcmp(pNEXT1->data, "NOT") != 0)
                {
                    pNEXT->data = str_alloc_and_insert(insert_str_pre, pNEXT->data);
                }
            }

            if (strcmp(pNEXT1->data, "NOT") == 0)
            {
                
                OUT = str_alloc_and_insert(OUT, insert_str_add);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                OUT = str_alloc_and_insert(OUT, insert_str_next);
                p = pNEXT1->next;
            }
            else if (strcmp(pNEXT1->data, "NOT") != 0)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_add);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            while ((strcmp(p->data, "O") == 0) || (strcmp(p->data, "ON") == 0))
            {

                pNEXT = p->next;
                pNEXT1 = pNEXT->next;
                if (strcmp(pNEXT1->data, "NOT") == 0)
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_add);
                    OUT = str_alloc_and_insert(OUT, pNEXT->data);
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                    p = pNEXT1->next;
                    continue;
                }

                OUT = str_alloc_and_insert(OUT, insert_str_add);
                OUT = str_alloc_and_insert(OUT, pNEXT->data);
                p = pNEXT1;
            }
            if ((strcmp(pPREV_temp->data, "ALD") == 0) || (strcmp(pPREV_temp->data, "OLD") == 0)) // nhánh nhân hoặc cộng với biến
            {
                OUT = str_alloc_and_insert(OUT, insert_str_next);
            }
            if (strncmp(p->data, "(", 1) == 0)
            {
                if ((strcmp(pPREV_temp->data, "ALD") != 0) && (strcmp(pPREV_temp->data, "OLD") != 0)) // nhánh nhân hoặc cộng với biến
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                }

                OUT = str_alloc_and_insert(OUT, insert_str_H);
                count++;
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
                p = p->next;
            }
            if (((strcmp(p->data, "A") == 0) || (strcmp(p->data, "AN") == 0)) && ((strcmp(pPREV_temp->data, "ALD") != 0) && (strcmp(pPREV_temp->data, "OLD") != 0)))
            {
                OUT = str_alloc_and_insert(OUT, insert_str_next);
                if ((strcmp(pPREV1->data, "A") == 0) || (strcmp(pPREV1->data, "AN") == 0))
                {
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                    OUT = str_alloc_and_insert(OUT, insert_str_next);
                    OUT = str_alloc_and_insert(insert_str_pre, OUT);
                }
            }

            if ((strncmp(p->data, ")", 1) == 0) && ((strcmp(pPREV_temp->data, "ALD") == 0) || (strcmp(pPREV_temp->data, "OLD") == 0)))
            {
                p = p->next;
            }

            if (((strcmp(p->data, "ALD") == 0) || (strcmp(p->data, "OLD") == 0)) && check_H == 1)
            {
                OUT = str_alloc_and_insert(OUTtemp_H, OUT);
                check_H = 0;
                count++;
                free(OUTtemp_H);
                OUTtemp_H = (char *)calloc(2, sizeof(char));
                OUTtemp_H = "";
            }

            continue;
        }
        else if (strcmp(p->data, "ALD") == 0)
        {
            if (count > 1)
            {
                char *OUTtemp_H1 = "";
                char *token = strtok(OUT, "?"); /// (...)?((.....)?(......))
                int a = strlen(token);
                OUTtemp_H = str_alloc_and_insert(OUTtemp_H, token); /// (...)?
                OUTtemp_H = str_alloc_and_insert(OUTtemp_H, insert_str_H);
                token = strtok(NULL, "?");
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, token); /// (...)?
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, insert_str_H);
                token = strtok(NULL, "?");
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, token); /// (...)?
                int size_of_arr_H1 = strlen(OUTtemp_H1);
                OUT = (char *)calloc(size_of_arr_H1, sizeof(char));
                strcpy(OUT, OUTtemp_H1);
                check_H = 1;
                count--;
            }

            OUT = str_alloc_and_insert(insert_str_pre, OUT); // Mở ngoặc cả cụm OLD này
            pNEXT = p->next;
            if (strncmp(pNEXT->data, "!", 1) == 0)
            {
                char *arr_temp = "!";
                OUT = str_alloc_and_insert(arr_temp, OUT);
            }
            int size_of_arr = strlen(OUT);

            char *OUTtemp, *OUTtemp1;
            OUTtemp = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUTtemp, OUT);
            char *token = strtok(OUTtemp, "?");
            OUTtemp = str_alloc_and_insert(OUTtemp, insert_str_mul);
            count--; // Thay đấu "?" bằng dấu "+"
            token = strtok(NULL, " ");
            OUTtemp1 = str_alloc_and_insert(OUTtemp, token);
            free(OUT);
            size_of_arr = strlen(OUTtemp1);
            OUT = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUT, OUTtemp1);
            OUT = str_alloc_and_insert(OUT, insert_str_next); // Đóng ngoặc cụm ALD này
            free(OUTtemp1);
            p = p->next;
            if (strncmp(p->data, "(", 1) == 0)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_H);
                count++;
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
                p = p->next;

                continue;
            }
            pNEXT = p->next;

            if ((check_H == 1) && (((strncmp(p->data, "OLD", 1) == 0) || (strncmp(p->data, "ALD", 1) == 0)) || ((strncmp(pNEXT->data, "OLD", 1) == 0) || (strncmp(pNEXT->data, "ALD", 1) == 0))))
            {
                OUT = str_alloc_and_insert(OUTtemp_H, OUT);
                check_H = 0;
                count++;
                free(OUTtemp_H);
                OUTtemp_H = (char *)calloc(2, sizeof(char));
                OUTtemp_H = "";
            }
            continue;
        }
        else if (strcmp(p->data, "OLD") == 0)
        {

            if (count > 1)
            {
                char *OUTtemp_H1 = "";
                char *token = strtok(OUT, "?"); /// (...)?((.....)?(......))
                int a = strlen(token);
                OUTtemp_H = str_alloc_and_insert(OUTtemp_H, token); /// (...)?
                OUTtemp_H = str_alloc_and_insert(OUTtemp_H, insert_str_H);
                token = strtok(NULL, "?");
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, token); /// (...)?
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, insert_str_H);
                token = strtok(NULL, "?");
                OUTtemp_H1 = str_alloc_and_insert(OUTtemp_H1, token); /// (...)?
                int size_of_arr_H1 = strlen(OUTtemp_H1);
                OUT = (char *)calloc(size_of_arr_H1, sizeof(char));
                strcpy(OUT, OUTtemp_H1);
                check_H = 1;
                count--;
            }

            OUT = str_alloc_and_insert(insert_str_pre, OUT); // Mở ngoặc cả cụm OLD này
            pNEXT = p->next;
            if (strncmp(pNEXT->data, "!", 1) == 0)
            {
                char *arr_temp = "!";
                OUT = str_alloc_and_insert(arr_temp, OUT);
            }
            int size_of_arr = strlen(OUT);

            char *OUTtemp, *OUTtemp1;
            OUTtemp = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUTtemp, OUT);
            char *token = strtok(OUTtemp, "?");
            OUTtemp = str_alloc_and_insert(OUTtemp, insert_str_add);
            count--; // Thay đấu "?" bằng dấu "+"
            token = strtok(NULL, " ");
            OUTtemp1 = str_alloc_and_insert(OUTtemp, token);
            free(OUT);
            size_of_arr = strlen(OUTtemp1);
            OUT = (char *)calloc(size_of_arr, sizeof(char));
            strcpy(OUT, OUTtemp1);
            OUT = str_alloc_and_insert(OUT, insert_str_next); // Đóng ngoặc cụm ALD này
            free(OUTtemp1);
            p = p->next;
            if (strncmp(p->data, "(", 1) == 0)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_H);
                count++;
                OUT = str_alloc_and_insert(OUT, insert_str_pre);
                p = p->next;

                continue;
            }
            pNEXT = p->next;

            if ((check_H == 1) && (((strncmp(p->data, "OLD", 1) == 0) || (strncmp(p->data, "ALD", 1) == 0)) || ((strncmp(pNEXT->data, "OLD", 1) == 0) || (strncmp(pNEXT->data, "ALD", 1) == 0))))
            {
                OUT = str_alloc_and_insert(OUTtemp_H, OUT);
                check_H = 0;
                count++;
                free(OUTtemp_H);
                OUTtemp_H = (char *)calloc(2, sizeof(char));
                OUTtemp_H = " ";
            }
            continue;
        }
        else if (strcmp(p->data, "T_NOT") == 0)
        {
            char *arr_temp = "!";
            OUT = str_alloc_and_insert(insert_str_pre, OUT);
            OUT = str_alloc_and_insert(arr_temp, OUT);
            OUT = str_alloc_and_insert(OUT, insert_str_next);
        }
        else if (strcmp(p->data, "=") == 0) // q0.1=(A*B)\n
        {
            int size_OUT = strlen(OUT);
            int left = 0;
            int right = 0;
            for (int i = 0; i < size_OUT; i++)
            {
                if (OUT[i] == '(')
                {
                    left++;
                }
                if (OUT[i] == ')')
                {
                    right++;
                }
            }
            while (left != right)
            {
                        if (left > right)
            {
                OUT = str_alloc_and_insert(OUT, insert_str_next);
                right ++ ;
            }
            else if (left < right)
            {
                OUT = str_alloc_and_insert(insert_str_pre, OUT);
                left ++ ;
            }
            }
            char *arr1 = "(";
            char *arr2 = ")";
            OUT = str_alloc_and_insert(arr1, OUT);
            OUT = str_alloc_and_insert(OUT, arr2);
            char *arr3 = ";\n";
            OUT = str_alloc_and_insert(OUT, arr3);
            OUT = str_alloc_and_insert(p->data, OUT);
            p = p->next;
            OUT = str_alloc_and_insert(p->data, OUT);
            fputs(OUT, pFile);
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