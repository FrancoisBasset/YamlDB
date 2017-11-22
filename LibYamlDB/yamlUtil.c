#include <stdio.h>
#include <string.h>

#include "yamlUtil.h"

void trim(char* string) {
    int length = strlen(string);

    int begin = 0;
    int end = length - 1;

    while (string[begin] == ' ') {
        begin++;
    }

    while (string[end] == ' ') {
        end--;
    }

    for (int i = 0; i < end; i++) {
        string[i] = string[begin + i];
    }

    string[end - begin + 1] = '\0';
}