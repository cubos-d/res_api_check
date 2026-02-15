#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/metrics.h"

CpuData* create_cpu_matrix(int numcores)
{
    CpuData* matrix = (CpuData *)malloc(sizeof(CpuData));
    matrix->n_cores = numcores;
    matrix->mat = (unsigned long**)malloc(numcores*sizeof(unsigned long*));
    for (int i = 0; i<numcores; i++) {
        matrix->mat[i] = (unsigned long *)malloc(10*sizeof(unsigned long));
    }
    return matrix;
}

CpuData* fetch_cpu_data()
{
    int num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
    FILE* info_stream = fopen("/proc/stat", "r");
    CpuData* cpu = create_cpu_matrix(num_cpus);
    char buffer[320];
    if (info_stream == NULL) return NULL;
    if (fgets(buffer, sizeof(buffer), info_stream)) {
        sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", 
            cpu->total, cpu->total+1, cpu->total+2, cpu->total+3, cpu->total+4,
            cpu->total+5, cpu->total+6, cpu->total+7, cpu->total+8, cpu->total+9);
    }
    int i = 0;
    while (i < cpu->n_cores && fgets(buffer, sizeof(buffer), info_stream)) {
        if (strncmp(buffer, "cpu", 3) == 0) {
            int cpu_id;
            sscanf(buffer, "cpu%d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld", 
                &cpu_id, 
                cpu->mat[i], cpu->mat[i]+1, cpu->mat[i]+2, cpu->mat[i]+3, cpu->mat[i]+4,
                cpu->mat[i]+5, cpu->mat[i]+6, cpu->mat[i]+7, cpu->mat[i]+8, cpu->mat[i]+9);
            i++;
        }
    }
    fclose(info_stream);
    return cpu;
}

void print_cpu_data(CpuData* cpu_data)
{
    printf("Total: ");
    for (int i = 0; i < 10; i++) printf("%ld ", cpu_data->total[i]);
    printf("\n");
    for (int i = 0; i < cpu_data->n_cores; i++) {
        printf("cpu %d: ", i);
        for (int j = 0; j < 10; j++) {
            printf("%ld ", cpu_data->mat[i][j]);
        }
        printf("\n");
    }
}

void clean_cpu_data(CpuData* cpu_data)
{
    for (int i = 0; i < cpu_data->n_cores; i++) {
        free(cpu_data->mat[i]);
    }
    free(cpu_data->mat);
    free(cpu_data);
}

/*
int main(int argc, char* argv[])
{
    CpuData* cpu = fetch_cpu_data();
    if (cpu) {
        print_cpu_data(cpu);
        clean_cpu_data(cpu);
    } else {
        printf("La cagaste\n");
    }
    return 0;
}
*/