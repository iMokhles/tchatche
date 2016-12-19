#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int regex_match(const char* message, const char* regex_string); // 0 = erreur | 1

void regex_replace_first(const char* message, const char* regex_string);