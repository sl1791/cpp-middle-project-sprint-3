#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess 
{
    bool operator()(std::string_view a_L, std::string_view a_R)const
    {
        return a_L < a_R;
    }
};

struct TransparentStringEqual {};

struct TransparentStringHash {};

}  // namespace bookdb
