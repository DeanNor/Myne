
#ifndef __TS_TOOL
#define __TS_TOOL

#include "tree_sitter/api.h"

// Pointer location storer for tree_sitter strings since none of them are externally accessable
struct ref_storer
{
    const char* function_def;
    const char* declarator;
    const char* compound_statement;

    const char* expr_stmt;
    const char* call_expr;
    const char* assignment_expression;

    const char* function_declarator;
    const char* qualified_identifier;
    const char* namespace_identifier;
    const char* identifier;

    const char* class_specifier;
    const char* struct_specifier;
    const char* type_identifier;
    const char* field_declaration_list;
    const char* declaration;
    const char* parameter_list;
    const char* parameter_declaration;

    const char* field_identifier;

    const char* base_class_clause;

    const char* template_method;
    const char* template_argument_list;
    const char* type_descriptor;
    const char* field_expression;
};

#ifdef __cplusplus
extern "C" {
#endif

// Malloc-ed and must be freed.
// Constructs a ref_storer struct that knows all of the string pointers needed for tree-sitter
struct ref_storer* construct_storer(void);

const TSLanguage* tree_sitter_cpp(void);

#ifdef __cplusplus
}
#endif

#endif
