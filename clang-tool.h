
#include "tree_sitter/api.h"

#include <string.h>

// Pointer location storer for tree_sitter strings since none of them are externally accessable
struct ref_storer
{
    const char* function_def;
    const char* declarator;
};

#ifdef __cplusplus
extern "C" {
#endif

// Malloced and must be freed.
// Constructs a ref_storer struct that knows all of the string pointers needed for tree-sitter
struct ref_storer* construct_storer(void);

const TSLanguage* tree_sitter_cpp(void);

#ifdef __cplusplus
}
#endif
