#include <stdio.h>
#include <stdlib.h>
#include <Python.h>

typedef struct MemRow {
    struct MemRow* previous;
    char data[64];
    unsigned long value;
    struct MemRow* next;
} MemRow;

typedef struct MemHM {
    int num_of_elements;
    int capacity;
    MemRow** arr;
} MemHM;

MemRow* fetch_mem_data(char *filename);
void print_meminfo_data(MemRow* head);
void free_meminfo_list(MemRow* head);

int main(int argc, char* argv[])
{
    MemRow* list_head = fetch_mem_data("/proc/meminfo");
    print_meminfo_data(list_head);
    free_meminfo_list(list_head);
    return 0;
}

MemRow* fetch_mem_data(char *filename)
{
    FILE* info_stream = fopen(filename, "r");
    MemRow* head = (MemRow *)malloc(sizeof(MemRow));
    head->previous = NULL;
    MemRow* current = head;
    char buffer[256];
    int actual_l = 0;
    if (info_stream == NULL) return NULL;
    while (fgets(buffer, sizeof(buffer), info_stream)) {
        if (sscanf(buffer, "%64[^:]: %ld kB", current->data, &current->value))
        {
            current->next = (MemRow *)malloc(sizeof(MemRow));
            current = current->next;
            current->next = NULL;
        } else {
            printf("Failed to format\n");
        }
    }
    fclose(info_stream);
    return head;
}

void print_meminfo_data(MemRow* head)
{
    for (MemRow* current = head; current->next; current = current->next)
    {
        printf("%s:\t %ld kB \n", current->data, current->value);
    }
}

void free_meminfo_list(MemRow* head)
{
    MemRow* current = head;
    while (current != NULL)
    {
        MemRow* next = current->next;
        free(current);
        current = next;
    }
}