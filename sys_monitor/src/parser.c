#include <stdio.h>
#include <stdlib.h>
#include <Python.h>
#include "../include/metrics.h"

MemHM* fetch_mem_data(char *filename, int capacity)
{
    FILE* info_stream = fopen(filename, "r");
    MemHM* dict = create_mem_hm(capacity);
    MemRow* row = (MemRow *)malloc(sizeof(MemRow));
    char buffer[256];
    int actual_l = 0;
    if (info_stream == NULL) return NULL;
    while (fgets(buffer, sizeof(buffer), info_stream)) {
        if (sscanf(buffer, "%63[^:]: %ld kB", row->data, &row->value))
        {
            insert_element(dict, row->data, row->value);
        } else {
            printf("Failed to format line #: %d\n", actual_l);
        }
        actual_l++;
    }
    fclose(info_stream);
    free(row);
    return dict;
}