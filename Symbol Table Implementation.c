#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol Table Entry
struct Symbol {
    char name[256];
    char type[256];
    struct Symbol* next;
};

// Symbol Table (hash table)
#define TABLE_SIZE 100
struct Symbol* symbol_table[TABLE_SIZE];

// Hash function for symbol table
unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

// Add variable to symbol table
void add_symbol(const char* name, const char* type) {
    unsigned int index = hash(name);
    struct Symbol* new_symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
    strcpy(new_symbol->name, name);
    strcpy(new_symbol->type, type);
    new_symbol->next = symbol_table[index];
    symbol_table[index] = new_symbol;
}

// Find symbol in symbol table
struct Symbol* find_symbol(const char* name) {
    unsigned int index = hash(name);
    struct Symbol* current = symbol_table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol Table Entry
struct Symbol {
    char name[256];
    char type[256];
    struct Symbol* next;
};

// Symbol Table (hash table)
#define TABLE_SIZE 100
struct Symbol* symbol_table[TABLE_SIZE];

// Hash function for symbol table
unsigned int hash(const char* str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

// Add variable to symbol table
void add_symbol(const char* name, const char* type) {
    unsigned int index = hash(name);
    struct Symbol* new_symbol = (struct Symbol*)malloc(sizeof(struct Symbol));
    strcpy(new_symbol->name, name);
    strcpy(new_symbol->type, type);
    new_symbol->next = symbol_table[index];
    symbol_table[index] = new_symbol;
}

// Find symbol in symbol table
struct Symbol* find_symbol(const char* name) {
    unsigned int index = hash(name);
    struct Symbol* current = symbol_table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
