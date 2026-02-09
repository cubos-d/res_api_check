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
void insert_element(MemHM* mp, char* key, unsigned long value);
unsigned long search_element(MemHM* mp, char* key);
void delete_element(MemHM* mp, char* key);
int hash_function(MemHM* mp, char* key);
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
            printf("Failed to format line #: %d\n", actual_l);
        }
        actual_l++;
    }
    fclose(info_stream);
    return head;
}

void insert_element(MemHM* mp, char* key, unsigned long value)
{
    int indice = hash_function(mp, key);
    MemRow* newRow = (MemRow *)malloc(sizeof(MemRow));
    strcpy(newRow->data, key);
    newRow->value = value;
    newRow->previous = NULL;
    newRow->next = NULL;
    if (mp->arr[indice] == NULL) {
        mp->arr[indice] = newRow;
    } else {
        mp->arr[indice]->previous = newRow;
        newRow->next = mp->arr[indice];
        mp->arr[indice] = newRow;
    }
}

void delete_element(MemHM* mp, char* key)
{
    int indice = hash_function(mp, key);
    MemRow* previous;
    MemRow* actual;
    while (actual != NULL) {
        if (strcmp(actual->data, key) == 0) {
            
        }
    }
}

unsigned long search_element(MemHM* mp, char* key)
{
    int indice = hash_function(mp, key);
    MemRow* actualRow = mp->arr[indice];
    for (MemRow* act=actualRow; act; act = act->next)
    {
        if (strcmp(act->data, key) == 0) return act->value;
    }
    printf("Key Error: Key %s does NOT exist", key);
    return 0;
}

int hash_function(MemHM* mp, char* key)
{
    int arrayIndex;
    int sum = 0, factor = 31;
    for (int i = 0; i < strlen(key); i++)
    {
        sum = ((sum % mp->capacity) + (((int)key[i])*factor)%mp->capacity) % mp->capacity;
        factor = ((factor % __INT16_MAX__)*(31% __INT16_MAX__)) % __INT16_MAX__;
    }
    arrayIndex = sum;
    return arrayIndex;
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