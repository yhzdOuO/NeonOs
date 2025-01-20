#include <stdio.h>
#include <stdlib.h>

int copy_section(const char *source_file, const char *dest_file, long src_offset, size_t length, long dest_offset) {
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

int main(int argc, char* argv[]) {
    const char *source_file = argv[1];
    const char *dest_file = argv[4];
    long src_offset = atoi(argv[2]);  // 从源文件的第100个字节开始
    size_t length = atoi(argv[3]);  // 复制200个字节
    long dest_offset = atoi(argv[5]);

    if (copy_section(source_file, dest_file, src_offset, length, dest_offset) == 0) {
        printf("文件复制成功！\n");
    } else {
        printf("文件复制失败！\n");
    }

    return 0;
}
