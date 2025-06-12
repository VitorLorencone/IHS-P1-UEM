#include "edit_distance.h"

int levenshtein_dist(const char *s1, const char *s2){
    if(strcmp(s1, "") == 0) return strlen(s2);
    if(strcmp(s2, "") == 0) return strlen(s1);
    if(*s1 == *s2) return levenshtein_dist(s1+1, s2+1);

    int deletion = levenshtein_dist(s1+1, s2);
    int insertion = levenshtein_dist(s1, s2+1);
    int substitution = levenshtein_dist(s1+1, s2+1);

    int min = deletion;
    if (insertion < min) min = insertion;
    if (substitution < min) min = substitution;

    return min + 1;
}