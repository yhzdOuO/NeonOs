#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "copy.h"

void* mallocread (FILE* fd, int offset, int size, int num) {
    void* buffer = malloc(size * num);
    if (buffer == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    int rtv = 0;
    if ((rtv = fseek(fd, offset, SEEK_SET)) != 0) {
        printf("fseek failed: %d\n", rtv);
        free(buffer);
        return NULL;
    }
    if ((rtv = fread(buffer, size, num, fd)) < num) {
        printf("read failed: read %d bytes, but %d bytes required\n", rtv, num);
        free(buffer);
        return NULL;
    }
    return buffer;
}

int main (int argc, char* argv[]) {
    int next = 0;
    char *src = NULL, *dest = NULL, *section = ".text";
    int offset = 0;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            // 选项
            if (strcmp("-s", argv[i]) == 0) {
                if (i == argc - 1) {
                    printf("missing section\n");
                    return -1;
                }
                section = argv[++i];
            }
            else if (strcmp("-o", argv[i]) == 0) {
                if (i == argc - 1) {
                    printf("missing offset\n");
                    return -1;
                }
                offset = atoi(argv[++i]);
            }
            else {
                printf("unknown option: %s\n", argv[i]);
                return -1;
            }
        } else {
            // 参数
            if (next == 0) {
                src = argv[i];
                next ++;
            }
            else if (next == 1) {
                dest = argv[i];
                next ++;
            }
            else {
                printf("too many arguments. usage: textcopy [src] [dest] [-s secion(=.text)][-o offset(=0)]\n");
                return -1;
            }
        }
    }
    if (next < 2) {
        printf("missing dest or src. usage: textcopy [src] [dest] [-s secion(=.text)][-o offset(=0)]\n");
        return -1;
    }
    // find the offset of .text in src
    FILE* sf = fopen(src, "rb");

    // 读入elf head
    Elf64_Ehdr* hd = mallocread(sf, 0, sizeof(Elf64_Ehdr), 1);
    if (hd == NULL) {
        return -1;
    }
    
    // 读入section table
    Elf64_Shdr* shtb = mallocread(sf, hd->e_shoff, hd->e_shentsize, hd->e_shnum);
    if (shtb == NULL) {
        return -1;
    }

    // 找到字符串表并读入
    Elf64_Shdr* strtb_entry = &shtb[hd->e_shstrndx];
    char* strtb = mallocread(sf, strtb_entry->sh_offset, strtb_entry->sh_size, 1);
    if (strtb == NULL) {
        return -1;
    }
    
    fclose(sf);

    // 遍历section table找段
    Elf64_Shdr* entry = shtb;
    for (int i = 0; i < hd->e_shnum; i++, entry++) {
        if (strcmp(section, strtb + entry->sh_name) == 0) {
            if (copy(src, dest, entry->sh_offset, entry->sh_size, offset) != -1) {
                printf("copy section \"%s\" from file \"%s\" to file \"%s\" at offset=%d\n", section, src, dest, offset);
            }
            entry = NULL;
            break;
        }
    }
    if (entry != NULL) {
        printf("no such section \"%s\" in file \"%s\"\n", section, src);
    }

    free(hd);
    free(shtb);
    free(strtb);
    return 0;
}