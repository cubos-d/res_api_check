#if !defined(METRICS_H)
#define METRICS_H
#endif
#include <stdio.h>

typedef struct MemRow {
    struct MemRow* previous;
    char data[64];
    unsigned long value;
    struct MemRow* next;
    struct MemRow* dic_next;
    struct MemRow* dic_previous;
} MemRow;

typedef struct MemHM {
    int num_of_elements;
    int capacity;
    MemRow** arr;
    MemRow* dic_head;
    MemRow* dic_actual;
} MemHM;

typedef struct CpuData {
    int n_cores;
    unsigned long total[10];
    unsigned long** mat;
} CpuData;

MemHM* fetch_mem_data(char *filename, int capacity);
MemHM* create_mem_hm(int capacity);
void insert_element(MemHM* mp, char* key, unsigned long value);
unsigned long search_element(MemHM* mp, char* key);
void delete_element(MemHM* mp, char* key);
int hash_function(MemHM* mp, char* key);
void clean_mem_hm(MemHM* mp);
void print_mem_hm(MemHM* mp);

CpuData* create_cpu_matrix(int numcores);
CpuData* fetch_cpu_data();
void print_cpu_data(CpuData* cpu_data);
void clean_cpu_data(CpuData* cpu_data);