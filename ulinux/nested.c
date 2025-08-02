#include <stdio.h>

int main() {
    int outer_variable = 10;

    // Nested function definition (GCC extension)
    void inner_function() {
        printf("Inner function: outer_variable = %d\n", outer_variable);
    }

    printf("Outer function: before calling inner_function\n");
    inner_function(); // Calling the nested function
    printf("Outer function: after calling inner_function\n");

    return 0;
}
