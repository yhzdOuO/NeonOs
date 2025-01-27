#include "copy.h"

int main(int argc, char* argv[]) {
    const char *source_file = argv[1];
    const char *dest_file = argv[4];
    long src_offset = atoi(argv[2]);  // 从源文件的第100个字节开始
    size_t length = atoi(argv[3]);  // 复制200个字节
    long dest_offset = atoi(argv[5]);

    if (copy(source_file, dest_file, src_offset, length, dest_offset) == 0) {
        printf("文件复制成功！\n");
    } else {
        printf("文件复制失败！\n");
    }

    return 0;
}
