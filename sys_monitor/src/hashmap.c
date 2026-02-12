#include <stdlib.h>
#include <string.h>
#include "../include/metrics.h"

MemHM* create_mem_hm(int capacity) {
    MemHM* mp = (MemHM*)malloc(sizeof(MemHM));
    if (!mp) return NULL;
    mp->capacity = capacity;
    mp->num_of_elements = 0;
    mp->arr = (MemRow**)calloc(capacity, sizeof(MemRow*));
    return mp;
}

void insert_element(MemHM* mp, char* key, unsigned long value)
{
    int indice = hash_function(mp, key);
    MemRow* newRow = (MemRow *)malloc(sizeof(MemRow));
    if (!newRow) return;
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
    if (mp->num_of_elements == 0) { 
        mp->dic_head = newRow;
        newRow->dic_previous = NULL;
        newRow->dic_next = NULL;
        mp->dic_actual = newRow;
    } else {
        newRow->dic_previous = mp->dic_actual;
        mp->dic_actual->dic_next = newRow;
        mp->dic_actual = newRow;
        newRow->dic_next = NULL;
    }
    mp->num_of_elements++;
}

void delete_element(MemHM* mp, char* key)
{
    int indice = hash_function(mp, key);
    MemRow* previous;
    MemRow* actual = mp->arr[indice];
    MemRow* sig = actual->next;
    MemRow* target;
    while (actual != NULL) {
        if (strcmp(actual->data, key) == 0) {
            if (actual == mp->arr[indice]) {
                mp->arr[indice] = actual->next;
                mp->arr[indice]->previous = NULL;
            } else {
                previous->next = actual->next;
                sig -> previous = actual->previous;
            }
            target = actual;
            break;
        }
        previous = actual;
        actual = actual->next;
        sig = actual->next;
    }
    if (target->dic_previous) {
        target->dic_previous->dic_next = target->dic_next;
    } else {
        mp->dic_head = target->dic_next;
    }

    if (target->dic_next) {
        target->dic_next->dic_previous = target->dic_previous;
    } else {
        mp->dic_actual = target->dic_previous;
    }
    free(target);
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

void clean_mem_hm(MemHM* mp)
{
    if (mp == NULL) return;
    MemRow* current = mp->dic_head;
    while (current != NULL)
    {
        MemRow* next = current->dic_next;
        free(current);
        current = next;
    }
    free(mp->arr);
    free(mp);
}

void print_mem_hm(MemHM* mp)
{
    for (MemRow* current = mp->dic_head; current; current = current->dic_next)
    {
        printf("%s:\t %ld kB \n", current->data, current->value);
    }
}

int hash_function(MemHM* mp, char* key)
{
    //Taken from geeks for geeks
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