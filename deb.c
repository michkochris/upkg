/*
author: michkochris
email: michkochris@gmail.com
date: started 12-31-2024
license: GPLV3
notice: This program is free software:
you can redistribute it and/or modify it
under the terms of the GNU General Public Lic>
Only the name of the program is copyrighted...
If you reuse code, please give credits...
file description:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "upkglib.h"

#define TABLE_SIZE 10000  // Size of the hash table
typedef struct Contact {
    char name[50];
    char phoneNumber[15];
    struct Contact *next;
} Contact;
typedef struct HashTable {
    Contact *table[TABLE_SIZE];
} HashTable;
// Simple hash function (can be improved for better distribution)
int hash(char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        sum += name[i];
    }
    return sum % TABLE_SIZE;
}
// Initialize the hash table
HashTable *createHashTable() {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->table[i] = NULL;
    }
    return table;
}
// Add a contact to the phonebook
void addContact(HashTable *table, char *name, char *phoneNumber) {
    int index = hash(name);
    Contact *newContact = (Contact *)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phoneNumber, phoneNumber);
    newContact->next = table->table[index];
    table->table[index] = newContact;
}
// Search for a contact by name
Contact *findContact(HashTable *table, char *name) {
    int index = hash(name);
    Contact *current = table->table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
// Print the entire phonebook
void printPhonebook(HashTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact *current = table->table[i];
        while (current != NULL) {
            printf("%s: %s\n", current->name, current->phoneNumber);
            current = current->next;
        }
    }
}

int main(int argc, char *argv[]) {
if (argc != 2) {
    fprintf(stderr, "Usage: %s <deb_file>\n", argv[0]);
    return 1;
    }
    extract_deb(argv[1], "installdir");
    extract_tar_xz("installdir/control.tar.xz", "installdir");
    char *pkg = search_file("installdir/control", "Package: ");
    if (pkg != NULL) {
        printf("\n%s\n", pkg);
        //free(pkg);
    } else {
        printf("String not found.\n");
    }
    char *new_str = rmstr(pkg, "Package: ");
    if (new_str == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    printf("New string: \n%s\n", new_str);
    //free(new_str);
    HashTable *phonebook = createHashTable();
    addContact(phonebook, "Alice", "123-4567");
    addContact(phonebook, "Bob", "987-6543"); 
    Contact *found = findContact(phonebook, "Alice");
    if (found) {
        printf("Alice's phone number: %s\n", found->phoneNumber);
    }
    printPhonebook(phonebook);
    
    return 0;
}
