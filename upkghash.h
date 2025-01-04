#ifndef UPKHASH_H
#define UPKHASH_H
#define TABLE_SIZE 20
typedef struct Entry {
    char name[50];
    char version[10];
    char release[20];
    struct Entry *next;
} Entry;
int hash(char *key);
void addEntry(char *name, char *version, char *release);
void deleteEntry(char *name);
void list();
void glob();
void startsearch();
void testhash();
#endif
