#if !defined(METRICS_H)
#define METRICS_H
#include <stdio.h>

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

MemHM* fetch_mem_data(char *filename, int capacity);
MemHM* create_mem_hm(int capacity);
void insert_element(MemHM* mp, char* key, unsigned long value);
unsigned long search_element(MemHM* mp, char* key);
void delete_element(MemHM* mp, char* key);
int hash_function(MemHM* mp, char* key);
void print_meminfo_data(MemRow* head);
void free_meminfo_list(MemRow* head);
void clean_mem_hm(MemHM* mp);
void print_mem_hm(MemHM* mp);

#endif
