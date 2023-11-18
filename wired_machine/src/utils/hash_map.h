#ifndef __LINKED_DICT_H__
#define __LINKED_DICT_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "base_log.h"
typedef struct _hash_map {
    char label[64];
    long long addr;
    int used;

    struct _hash_map* next;
} hash_map_t;

#define __HASH_MAP_IMPL__
#ifdef __HASH_MAP_IMPL__

#define HASHMAP_SIZE 256
hash_map_t hash_map[HASHMAP_SIZE] = {0};


size_t hash_label(char label[64]) {
    size_t hash = 0;

    char* current = label;
    while (*current != '\0') {
        hash ^= (*current) + hash;
        current++;
    }

    return hash%HASHMAP_SIZE;
}

void table_add_label(char label[64]) {

    size_t hash = hash_label(label);
    hash_map_t* instance = &hash_map[hash];
    hash_map_t* current_instance = instance;

    if (!current_instance->used) {
        current_instance->next = NULL;
        current_instance->addr = 0;
        current_instance->used = 1;
        
        if (strcpy(current_instance->label, label) == NULL)
            ERROR("can't copy the label")
        
        return;
    }
    INFO("Collison")

    if (strcmp(current_instance->label, label) == 0 && current_instance->used) 
        return;

    if (strcmp(current_instance->label, label) != 0 && current_instance->used) {
        
        while (current_instance->next != NULL) {
            
            current_instance = current_instance->next;
            if (strcmp(current_instance->label, label) == 0 && current_instance->used) {
                return;
            
            } else if (!current_instance->used) {
                current_instance->next = NULL;
                current_instance->addr = 0;
                current_instance->used = 1;
        
                if (strcpy(current_instance->label, label) == NULL) 
                    ERROR("can't copy the label")
                    
                return;
            }
        }
    }
    
    current_instance->next = (hash_map_t*)malloc(sizeof(hash_map_t));
    if (current_instance->next == NULL) 
        ERROR("Can't allocate memory for a new hashmap")

    current_instance->next->next = NULL;
    current_instance->next->addr = 0;
    current_instance->next->used = 1;
        
    if (strcpy(current_instance->next->label, label) == NULL) 
        ERROR("can't copy the label")
}


#endif
#endif
