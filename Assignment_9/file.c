#define _CRT_SECURE_NO_WARNINGS

#include "file.h"
#include <stdio.h>
#include <string.h>

bool openOrCreateFile(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        file = fopen(path, "w");
        if (file == NULL) {
            printf("Error: Could not create the file.\n");
            return false;
        }
        else {
            printf("File created: %s\n", path);
        }
    }
    else {
        printf("File exists: %s\n", path);
    }
    fclose(file);
    return true;
}

void displayFileContent(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
}

void appendStringToFile(const char* path, const char* str) {
    FILE* file = fopen(path, "a");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return;
    }
    fprintf(file, "%s\n", str);
    fclose(file);
}

bool findStringInFile(const char* path, const char* str) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error: Could not open the file.\n");
        return false;
    }
    char line[256];
    int lineNumber = 0;
    bool found = false;
    while (fgets(line, sizeof(line), file) != NULL) {
        lineNumber++;
        char* pos = strstr(line, str);
        if (pos != NULL) {
            int positionInLine = pos - line + 1;
            printf("Found \"%s\" at line %d, position %d.\n", str, lineNumber, positionInLine);
            found = true;
            break;
        }
    }
    if (!found) {
        printf("Not found!\n");
    }
    fclose(file);
    return found;
}
