// File made by myself (Vitor) while searching.
// Some sources:
// https://en.wikipedia.org/wiki/Edit_distance
// https://en.wikipedia.org/wiki/Levenshtein_distance
// https://en.wikipedia.org/wiki/Longest_common_subsequence
// https://en.wikipedia.org/wiki/Hamming_distance
// https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
// https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance

#include "edit_distance.h"

// aux max 2 funcion
int max(int a, int b) {
  	return a > b ? a : b; 
}

// aux min 2 function
int min(int a, int b) {
  	return a < b ? a : b; 
}

// aux max 3 funcion
int max3(int a, int b, int c) {
    int m = a > b ? a : b;
    return m > c ? m : c;
}

// aux min 3 function
int min3(int a, int b, int c) {
    int m = a < b ? a : b;
    return m < c ? m : c;
}

// levenshtein distance
int levenshtein_dist(const char *s1, const char *s2){
    if(strcmp(s1, "") == 0) return strlen(s2);
    if(strcmp(s2, "") == 0) return strlen(s1);
    if(*s1 == *s2) return levenshtein_dist(s1+1, s2+1);

    int deletion = levenshtein_dist(s1+1, s2);
    int insertion = levenshtein_dist(s1, s2+1);
    int substitution = levenshtein_dist(s1+1, s2+1);

    return min3(deletion, insertion, substitution) + 1;
}

// longest common subsequence
int lcs_rec(char s1[], char s2[], int m, int n) {
    if (m == 0 || n == 0) return 0;
    if (s1[m - 1] == s2[n - 1]) return 1 + lcsRec(s1, s2, m - 1, n - 1);
    else return max(lcsRec(s1, s2, m, n - 1), lcsRec(s1, s2, m - 1, n));
}
int lcs(char s1[],char s2[]){
    int m = strlen(s1);
    int n = strlen(s2);
    
    return lcsRec(s1,s2,m,n);
}

// hamming distance
int hamming_dist(char s1[], char s2[])
{
    int i = 0, count = 0;
    while(strcmp(*s1, "") == 0)
    {
        if (s1[i] != s2[i])
            count++;
        i++;
    }
    return count;
}

// damerau–levenshtein distance
int dld_rec(const char *s1, const char *s2, int len1, int len2) {
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;
    int cost = (s1[len1 - 1] == s2[len2 - 1]) ? 0 : 1;
    int res = min3(
        dld_rec(s1, s2, len1 - 1, len2) + 1,        
        dld_rec(s1, s2, len1, len2 - 1) + 1,        
        dld_rec(s1, s2, len1 - 1, len2 - 1) + cost
    );
    if (len1 > 1 && len2 > 1 &&
        s1[len1 - 1] == s2[len2 - 2] &&
        s1[len1 - 2] == s2[len2 - 1]) {
        int trans = dld_rec(s1, s2, len1 - 2, len2 - 2) + 1;
        if (trans < res) res = trans;
    }
    return res;
}
int dld(const char *s1, const char *s2) {
    return dld_rec(s1, s2, strlen(s1), strlen(s2));
}

// jaro distance
double jaro_dist(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (len1 == 0 && len2 == 0) return 1.0;
    if (len1 == 0 || len2 == 0) return 0.0;

    int match_distance = max(len1, len2) / 2 - 1;

    int *s1_matches = calloc(len1, sizeof(int));
    int *s2_matches = calloc(len2, sizeof(int));

    int matches = 0;
    for (int i = 0; i < len1; i++) {
        int start = max(0, i - match_distance);
        int end = min(i + match_distance + 1, len2);

        for (int j = start; j < end; j++) {
            if (s2_matches[j]) continue;
            if (s1[i] != s2[j]) continue;
            s1_matches[i] = 1;
            s2_matches[j] = 1;
            matches++;
            break;
        }
    }

    if (matches == 0) {
        free(s1_matches);
        free(s2_matches);
        return 0.0;
    }

    int t = 0;
    for (int i = 0, k = 0; i < len1; i++) {
        if (!s1_matches[i]) continue;
        while (!s2_matches[k]) k++;
        if (s1[i] != s2[k]) t++;
        k++;
    }

    free(s1_matches);
    free(s2_matches);

    double m = matches;
    return ((m / len1) + (m / len2) + ((m - t / 2.0) / m)) / 3.0;
}

double jaro_winkler(const char *s1, const char *s2) {
    double jaro = jaro_distance(s1, s2);
    int prefix = 0;
    for (int i = 0; i < MIN(4, MIN(strlen(s1), strlen(s2))); i++) {
        if (s1[i] == s2[i]) {
            prefix++;
        } else {
            break;
        }
    }
    return jaro + prefix * 0.1 * (1.0 - jaro);
}