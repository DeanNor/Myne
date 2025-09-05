

#include "ts_tool.h"

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
    stored->compound_statement = ts_symbol_names[sym_compound_statement];

    stored->expr_stmt = ts_symbol_names[sym_expression_statement];
    stored->call_expr = ts_symbol_names[sym_call_expression];
    stored->assignment_expression = ts_symbol_names[sym_assignment_expression];

    stored->function_declarator = ts_symbol_names[sym_function_declarator];
    stored->qualified_identifier = ts_symbol_names[sym_qualified_identifier];
    stored->namespace_identifier = ts_symbol_names[alias_sym_namespace_identifier];
    stored->declaration = ts_symbol_names[sym_declaration];

    stored->class_specifier = ts_symbol_names[sym_class_specifier];
    stored->struct_specifier = ts_symbol_names[sym_struct_specifier];
    stored->type_identifier = ts_symbol_names[alias_sym_type_identifier];
    stored->field_declaration_list = ts_symbol_names[sym_field_declaration_list];
    stored->identifier = ts_symbol_names[sym_identifier];
    stored->parameter_list = ts_symbol_names[sym_parameter_list];
    stored->parameter_declaration = ts_symbol_names[sym_parameter_declaration];

    stored->field_identifier = ts_symbol_names[alias_sym_field_identifier];

    stored->base_class_clause = ts_symbol_names[sym_base_class_clause];

    stored->template_method = ts_symbol_names[sym_template_method];
    stored->template_argument_list = ts_symbol_names[sym_template_argument_list];
    stored->type_descriptor = ts_symbol_names[sym_type_descriptor];
    stored->field_expression = ts_symbol_names[sym_field_expression];

    return stored;
}
