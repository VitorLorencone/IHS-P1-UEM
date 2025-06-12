#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// levenshtein distance
int levenshtein_dist(const char *s1, const char *s2);

// longest common subsequence
int lcs_rec(char s1[], char s2[], int m, int n);
int lcs(char s1[], char s2[]);

// hamming distance
int hamming_dist(char s1[], char s2[]);

// damerau–levenshtein distance
int dld_rec(const char *s1, const char *s2, int len1, int len2);
int dld(const char *s1, const char *s2);

// jaro distance
double jaro_dist(const char *s1, const char *s2);
double jaro_winkler(const char *s1, const char *s2);