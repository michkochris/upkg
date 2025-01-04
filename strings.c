Here's an example C function that edits multiple strings and tests to see if the editing was successful:

```
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STRINGS 5
#define MAX_STRING_LENGTH 100

// Function to edit multiple strings
bool edit_strings(char *strings[], int num_strings, const char *old_substring, const char *new_substring) {
    bool editing_successful = true;

    for (int i = 0; i < num_strings; i++) {
        char *string = strings[i];
        char temp_string[MAX_STRING_LENGTH];

        // Create a temporary copy of the original string
        strcpy(temp_string, string);

        // Replace the old substring with the new substring
        char *ptr = strstr(string, old_substring);
        if (ptr != NULL) {
            int length = strlen(new_substring) - strlen(old_substring);
            memmove(ptr + length, ptr + strlen(old_substring), strlen(ptr + strlen(old_substring)) + 1);
            memcpy(ptr, new_substring, strlen(new_substring));
        } else {
            editing_successful = false;
        }

        // Check if the editing was successful
        if (strcmp(string, temp_string) == 0) {
            editing_successful = false;
        }
    }

    return editing_successful;
}

int main() {
    char *strings[] = {"Hello, world!", "This is a test.", "Replace me!", "Nothing to replace here.", "Another test string."};
    int num_strings = sizeof(strings) / sizeof(strings[0]);
    const char *old_substring = "test";
    const char *new_substring = "example";

    bool editing_successful = edit_strings(strings, num_strings, old_substring, new_substring);

    printf("Editing successful: %s\n", editing_successful ? "true" : "false");

    // Print the edited strings
    for (int i = 0; i < num_strings; i++) {
        printf("String %d: %s\n", i + 1, strings[i]);
    }

    return 0;
}
