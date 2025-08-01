

#include "clang-tool.h"

#include <stdlib.h>

#include "tree-sitter-cpp/src/scanner.c"
#include "tree-sitter-cpp/src/parser.c" // Top 10 coding moments
                                        // We call it an extension :/
// In my defense it is incredibly inefficient to rely on strncmps when I can pointer compare.
// wait, don't strncmps take like no time at all?

struct ref_storer* construct_storer(void)
{
    struct ref_storer* stored = malloc(sizeof(struct ref_storer));

    stored->function_def = ts_symbol_names[sym_function_definition];
    stored->declarator = ts_field_names[field_declarator];

    return stored;
}