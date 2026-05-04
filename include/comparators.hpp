#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor
{
    bool operator()(const Book& a, const Book& b) const
    {
        return a.author < b.author;
    }
};

struct LessByTitle
{
    bool operator()(const Book& a, const Book& b) const
    {
        return a.title < b.title;
    }
};

struct LessByYear
{
    bool operator()(const Book& a, const Book& b) const
    {
        return a.year < b.year;
    }
};

struct LessByRating
{
    bool operator()(const Book& a, const Book& b) const
    {
        return a.rating > b.rating;
    }
};

struct LessByReadCount
{
    bool operator()(const Book& a, const Book& b) const
    {
        return a.read_count > b.read_count;
    }
};

struct LessByGenre
{
    bool operator()(const Book& a, const Book& b) const
    {
        return static_cast<int>(a.genre) < static_cast<int>(b.genre);
    }
};

struct LessByPopularity 
{
    bool operator()(const Book& lhs, const Book& rhs) const 
    {
        return lhs.read_count > rhs.read_count;
    }
};

}  // namespace bookdb::comp