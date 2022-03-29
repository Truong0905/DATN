#ifndef _STL_H_
#define _STL_H_
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
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
char *str_alloc_and_insert(const char *str1, const char *str2) // chèn tại cuối chuỗi 
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
char* str_alloc_and_insert_every(const char* str1, size_t pos, const char* str2) // Chèn tại vị trí bất kỳ
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


#endif