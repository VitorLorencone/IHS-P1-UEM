#include <stdio.h>
#include <stdlib.h>
#include "duktape.h"
#include "edit_distance.h"

// Path to js file, remove comments for other algorithms
//const char *path_js = "./scripts/dld.js";
//const char *path_js = "./scripts/hamming.js";
//const char *path_js = "./scripts/jaro.js";
//const char *path_js = "./scripts/lcs.js";
const char *path_js = "./scripts/levenshtein.js";

// Function needed for reading a file to a string in C.
// Duktape v2.x does NOT have support for direct file manipulation for compatibility reasons.
char *read_file_string(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buf = (char *)malloc(size + 1);
    if (!buf) { fclose(f); return NULL; }

    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
}

// Wrapper function for print in the stdin.
// Duktape v2.x does NOT have support for direct print in standard input for compatibility reasons.
duk_ret_t native_print(duk_context *ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

// Wrapper function for the levenshtein dist in edit_distance.c
duk_ret_t js_levenshtein_dist(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    int dist = levenshtein_dist(s1, s2);
    duk_push_int(ctx, dist);
    return 1;
}

// Wrapper function for the dld in edit_distance.c
duk_ret_t js_dld(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    int dist = dld(s1, s2);
    duk_push_int(ctx, dist);
    return 1;
}

// Wrapper function for the hamming dist in edit_distance.c
duk_ret_t js_hamming_dist(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    int dist = hamming_dist(s1, s2);
    duk_push_int(ctx, dist);
    return 1;
}

// Wrapper function for the jaro dist in edit_distance.c
duk_ret_t js_jaro_dist(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    double dist = jaro_dist(s1, s2);
    duk_push_number(ctx, dist);
    return 1;
}

// Wrapper function for the lcs in edit_distance.c
duk_ret_t js_lcs(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    char s1_var[100];
    char s2_var[100];
    strcpy(s1_var, s1);
    strcpy(s2_var, s2);
    int dist = lcs(s1_var, s2_var);
    duk_push_int(ctx, dist);
    return 1;
}

int main(int argc, char const *argv[]) {
    // Creating context in js
    duk_context *ctx = duk_create_heap_default();
    if (!ctx) return 1;

    // Adding function for duktape to use
    duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");

    duk_push_c_function(ctx, js_levenshtein_dist, 2);
    duk_put_global_string(ctx, "levenshtein_dist");

    duk_push_c_function(ctx, js_dld, 2);
    duk_put_global_string(ctx, "dld");

    duk_push_c_function(ctx, js_hamming_dist, 2);
    duk_put_global_string(ctx, "hamming_dist");

    duk_push_c_function(ctx, js_jaro_dist, 2);
    duk_put_global_string(ctx, "jaro_dist");

    duk_push_c_function(ctx, js_lcs, 2);
    duk_put_global_string(ctx, "lcs");

    // read js code
    const char *js_code = read_file_string(path_js);

    // eval js code
    duk_eval_string(ctx, js_code);

    // end context
    duk_pop(ctx);
    duk_destroy_heap(ctx);

    return 0;
}