#include <iostream>
#include <vector>

#include "game.hpp"
#include "drawobj.hpp"
#include "blendobj.hpp"
#include "collobj.hpp"
#include "joint.hpp"

class Float : public BlendObj
{
protected:
    pos offset;

    pos normal;

    display* window = nullptr;

public:
    Float()
    {
        window = get_current_game()->game_window;
        normal = window->size;
    }

    virtual void _process(double delta)
    {
        BlendObj::_process(delta);

        pos window_zero = window->center - window->half_size;
        position = window_zero + offset.scaled(normal, window->size);

        if (global_position.parent != nullptr)
        {
            position -= *global_position.parent->position;
        }
    }

    void set_position(pos new_position) = delete;

    void set_offset(pos new_offset)
    {
        offset = new_offset;
    }

    pos get_offset()
    {
        return offset;
    }

    pos get_normal()
    {
        return normal;
    }
};

class FloatScaled : public Float // Size is more of a readable value than usable, use scale instead
{
protected:
    pos scale;

    bool parent_scope = false;
    BlendObj* scalar_parent = nullptr;

public:
    virtual void _process(double delta)
    {
        Float::_process(delta);

        if (parent_scope)
        {
            size = scale.scaled(normal, scalar_parent->get_size());
        }
        
        else
        {
            size = scale.scaled(normal, window->size);
        }
    }

    void set_parent(Process* new_parent)
    {
        Float::set_parent(new_parent);

        BlendObj* par = dynamic_cast<BlendObj*>(parent);
        scalar_parent = par;
    }

    void set_normal(pos new_normal)
    {
        normal = new_normal;
    }

    pos get_normal()
    {
        return normal;
    }

    void set_size(pos) = delete;

    void set_scale(pos new_scale)
    {
        scale = new_scale;
    }

    pos get_scale()
    {
        return scale;
    }
};

#include <fstream>
#include <filesystem>
#include <map>

namespace AUTOGEN
{
    /*
    Finds any .hpp file with a class or struct in it, to comment out a class or struct, put a comment flag around it, such as
        class //
        A
    or
        class / ** /
        B
    */

    void find_and_create_domain();

    std::vector<std::ifstream> recursive_find_files(std::filesystem::path path, std::vector<std::string>& names);

    void recursive_find_files(std::vector<std::ifstream>& search_files, std::filesystem::path path, std::vector<std::string>& names);

    std::string get_class_in_file(std::string file, std::string search_term, size_t& end, size_t start = 0ul);
    
    void create_domain_file(std::vector<std::string> type_names); // Creates a file that contains creator functions for all classes in the project

    void create_include_file(std::vector<std::string> files); // Creates a file that contains the includes for the domain file

    void find_and_create_domain()
    {
        std::vector<std::string> names;
        std::vector<std::ifstream> search_files = recursive_find_files(std::filesystem::current_path(), names);
        std::vector<std::string> classes;

        size_t end = 0ul, end2 = 0ul;
        while (false) // for search files
        {
            std::string file_contents = "HI"; // Read file into file_contents
            
            while (!(end == file_contents.npos && end2 == file_contents.npos))
            {
                std::string class_string = get_class_in_file(file_contents, "class ", end, end);
            
                if (end == file_contents.npos)
                {
                    class_string = get_class_in_file(file_contents, "struct ", end2, end2);
                }

                if (class_string.size() != 0ul)
                {
                    classes.push_back(class_string);
                }
            }

            end = 0ul;
            end2 = 0ul;
        }

        // Remove duplicates from classes

        std::filesystem::create_directory(".autogen");

        create_include_file(names);
        create_domain_file(classes);
    }

    std::vector<std::ifstream> recursive_find_files(std::filesystem::path path, std::vector<std::string>& names)
    {
        std::vector<std::ifstream> files;

        recursive_find_files(files, path, names);

        return files;
    }

    void recursive_find_files(std::vector<std::ifstream>& search_files, std::filesystem::path path, std::vector<std::string>& names)
    {
        for (const std::filesystem::__cxx11::directory_entry& entry : std::filesystem::directory_iterator(path))
        {
            std::filesystem::__cxx11::path entry_filename = entry.path().filename();
            if (entry.is_directory())
            {
                if (!(entry_filename == "build" || entry_filename == ".git" || entry_filename == ".vscode" || entry_filename == ".autogen"))
                {
                    recursive_find_files(search_files, entry.path(), names);
                }
            }

            else if (entry_filename.extension() == ".hpp")
            {
                search_files.push_back(std::ifstream(entry.path()));
            }
        }
    }

    std::string get_class_in_file(std::string file, std::string search_term, size_t& end, size_t start)
    {
        end = file.find(search_term, start);
        
        std::string class_name;
        if (end != file.npos)
        {
            end += search_term.size();
            char at = file.at(end++);

            while (at != '{' && at != ':' && at != ';' && at != '/' && end != file.size())
            {
                if (at != '\n' && at != ' ')
                {
                    class_name += at;
                }

                at = file.at(end);

                ++end;
            }
        }

        return class_name;
    }
    
    void create_domain_file(std::vector<std::string> type_names) // Creates a file that contains creator functions for all classes in the project
    {
        std::ofstream header_file(".autogen/domain.hpp");

        std::ofstream cpp_file(".autogen/domain.cpp");

        header_file << "#pragma once\n#include \"include.hpp\"\n";
        cpp_file << "#include \"domain.hpp\"\n";

        for (std::string class_name : type_names)
        {
            std::string function_def = class_name + "* get_" + class_name + "()";
            header_file << function_def << ";\n";

            cpp_file << function_def << "{return new " << class_name << ";}\n";
        }

        header_file.close();
        cpp_file.close();
    }

    void create_include_file(std::vector<std::string> files) // Creates a file that contains the includes for the domain file
    {
        std::ofstream include_file(".autogen/include.hpp");

        for (std::string file : files)
        {
            include_file << "#include \"" << file << "\"\n";
        }

        include_file.close();
    }
}

int main()
{
    game gameplay;
    set_current_game(&gameplay);
    gameplay.physics = false;

    AUTOGEN::find_and_create_domain();

    pos window_size = gameplay.game_window->size;

    FloatScaled* drawer = new FloatScaled;
    FloatScaled* drawer2 = new FloatScaled;
    drawer->set_offset({0,0});
    drawer2->set_offset({800,0});

    BLImage img(500, 1000, BLEND_FORMAT);

    BLContext ctx(img);
    ctx.clearAll();

    BLGradient linear(
    BLLinearGradientValues(0, 0, 0, 1000));
    linear.addStop(0.0, BLRgba32(0xFFBBDEF0));
    linear.addStop(1.0, BLRgba32(0xFF324376));

    ctx.fillRect(
    BLRect(0, 0, 500, 1000), linear);
    ctx.end();

    drawer->set_image(img);
    drawer->set_scale({200,500});

    drawer2->set_image(img);
    drawer2->set_scale({200,500});

    gameplay.root->add_child(drawer);
    gameplay.root->add_child(drawer2);

    FloatScaled* drawer3 = new FloatScaled;
    
    BLImage img2(500, 1000, BLEND_FORMAT);

    BLContext ctx2(img2);
    ctx2.clearAll();

    BLGradient linear2(
    BLLinearGradientValues(0, 0, 0, 1000));
    linear2.addStop(0.0, BLRgba32(0xFFFF0000));

    ctx2.fillRect(
    BLRect(0, 0, 500, 1000), linear);

    ctx2.fillRect(
    BLRect(50, 50, 400, 900), linear2);
    ctx2.end();

    drawer3->set_image(img2);
    drawer2->add_child(drawer3);
    drawer3->set_offset({30,30});
    drawer3->set_scale({30,30});

    gameplay.start();

    return 0;
}