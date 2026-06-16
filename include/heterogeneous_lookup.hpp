#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess 
{
    using is_transparent = void;
    bool operator()(std::string_view l, std::string_view r)const
    {
        return l < r;
    }

    bool operator()(const std::string& l, std::string_view r)const
    {
        return l < r;
    }

    bool operator()(std::string_view l, const std::string& r)const
    {
        return l < r;
    }

    bool operator()(const char* l, std::string_view r) const 
    {
        return std::string_view(l) < r;
    }

    bool operator()(std::string_view l, const char* r) const 
    {
        return l < std::string_view(r);
    }

};

struct TransparentStringEqual 
{
    using is_transparent = void;
    bool operator()(std::string_view l, std::string_view r) const 
    { 
        return l == r; 
    }

    bool operator()(const std::string& l, std::string_view r) const 
    { 
        return l == r; 
    }

    bool operator()(std::string_view l, const std::string& r) const 
    { 
        return l == r; 
    }

    bool operator()(const char* l, std::string_view r) const 
    { 
        return std::string_view(l) == r; 
    }

    bool operator()(std::string_view l, const char* r) const 
    { 
        return l == std::string_view(r); 
    }
};

struct TransparentStringHash 
{
    using is_transparent = void;

    std::size_t operator()(std::string_view key) const 
    { 
        return std::hash<std::string_view>{}(key); 
    }

    std::size_t operator()(const std::string& key) const 
    { 
        return std::hash<std::string_view>{}(std::string_view(key)); 
    }

    std::size_t operator()(const char* key) const 
    { 
        return std::hash<std::string_view>{}(std::string_view(key)); 
    }
};

}  // namespace bookdb
