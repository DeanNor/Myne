
#pragma once

#include "hash.hpp"

#include <iostream>
#include <fstream>
#include <string>

#include "load_save_requirements.hpp"

class Process;

// Saves binary data with system endianness
class Saver
{
private:
    std::ofstream file;

public:
    Saver() = default;

    Saver(const char* where);

    Saver(std::string where);

    void open(const char* where);

    void open(std::string where);

    template <VAR T>
    void save_data(T data)
    {
        file.write((const char*)(&data), sizeof(T));
    }

    template <>
    void save_data<bool>(bool data)
    {
        if (data) file << '\1';
        else file << '\0';
    }

    template <typename T>
    void save_enum(T data)
    {
        file.write((const char*)(&data), sizeof(int));
    }

    template<CLS T>
    void save_complex(T data)
    {
        save_data<hash_t>(data._get_type_hash().value);

        data.save(this);
    }

    template <>
    void save_complex<std::string>(std::string data)
    {
        file << data;
        file << '\0';
    }

    template<CLS_PTR T>
    void save_complex_ptr(T data)
    {
        save_data<hash_t>(data->get_type_hash().value);
        data->save(this);
    }

    void save_process(Process* data);
};