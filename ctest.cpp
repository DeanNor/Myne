
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

// TODO make all the error windows here present to a buffer to be displayed

struct load_ast
{
public:
    std::string name;

    virtual void use_editor() = 0;

    virtual ~load_ast()
    {
        
    }

    load_ast(std::string variable_name) : name(variable_name)
    {

    }

    load_ast() = default;
};

struct vector_link
{
public:
    std::vector<load_ast*>* where;
    size_t where_inside;

    std::string variable_name;
    bool is_variable;

    vector_link(std::vector<load_ast*>* vector, size_t index) : where(vector), where_inside(index), is_variable(false)
    {

    }

    vector_link(std::vector<load_ast*>* vector, size_t index, std::string name) : where(vector), where_inside(index), variable_name(name), is_variable(true)
    {

    }
};

struct ast_expands : public load_ast
{
private:
    std::vector<load_ast*>* expandable_values;

public:
    ast_expands(std::string variable_name) : load_ast(variable_name)
    {
        expandable_values = new std::vector<load_ast*>;
    }

    ast_expands()
    {
        expandable_values = new std::vector<load_ast*>;
    }

    void add_to_values(load_ast* val)
    {
        expandable_values->push_back(val);
    }

    vector_link* add_linked()
    {
        size_t size = expandable_values->size();
        expandable_values->push_back(0);

        return new vector_link(expandable_values, size);
    }

    vector_link* add_linked(std::string variable_name)
    {
        size_t size = expandable_values->size();
        expandable_values->push_back(0);

        return new vector_link(expandable_values, size, variable_name);
    }

    virtual void use_editor() override
    {

    }
};

struct ast_expands_complex : public ast_expands
{
public:
    ast_expands_complex(std::string variable_name) : ast_expands(variable_name)
    {
        
    }

    virtual void use_editor() override
    {

    }
};

struct ast_expands_process : public ast_expands
{
public:
    ast_expands_process(std::string variable_name) : ast_expands(variable_name)
    {

    }

    ast_expands_process()
    {

    }

    virtual void use_editor() override
    {
        // TODO as in we can not usefully store processes in variables like this. Maybe do also as an index of children?
    }
};

struct ast_value : public load_ast
{
public:
    std::string type;

    ast_value(std::string variable_name, std::string variable_type) : load_ast(variable_name), type(variable_type)
    {

    }

    virtual void use_editor() override
    {

    }
};

static std::unordered_map<std::string, vector_link*> to_be_linked;

#include "clang-tool.h"

// Searched list needs to be updated with these
// load function names
const static char load_func[] = "load"; const static size_t load_func_size = strlen(load_func);

// Same implementation
const static char load_complex[] = "load_complex"; const static size_t load_complex_size = strlen(load_complex);
const static char load_complex_ptr[] = "load_complex_ptr"; const static size_t load_complex_ptr_size = strlen(load_complex_ptr);

const static char load_data[] = "load_data"; const static size_t load_data_size = strlen(load_data);
const static char load_process[] = "load_process"; const static size_t load_process_size = strlen(load_process);
const static char load_enum[] = "load_enum"; const static size_t load_enum_size = strlen(load_enum);

static std::vector<load_ast*> loads_in_file;

void search_load_out(const char* file_loc, const ref_storer* refs) // TODO inside class and external versions
{
    std::ifstream ifile(file_loc);

    if (!ifile) return; // TODO cache error;

    std::string file((std::istreambuf_iterator<char>(ifile)),std::istreambuf_iterator<char>());
    
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_cpp());

    TSTree *tree = ts_parser_parse_string(parser, NULL, file.c_str(), file.size());
    TSNode root = ts_tree_root_node(tree);

    uint32_t child_count = ts_node_named_child_count(root);
    for (uint32_t i = 0; i < child_count; i++)
    {
        TSNode node = ts_node_named_child(root, i);

        if (ts_node_type(node) == refs->function_def)
        {
            char current = '\1';
            long start = ts_node_start_byte(ts_node_named_child(node, 1));
            long iterator = start;

            while(current != '(' && current != '\0')
            {
                current = file.at(iterator);

                if (current == ':' && file.at(iterator + 1) == ':') // TODO overflow? also below?
                {
                    if (strncmp(file.c_str() + iterator + 2, load_func, load_func_size) == 0)
                    {
                        std::string class_name;
                        class_name.resize(iterator - start);

                        for (int x = start, y = 0; x < iterator; x++, y++)
                        {
                            class_name.at(y) = file.at(x);
                        }

                        loads_in_file.push_back(new ast_expands(class_name));
                    }
                }

                iterator++;
            }
        }
    }

    ts_tree_delete(tree);
    ts_parser_delete(parser);
}

int main(void)
{
    ref_storer* refs = construct_storer();

    search_load_out("./.cpp/object.cpp", refs);

    search_load_out("./.cpp/drawobj.cpp", refs);

    for (size_t x = 0; x < load_classes.size(); x++)
    {
        std::cout << load_classes.at(x) << std::endl;
    }

    free(refs);
 
    return 0;
}
