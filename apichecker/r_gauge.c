#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

typedef struct MemInfo {
    unsigned long mem_total;
    unsigned long mem_free;
    unsigned long mem_avail; 
    unsigned long buffers;
    unsigned long cached;
    unsigned long used;
} MemInfo;

MemInfo* fetch_mem_data(char *filename);
void print_mem_info(MemInfo info);

int main(int argc, char* argv[])
{
    MemInfo* mem_info = fetch_mem_data("/proc/meminfo");
    print_mem_info(*mem_info);
    free(mem_info);
    return 0;
}

MemInfo* fetch_mem_data(char *filename)
{
    FILE* info_stream = fopen(filename, "r");
    MemInfo *mem_info = (MemInfo *)malloc(sizeof(MemInfo));
    char buffer[256];
    int actual_l = 0;
    if (info_stream == NULL) return NULL;
    while (fgets(buffer, sizeof(buffer), info_stream)) {
        if (sscanf(buffer, "MemTotal: %ld kB", &mem_info->mem_total) == 1) continue;
        if (sscanf(buffer, "MemFree: %ld kB", &mem_info->mem_free) == 1) continue;
        if (sscanf(buffer, "MemAvailable: %ld kB", &mem_info->mem_avail) == 1) continue;
        if (sscanf(buffer, "Buffers: %ld kB", &mem_info->buffers) == 1) continue;
        if (sscanf(buffer, "Cached: %ld kB", &mem_info->cached) == 1) continue;
    }
    fclose(info_stream);
    mem_info->used = mem_info->mem_total - mem_info->mem_free - mem_info->buffers - mem_info->cached;
    return mem_info;
}

void print_mem_info(MemInfo info) 
{
    printf("MemTotal: %ld\nMemFree: %ld\nMemAvail: %ld\nBuffers: %ld\nCached: %ld\nUsed: %ld\n", 
        info.mem_total, info.mem_free, info.mem_avail, info.buffers, info.cached, info.used);
}