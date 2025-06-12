#include <stdio.h>
#include <stdlib.h>
#include "duktape.h"
#include "edit_distance.h"

duk_ret_t native_print(duk_context *ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

duk_ret_t js_levenshtein_dist(duk_context *ctx){
    const char *s1 = duk_require_string(ctx, 0);
    const char *s2 = duk_require_string(ctx, 1);
    int dist = levenshtein_dist(s1, s2);
    duk_push_int(ctx, dist);
    return 1;
}

int main(int argc, char const *argv[]) {
    duk_context *ctx = duk_create_heap_default();
    if (!ctx) {
        printf("Erro ao criar contexto Duktape.\n");
        return 1;
    }

    duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
    duk_push_c_function(ctx, js_levenshtein_dist, 2);
    duk_put_global_string(ctx, "levenshtein_dist");

    const char *js_code = "levenshtein_dist('kitten', 'sitting')";

    duk_eval_string(ctx, js_code);

    const char *result = duk_safe_to_string(ctx, -1);

    printf("%s       ", result);

    duk_pop(ctx);
    duk_destroy_heap(ctx);

    return 0;
}
