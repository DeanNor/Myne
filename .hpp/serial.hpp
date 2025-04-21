
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <functional>

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>

#define max_t uint64_t

namespace __ARCHIVE
{
    template <typename T>
    struct serializer 
    {
        std::function<void(cereal::JSONOutputArchive&, T*)> json_serializer;
        std::function<void(cereal::BinaryOutputArchive&, T*)> binary_serializer;

        template <class Archive>
        void use_serializer(Archive& archive, T* what)
        {
            if constexpr (std::is_same_v<Archive, cereal::BinaryOutputArchive>)
            {
                binary_serializer(archive, what);
            }

            else
            {
                std::cout << "HI" << std::endl;
                json_serializer(archive, what);
            }
        }

        std::function<void(cereal::JSONInputArchive&, T*)> json_deserializer;
        std::function<void(cereal::BinaryInputArchive&, T*)> binary_deserializer;

        template <class Archive>
        void use_deserializer(Archive& archive, T* what)
        {
            if constexpr (std::is_same_v<Archive, cereal::BinaryInputArchive>)
            {
                binary_deserializer(archive, what);
            }

            else
            {
                json_deserializer(archive, what);
            }
        }
    };
}

#define BASE_SERIALIZE_INIT(type) \
template<> template<> void serializer<type>::use_serializer(cereal::BinaryOutputArchive& archive, type* what) \
{ \
    binary_serializer(archive, what); \
} \
template<> template<> void serializer<type>::use_serializer(cereal::JSONOutputArchive& archive, type* what) \
{ \
    json_serializer(archive, what); \
} \
template<> template<> void serializer<type>::use_deserializer(cereal::BinaryInputArchive& archive, type* what) \
{ \
    binary_deserializer(archive, what); \
} \
template<> template<> void serializer<type>::use_deserializer(cereal::JSONInputArchive& archive, type* what) \
{ \
    json_deserializer(archive, what); \
} \

#define BASE_INIT(type) \
namespace __ARCHIVE \
{ \
    BASE_SERIALIZE_INIT(type) \
} \

template <typename T>
class ObjFactory {
public:
    static void registerType(std::string name, std::function<T*()> ctor);

    static void registerSerializers(std::string name, __ARCHIVE::serializer<T> serializers);

    static T* create(const std::string& name);

    template <class Archive>
    static void saveToArchive(Archive& archive, T* what);

    template <class Archive, typename C>
    static void loadFromArchive(Archive& archive, C*& what);

private:
    static std::unordered_map<std::string, __ARCHIVE::serializer<T>>& get_serializer_map();

    static std::unordered_map<std::string, std::function<T*()>>& get_map();
};

class Process;
typedef ObjFactory<Process> ProcessFactory;

#define REGISTER_OBJECT(type) \
public: \
    virtual std::string _get_type_name() const { return #type; } \
private: \
    static bool _register_##type() { \
        ProcessFactory::registerType(#type, []() -> type* { return new type(); }); \
        __ARCHIVE::serializer<Process> serializers; \
        serializers.json_serializer = [](cereal::JSONOutputArchive& ar, Process* base) { ar(static_cast<type&>(*base)); }; \
        serializers.binary_serializer = [](cereal::BinaryOutputArchive& ar, Process* base) { ar(static_cast<type&>(*base)); }; \
        serializers.json_deserializer = [](cereal::JSONInputArchive& ar, Process* base) { ar(static_cast<type&>(*base)); }; \
        serializers.binary_deserializer = [](cereal::BinaryInputArchive& ar, Process* base) { ar(static_cast<type&>(*base)); }; \
        ProcessFactory::registerSerializers(#type, serializers); \
        return true; \
    } \
    static inline const bool _reg_trigger_##type = _register_##type();

#define REGISTER_CLASS(type, base_class) \
public: \
    virtual std::string _get_type_name() const { return #type; } \
private: \
    static bool _register_##type() { \
        ObjFactory<base_class>::registerType(#type, []() -> type* { return new type(); }); \
        __ARCHIVE::serializer<base_class> serializers; \
        serializers.json_serializer = [](cereal::JSONOutputArchive& ar, base_class* base) { ar(static_cast<type&>(*base)); }; \
        serializers.binary_serializer = [](cereal::BinaryOutputArchive& ar, base_class* base) { ar(static_cast<type&>(*base)); }; \
        serializers.json_deserializer = [](cereal::JSONInputArchive& ar, base_class* base) { ar(static_cast<type&>(*base)); }; \
        serializers.binary_deserializer = [](cereal::BinaryInputArchive& ar, base_class* base) { ar(static_cast<type&>(*base)); }; \
        ObjFactory<base_class>::registerSerializers(#type, serializers); \
        return true; \
    } \
    static inline const bool _reg_trigger_##type = _register_##type();


#define ARCHIVE_BASE(...) \
public: \
    template <class Archive> \
    void save(Archive& ar) const { ar(__VA_ARGS__); } \
    template <class Archive> \
    void load(Archive& ar) { ar(__VA_ARGS__); } \

#define ARCHIVE(parent, ...) \
public: \
    template <class Archive> \
    void save(Archive& ar) const { ar(cereal::base_class<parent>(this), __VA_ARGS__); } \
    template <class Archive> \
    void load(Archive& ar) { ar(cereal::base_class<parent>(this), __VA_ARGS__); } \

#define ARCHIVE_INHERIT(parent) \
public: \
    template <class Archive> \
    void save(Archive& ar) const { ar(cereal::base_class<parent>(this)); } \
    template <class Archive> \
    void load(Archive& ar) { ar(cereal::base_class<parent>(this)); } \

template <typename T>
void ObjFactory<T>::registerType(std::string name, std::function<T*()> ctor)
{
    get_map()[name] = ctor;
}

template <typename T>
void ObjFactory<T>::registerSerializers(std::string name, __ARCHIVE::serializer<T> serializers)
{
    get_serializer_map()[name] = serializers;
}

template <typename T>
T* ObjFactory<T>::create(const std::string& name)
{
    auto it = get_map().find(name);
    if (it != get_map().end()) return (it->second)();
    throw std::runtime_error("Unknown type: " + name);
}

template <typename T>
template <class Archive>
void ObjFactory<T>::saveToArchive(Archive& archive, T* what)
{
    std::string typeName = what->_get_type_name();
    archive(typeName);

    auto& serializer_map = get_serializer_map();
    auto it = serializer_map.find(typeName);
    if (it != serializer_map.end()) {
        it->second.use_serializer(archive, what);
    } else {
        throw std::runtime_error("No serializer registered for type: " + typeName);
    }
}

template <typename T>
template <class Archive, typename C>
void ObjFactory<T>::loadFromArchive(Archive& archive, C*& what)
{
    std::string type;
    archive(type);

    T* temp = create(type);

    auto& serializer_map = get_serializer_map();
    auto it = serializer_map.find(type);
    
    if (it != serializer_map.end())
    {
        it->second.use_deserializer(archive, temp);
    }
    
    else
    {
        throw std::runtime_error("No deserializer registered for type: " + type);
    }

    what = static_cast<C*>(temp);
}

// TODO combine these into one object, probably fit the bottom into __ARCHIVE::serializer<T>
template <typename T>
std::unordered_map<std::string, __ARCHIVE::serializer<T>>& ObjFactory<T>::get_serializer_map()
{
    static std::unordered_map<std::string, __ARCHIVE::serializer<T>> map_val;
    return map_val;
}

template <typename T>
std::unordered_map<std::string, std::function<T*()>>& ObjFactory<T>::get_map()
{
    static std::unordered_map<std::string, std::function<T*()>> map_val;
    return map_val;
}