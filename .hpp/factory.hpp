
#pragma once

#include <unordered_map>
#include <functional>

#include "hash.hpp"

class Process;

class Factory
{
private:
    // Throws same as map would throw for errors
    static std::unordered_map<hash, std::function<void*()>>& get_complex_constructors()
    {
        static std::unordered_map<hash, std::function<void*()>> complex_constructors;
        return complex_constructors;
    }

    // Throws same as map would throw for errors
    static std::unordered_map<hash, std::function<Process*()>>& get_process_constructors()
    {
        static std::unordered_map<hash, std::function<Process*()>> process_constructors;
        return process_constructors;
    }

public:
    static void* construct(const hash name);

    static Process* construct_process(const hash name);

    static bool _add_constructor(const std::function<void*()>& constructor, const hash& name)
    {
        get_complex_constructors()[name] = constructor;

        return 1;
    }

    static bool _add_process_constructor(const std::function<Process*()>& constructor, const hash& name)
    {
        get_process_constructors()[name] = constructor;

        return 1;
    }
};

// To be stored a type needs a void load(const data* root) and void save(const Saver* os) function, along with a void onload(), all of which are virtual for inheriting types.
// At minimum, the type needs to be named (NAME_TYPE(type)), and to store a stack version it needs to have a ASSIGN_*_CONSTRUCTOR(type) macro, which will use NAME_TYPE(type)

// Viratual and non virtual naming
#define NAME_TYPE(type) private: static constexpr hash _hash_val = hash(#type); public: constexpr const hash& _get_type_hash() const {return _hash_val;}
#define VIR_NAME_TYPE(type) private: static constexpr hash _hash_val = hash(#type); public: virtual const hash& _get_type_hash() const override {return _hash_val;}
#define VIR_NAME_TYPE_OVERRIDE(type) private: static constexpr hash _hash_val = hash(#type); public: virtual const hash& _get_type_hash() const {return _hash_val;}

#define CREATE_TYPE(type) public: static type* create() {type* t = new type(); t->onload(); return t;};

#define EXTERNAL_NAME_TYPE(type) constexpr const hash& _get_##type##_hash() {static constexpr hash hash_val = hash(#type); return hash_val;}

// Macro for non-process based stuff.
#define ASSIGN_VAR_CONSTRUCTOR(type)\
NAME_TYPE(type)\
private:\
static bool _register_##type(){\
Factory::_add_constructor([]()->type*{return new type();},_hash_val);\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

#define ASSIGN_VIR_VAR_CONSTRUCTOR(type)\
VIR_NAME_TYPE(type)\
private:\
static bool _register_##type(){\
Factory::_add_constructor([]()->type*{return new type();},_hash_val);\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

// Version without override, so no error for base virtual
#define ASSIGN_VIR_VAR_CONSTRUCTOR_OVERRIDE(type)\
VIR_NAME_TYPE_OVERRIDE(type)\
private:\
static bool _register_##type(){\
Factory::_add_constructor([]()->type*{return new type();},_hash_val);\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

// Macro for process based stuff.
#define ASSIGN_CONSTRUCTOR(type)\
VIR_NAME_TYPE(type)\
CREATE_TYPE(type)\
private:\
static bool _register_##type(){\
Factory::_add_process_constructor([]()->type*{return new type();},_hash_val);\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

// Version without override, so no error for base virtual
#define ASSIGN_CONSTRUCTOR_OVERRIDE(type)\
VIR_NAME_TYPE_OVERRIDE(type)\
CREATE_TYPE(type)\
private:\
static bool _register_##type(){\
Factory::_add_process_constructor([]()->type*{return new type();},_hash_val);\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

#define EXTERNAL_VAR_CONSTRUCTOR(type)\
EXTERNAL_NAME_TYPE(type)\
static bool _register_##type(){\
Factory::_add_constructor([]()->type*{return new type();},_get_##type##_hash());\
return true;\
}\
inline static const bool _reg_trigger_##type=_register_##type();

// The below should be used with exactly seventeen grains of salt only to create load functions for other libraries values.
// See details on each, please!
// All should be called after EXTERNAL_VAR_CONSTRUCTOR()

// used as OVERRIDE_LOAD(A) {A a; a.does_things = load_data<bool>(); return a;}
#define OVERRIDE_LOAD(type) template<> inline type Loader::load_complex<type>()

// used as OVERRIDE_SAVE(A)(A&) {save_data(a.does_things);}
#define OVERRIDE_SAVE(type) template<> inline void Saver::save_complex<type>