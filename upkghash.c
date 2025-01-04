#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "upkglib.h"
#include "upkghash.h"

Entry *hashTable[TABLE_SIZE];
int hash(char *key) {
    int hashValue = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashValue += key[i];
    }
    return hashValue % TABLE_SIZE;
}
void addEntry(char *name, char *version, char *release) {
    int index = hash(name);
    Entry *newEntry = (Entry *)malloc(sizeof(Entry));
    strcpy(newEntry->name, name);
    strcpy(newEntry->version, version);
    strcpy(newEntry->release, release);
    newEntry->next = hashTable[index];
    hashTable[index] = newEntry;
}
Entry *searchEntry(char *name) {
    int index = hash(name);
    Entry *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void deleteEntry(char *name) {
    int index = hash(name);
    Entry *current = hashTable[index];
    Entry *prev = NULL;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}
void list() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = hashTable[i];
        while (current != NULL) {
            printf("%s-%s-%s\n", current->name, current->version, current->release);
            current = current->next;
        }
    }
}
void glob() {
    printf("\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry *current = hashTable[i];
        while (current != NULL) {
            printf("%s-%s-%s ", current->name, current->version, current->release);
            current = current->next;
        }
        //printf("\n");
    }
printf("\n");
}
void startsearch() {
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
    }
    printf("New string: \n%s\n", new_str);
    //free(new_str);
}
void testhash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
    addEntry("bash", "1.2", "1");
    addEntry("nano", "2.0", "1");
    addEntry("binutils", "1.1", "1");
    addEntry("coreutils", "1.2", "1");
    addEntry("findutils", "2.0", "1");
    addEntry("util-linux", "1.1", "1");
    printf("Search for nano: \n");
    Entry *nano = searchEntry("nano");
    printf("%s\n", nano->name);
    Entry *found = searchEntry("nano");
    if (found != NULL) {
        printf("%s-%s-%s\n", found->name, found->version, found->release);
    } else {
        printf("Not found\n");
    }
    list();
    deleteEntry("nano");
    glob();
}
