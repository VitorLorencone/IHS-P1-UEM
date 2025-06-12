// File made by myself (Vitor) while searching.
// Some sources:
// https://en.wikipedia.org/wiki/Edit_distance
// https://en.wikipedia.org/wiki/Levenshtein_distance
// https://en.wikipedia.org/wiki/Longest_common_subsequence
// https://en.wikipedia.org/wiki/Hamming_distance
// https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
// https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance
// https://gist.github.com/badocelot/5331587

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
    if (s1[m - 1] == s2[n - 1]) return 1 + lcs_rec(s1, s2, m - 1, n - 1);
    else return max(lcs_rec(s1, s2, m, n - 1), lcs_rec(s1, s2, m - 1, n));
}
int lcs(char s1[],char s2[]){
    int m = strlen(s1);
    int n = strlen(s2);
    
    return lcs_rec(s1,s2,m,n);
}

// hamming distance
int hamming_dist(const char *s1, const char *s2) {
    int dist = 0;
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    if (len1 != len2) {
        return -1;
    }
    for (int i = 0; i < len1; i++) {
        if (s1[i] != s2[i]) {
            dist++;
        }
    }
    return dist;
}

// damerau–levenshtein distance
int dld (const char *s1, const char *s2)
{
    int m = 0;
    if (s1 != NULL)
        m = strlen(s1);
        
    int n = 0;
    if (s2 != NULL)
        n = strlen(s2);
    
    if (m == 0)
        return n;
    if (n == 0)
        return m;
    
    int **matrix = (int**) malloc ((m + 2) * sizeof (int*));
    if (matrix == NULL)
        abort();

    for (int i = 0; i < m + 2; i++)
    {
        matrix[i] = (int*) malloc ((n + 2) * sizeof (int));

        if (matrix[i] == NULL)
            abort();
    }

    int INF = m + n;
    
    matrix[0][0] = INF;
    for (int i = 0; i <= m; i++)
    {
        matrix[i+1][1] = i;
        matrix[i+1][0] = INF;
    }
    for (int j = 0; j <= n; j++)
    {
        matrix[1][j+1] = j;
        matrix[0][j+1] = INF;
    }

    int *last_row = (int*) malloc (256 * sizeof (int));
    
    for (int d = 0; d < 256; d++)
        last_row[d] = 0;

    for (int row = 1; row <= m; row++)
    {
        unsigned char ch_s = s1[row-1];
        
        int last_match_col = 0;

        for (int col = 1; col <= n; col++)
        {

            unsigned char ch_t = s2[col-1];
            
            int last_matching_row = last_row[ch_t];

            int cost = (ch_s == ch_t) ? 0 : 1;

            int dist_add = matrix[row][col+1] + 1;
            int dist_del = matrix[row+1][col] + 1;
            int dist_sub = matrix[row][col] + cost;
            int dist_trans = matrix[last_matching_row][last_match_col]
                             + (row - last_matching_row - 1) + 1
                             + (col - last_match_col - 1);

            int min = dist_add;
            if (dist_del < min)
                min = dist_del;
            if (dist_sub < min)
                min = dist_sub;
            if (dist_trans < min)
                min = dist_trans;

            matrix[row+1][col+1] = min;

            if (cost == 0)
                last_match_col = col;
        }

        last_row[ch_s] = row;
    }

    int result = matrix[m+1][n+1];
    
    for (int i = 0; i < m + 2; i++)
        free (matrix[i]);
    free (matrix);
    free (last_row);

    return result;
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
    double jaro = jaro_dist(s1, s2);
    int prefix = 0;
    for (int i = 0; i < min(4, min(strlen(s1), strlen(s2))); i++) {
        if (s1[i] == s2[i]) {
            prefix++;
        } else {
            break;
        }
    }
    return jaro + prefix * 0.1 * (1.0 - jaro);
}