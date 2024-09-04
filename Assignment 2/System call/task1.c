#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *filePtr;
    char filename[100];
    char input[100];

    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    strcpy(filename, argv[1]);
    filePtr = fopen(filename, "a");
    if (filePtr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    printf("Enter strings to write to the file. Enter \"-1\" to stop.\n");

    while (1) {
        printf("Enter string: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "-1") == 0)
            break;
        fprintf(filePtr, "%s\n", input);
    }

    fclose(filePtr);
    printf("Strings have been written to %s.\n", filename);
    return 0;
}
