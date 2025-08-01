
#pragma once

#include ".hpp/tube.hpp"
#include ".hpp/factory.hpp"
#include ".hpp/hash.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include ".hpp/load_save_requirements.hpp"

class Loader
{
public:
    tube* values = nullptr;

    std::ifstream file;

    bool eof = false;

public:
    Loader() = default;

    ~Loader();

    Loader(const char* where);

    Loader(std::string where);

    void open(const char* where);

    void open(std::string where);

    // Enables repeat loading after the file has been read through
    void repeat();

    template <VAR T>
    T load_data()
    {
        if (eof)
        {
            if (values)
            {
                T d = *(T*)(values->get());
                return d;
            }

            else throw std::bad_alloc();
        }
        
        else
        {
            char* data = (char*)malloc(sizeof(T));

            file.read(data, sizeof(T));

            T d = *(T*)data;

            if (values) values->add(data);
            else free(data);

            eof = file.peek() == -1;

            return d;
        }
    }

    // TODO make into load_data_ptr
    template <>
    const char* load_data<const char*>()
    {
        throw std::logic_error("Who wants to make this actually work?");
    }

    template<typename T>
    T load_enum()
    {
        if (eof)
        {
            if (values)
            {
                T d = *(T*)(values->get());
                return d;
            }

            else throw std::bad_alloc();
        }
        
        else
        {
            char* data = (char*)malloc(sizeof(int));

            file.read(data, sizeof(int));

            T d = *(T*)data;

            if (values) values->add(data);
            else free(data);

            eof = file.peek() == -1;

            return d;
        }
    }

    template<CLS T>
    T load_complex()
    {
        if (eof)
        {
            if (values)
            {
                hash* hash_name = (hash*)values->get();

                T* p = (T*)Factory::construct(*hash_name);
                T d = *p;

                delete p;

                d.load(this);

                return d;
            }

            else throw std::bad_alloc();
        }

        else
        {
            if (values)
            {
                hash_t* hash_name = (hash_t*)malloc(sizeof(hash_t));

                file.read((char*)hash_name, sizeof(hash_t));

                values->add(hash_name);

                T* p = (T*)(Factory::construct(*hash_name));

                T d = *p;

                delete p;

                d.load(this);

                eof = file.peek() == -1;

                return d;
            }

            else
            {
                hash* hash_name = new hash(load_data<hash_t>());

                T* p = (T*)(Factory::construct(*hash_name));

                T d = std::move(*p);

                delete p;

                d.load(this);

                eof = file.peek() == -1;

                return d;
            }
        }
    }

    template <>
    std::string load_complex<std::string>()
    {
        if (eof)
        {
            if (values)
            {
                std::string d = (char*)(values->get());
                return d;
            }

            else throw std::bad_alloc();
        }

        else
        {
            char c = '\1';
            std::string d;

            while (c != '\0')
            {
                c = file.get();
                d += c;
            }

            if (values)
            {
                char* c_str = (char*)malloc(d.size() + 1);
                std::memcpy(c_str, d.c_str(), d.size() + 1);

                values->add(c_str);
            }

            eof = file.peek() == -1;

            return d;
        }
    }

    template<CLS_PTR T>
    T* load_complex_ptr()
    {
        if (eof)
        {
            if (values)
            {
                hash* hash_name = (hash*)values->get();

                T* p = (T*)(Factory::construct(*hash_name));

                p->load(this);

                return p;
            }

            else throw std::bad_alloc();
        }

        else
        {
            T* p;

            if (values)
            {
                hash_t* hash_name = (hash_t*)malloc(sizeof(hash_t));

                file.read((char*)hash_name, sizeof(hash_t));

                values->add(hash_name);

                p = (T*)(Factory::construct(*hash_name));
            }

            else
            {
                hash hash_name = load_data<hash_t>();

                p = (T*)(Factory::construct(hash_name));
            }

            p->load(this);

            eof = file.peek() == -1;

            return p;
        }
    }

    Process* load_process(); // Expanded in process.hpp
};