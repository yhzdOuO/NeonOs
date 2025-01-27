#include "copy.h"

int copy(const char *source_file, const char *dest_file, long src_offset, size_t length, long dest_offset) {
    FILE *src = fopen(source_file, "rb");  // 以二进制只读模式打开源文件
    if (src == NULL) {
        perror("打开源文件失败");
        return -1;
    }

    FILE *dest = fopen(dest_file, "rb+");  // 以二进制写模式打开目标文件
    if (dest == NULL) {
        perror("打开目标文件失败");
        fclose(src);
        return -1;
    }

    // 定位到文件中的指定位置
    if (fseek(src, src_offset, SEEK_SET) != 0 || fseek(dest, dest_offset, SEEK_SET) != 0) {
        perror("定位文件失败");
        fclose(src);
        fclose(dest);
        return -1;
    }

    // 读取指定长度的数据并写入目标文件
    unsigned char *buffer = (unsigned char *)malloc(length);
    if (buffer == NULL) {
        perror("内存分配失败");
        fclose(src);
        fclose(dest);
        return -1;
    }

    size_t bytes_read = fread(buffer, 1, length, src);
    if (bytes_read != length) {
        if (feof(src)) {
            printf("已到达文件末尾\n");
        } else {
            perror("读取文件失败");
        }
        free(buffer);
        fclose(src);
        fclose(dest);
        return -1;
    }

    // 将读取的数据写入目标文件
    size_t bytes_written = fwrite(buffer, 1, bytes_read, dest);
    if (bytes_written != bytes_read) {
        perror("写入文件失败");
        free(buffer);
        fclose(src);
        fclose(dest);
        return -1;
    }

    free(buffer);
    fclose(src);
    fclose(dest);
    return 0;
}